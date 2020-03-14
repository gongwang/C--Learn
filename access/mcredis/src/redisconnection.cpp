
#include "redisconnection.h"
#include "mcredis.h"
#include "crc.h"
//目前redis3.0的集群slot hash方法：计算出key的crc16，然后对16384取模
static u32 GetSlotFromKey(const s8* pszKey, u32 nLength)
{
    int nSlot = GetCRC16((const u8*)pszKey, nLength);
    return nSlot % REDIS_SLOTS_NUM;
}
CRedisConnection::CRedisConnection() : m_pcNodeArr(NULL),
    m_bIsCluster(false), m_nNodeNum(0), m_bConnStatus(false),
    m_nTimeout(0), m_bInit(false), m_pcMcParent(NULL)
{
}
CRedisConnection::~CRedisConnection()
{
    if (NULL != m_pcNodeArr)
    {
        delete m_pcNodeArr;
    }
}

bool CRedisConnection::Init(CMCRedis *pcMcRedis,  vecMCServerInfo &vServer, std::string &strPassword, unsigned int nTimeout /*= 0*/ )
{
    if (m_bInit)
    {
        m_pcMcParent->PrintLog(MC_LOG_WARN_LEVEL, "CRedisConnection has been inited!\n");
        return false;
    }
    if (NULL == pcMcRedis)
    {
        return false;
    }
    m_pcMcParent = pcMcRedis;
    if (0 == vServer.size())
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection vecServerInfo is empty!\n");
        return false;
    }
    m_vecServerinfo.assign(vServer.begin(), vServer.end());
    m_strPassword.assign(strPassword);
    m_nTimeout = nTimeout;
    m_bInit = true;
    return true;
}

s32 CRedisConnection::Connect()
{
    m_bConnStatus = false;
    //先使用节点登陆后获取集群所有信息，再连接所有节点
    vecMCServerInfo::iterator iteServer = m_vecServerinfo.begin();
    redisContext *pCtx = NULL;
    for (; iteServer != m_vecServerinfo.end(); ++iteServer)
    {
        pCtx = ConnServerNode(*iteServer, m_strPassword);
        if (NULL != pCtx)
        {
            std::vector<TRedisNodeInfo> vecCLusterNode(GetClusterInfo(pCtx));
            if (vecCLusterNode.size() == 0)
            {
                //单节点实例，非集群
                TRedisNodeInfo tSingleNode;
                strcpy(tSingleNode.masterConn.szIPAddr, iteServer->first.c_str());
                tSingleNode.masterConn.nPort = iteServer->second;
                m_pcNodeArr = new CRedisNode();
                if (NULL == m_pcNodeArr)
                {
                    m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection create node failed!\n");
                    g_predisFree(pCtx);
                    return MC_ERR_SDK;
                }
                m_pcNodeArr->Init(m_pcMcParent, tSingleNode, m_strPassword, m_nTimeout);
                if(!m_pcNodeArr->RedisConnect())
                {
                    delete m_pcNodeArr;
                    m_pcNodeArr = NULL;
                    m_nNodeNum = 0;
                    g_predisFree(pCtx);
                    return MC_ERR_LOGIN_FAILED;
                }
                m_nNodeNum = 1;
                m_bIsCluster = false;
            }
            else
            {
                //集群
                std::vector<TRedisNodeInfo>::iterator iteNode = vecCLusterNode.begin();
                m_nNodeNum = vecCLusterNode.size();
                m_pcNodeArr = new CRedisNode[m_nNodeNum];
                if (NULL == m_pcNodeArr)
                {
                    m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection create node failed!\n");
                    m_nNodeNum = 0;
                    g_predisFree(pCtx);
                    return MC_ERR_SDK;
                }
                for(int i = 0; iteNode != vecCLusterNode.end(); ++iteNode, ++i)
                {
                    if (!m_pcNodeArr[i].Init(m_pcMcParent, *iteNode, m_strPassword, m_nTimeout))
                    {
                        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection init node failed!\n");
                        delete m_pcNodeArr;
                        m_pcNodeArr = NULL;
                        m_nNodeNum = 0;
                        g_predisFree(pCtx);
                        return MC_ERR_INVALID_PARAM;
                    }
                    if(!m_pcNodeArr[i].RedisConnect())
                    {
                        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection connect node failed!\n");
                        delete m_pcNodeArr;
                        m_pcNodeArr = NULL;
                        m_nNodeNum = 0;
                        g_predisFree(pCtx);
                        return MC_ERR_LOGIN_FAILED;
                    }
                }
                m_bIsCluster = true;
            }
            g_predisFree(pCtx);
            m_bConnStatus = true;
            
            return MC_ERR_NO;
        }
    }
    return MC_ERR_LOGIN_FAILED;
}

s32 CRedisConnection::ReConnect()
{
    //先清理原来的所有节点连接，再重新连接
    DisConnect();
    return Connect();
}

s32 CRedisConnection::DisConnect()
{
    for (int i = 0; i < m_nNodeNum; ++i)
    {
        m_pcNodeArr[i].RedisDisConnect();
    }
    delete m_pcNodeArr;
    m_pcNodeArr = NULL;
    m_nNodeNum = 0;
    m_bConnStatus = false;
    return MC_ERR_NO;
}

BOOL32 CRedisConnection::KeepAlive()
{
    if (!m_bConnStatus)
    {
        return FALSE;
    }
    for (int i = 0; i < m_nNodeNum; ++i)
    {
        if(!m_pcNodeArr[i].Ping())
        {
            m_bConnStatus = false;
            return FALSE;
        }
    }
    return TRUE;
}

CRedisNode* CRedisConnection::GetNode( std::string &strKey )
{
    //针对默认集群进行快速匹配
    if (!IsCluster())
    {
        //单节点，非集群，直接返回服务器连接
        return m_pcNodeArr;
    }
    else
    {
        //calc the slots key belong to
        int nSlot = GetSlotFromKey(strKey.c_str(), strKey.length());
        int nQuickIndex = nSlot / (REDIS_SLOTS_NUM / m_nNodeNum);
        //快速匹配，尝试减小运算量，然而服务器返回的节点顺序不是按照slots顺序来的...
        if (nQuickIndex < m_nNodeNum && m_pcNodeArr[nQuickIndex].CheckSlot(nSlot))
        {
            return m_pcNodeArr + nQuickIndex;
        }
        //快速匹配失败，手动匹配
        for (int i = 0; i < m_nNodeNum; ++i)
        {
            if(m_pcNodeArr[i].CheckSlot(nSlot))
            {
                return m_pcNodeArr + i;
            }
        }
    }
    return NULL;
}

CRedisNode* CRedisConnection::GetNode( u32 nNodeIndex )
{
    //针对默认集群进行快速匹配
    if (!IsCluster())
    {
        //单节点，非集群，直接返回服务器连接
        return m_pcNodeArr;
    }
    else
    {
        if(nNodeIndex < m_nNodeNum)
        {
            return m_pcNodeArr + nNodeIndex;
        }
    }
    return NULL;
}

redisContext * CRedisConnection::ConnServerNode( std::pair<std::string, u32> pairNode, std::string &strPassword)
{
    struct timeval timeoutVal;
    timeoutVal.tv_sec = m_nTimeout / 1000;              //参数为秒
    timeoutVal.tv_usec = (m_nTimeout % 1000) * 1000;    //微秒

    redisContext *ctx = NULL;
    ctx = g_predisConnectWithTimeout(pairNode.first.c_str(), pairNode.second, timeoutVal);
    if (NULL == ctx || ctx->err) {
        if (NULL != ctx) {
            g_predisFree(ctx);
            ctx = NULL;
        }
    } 
    if (NULL == ctx)
    {
        return NULL;
    }
    bool bRet = false;
    if (0 == m_strPassword.length()) {
        bRet = true;
    } else {
        redisReply *reply = static_cast<redisReply *>(g_predisCommand(ctx, "AUTH %s", m_strPassword.c_str()));
        if ((NULL == reply) || (strcasecmp(reply->str, "OK") != 0)) {
            bRet = false;
        } else {
            bRet = true;    
        }
        g_pfreeReplyObject(reply);
    }
    if (!bRet)
    {
        g_predisFree(ctx);
        ctx = NULL;
    }
    return ctx;
}

std::vector<TRedisNodeInfo> CRedisConnection::GetClusterInfo( redisContext* pCtx )
{
    //连接建立完成，获取集群信息
    redisReply *reply = static_cast<redisReply *>(g_predisCommand(pCtx, "CLUSTER SLOTS"));
    //closter slots命令返回的数组数据结构如下，如当前服务没开启集群，则返回ERR this distance hase cluster support disabled：
    /*struct node * n
    {
        int startslot;
        int endslot;
        struct mainaddr
        {string szipaddr;int nport;string szguid;};
        struct slaveaddr * n
        {string szipaddr;int nport;string szguid;}
    }*/
    int nNodesCnt = 0;
    std::vector<TRedisNodeInfo> vecRet;
    if (REDIS_REPLY_ARRAY == reply->type) 
    {
        //if (bPrintDbg) printf("cluster nodes number:%d\n",reply->elements);
        nNodesCnt = reply->elements;
        if (0 == nNodesCnt)
        {
            m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection::GetClusterInfo failed!nodes is 0\n");
            return vecRet;
        }
        for (int i = 0; i < reply->elements; ++i)
        {
            redisReply *replyTmp = reply->element[i];
            if (REDIS_REPLY_ARRAY == replyTmp->type) 
            {
                //nodes信息数组部分
                bool bSlotsRangeEnd = false;
                TRedisNodeInfo tNodeInfo;
                TRedisNodeSlotsRange tSlotRangeTmp ;
                TRedisConnInfo   tRedisConnInfo ;
                //if (bPrintDbg) printf("cur nodeinfo elements numbers:%d\n",replyTmp->elements);
                for (int j = 0; j < replyTmp->elements; ++j) 
                { 
                    //单个nodes信息 
                    if (REDIS_REPLY_INTEGER == replyTmp->element[j]->type) 
                    { 
                        if (!bSlotsRangeEnd)
                        {
                            if (0 == j%2)
                            {
                                tSlotRangeTmp.slotEnd = 0;
                                tSlotRangeTmp.slotStart = replyTmp->element[j]->integer;
                            }
                            else
                            {
                                tSlotRangeTmp.slotEnd = replyTmp->element[j]->integer;
                                //结构体添加到节点信息的slot范围数组中
                                tNodeInfo.nodeSlots.push_back(tSlotRangeTmp);
                                //if (bPrintDbg) printf("add a slotrange to node[%d],start:%d,end:%d\n", i, sSlotRangeTmp.slotStart, sSlotRangeTmp.slotEnd);
                            }
                        } 
                        else 
                        {//不应该出现的情况，在节点地址信息之后再出现数字类型 
                            m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection::GetClusterInfo error recv int when slots range ended\n");
                        } 
                    } 
                    else if (REDIS_REPLY_ARRAY == replyTmp->element[j]->type) {
                        for (int k = 0; k < replyTmp->element[j]->elements; ++k) 
                        { 
                            redisReply *replyTmpTmp = replyTmp->element[j]->element[k]; 
                            if (REDIS_REPLY_STRING == replyTmpTmp->type) 
                            { 
                                if (0 == k)
                                {
                                    if (!bSlotsRangeEnd)
                                    { //bSlotsRangeEnd没置为true时，说明获取到的为第一个节点信息，即主节点
                                        //TODO 可能因字符串溢出将之前写入结构体的slots范围vector洗掉
                                        strcpy(tNodeInfo.masterConn.szIPAddr, replyTmpTmp->str);
                                    }
                                    else
                                    {//从节点信息
                                        memset(&tRedisConnInfo, 0, sizeof(tRedisConnInfo));
                                        strcpy(tRedisConnInfo.szIPAddr, replyTmpTmp->str);
                                    }
                                } 
                                else if (2 == k)
                                {
                                    if (!bSlotsRangeEnd)
                                    { //bSlotsRangeEnd没置为true时，说明获取到的为第一个节点信息，即主节点
                                        //TODO 可能因字符串溢出将之前写入结构体的slots范围vector洗掉
                                        strcpy(tNodeInfo.masterConn.szConnGUID, replyTmpTmp->str);
                                    }
                                    else
                                    {//从节点信息
                                        strcpy(tRedisConnInfo.szConnGUID, replyTmpTmp->str);
                                        //校验从节点信息完整性
                                        if (tRedisConnInfo.szIPAddr[0] != 0 && tRedisConnInfo.szConnGUID[0] != 0 && tRedisConnInfo.nPort != 0)
                                        {
                                            //将信息加入从节点数组中
                                            tNodeInfo.slaveConn.push_back(tRedisConnInfo);
//                                             m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "CRedisConnection::GetClusterInfo cluster nodes[%d] slave conn info:ip=%s,port=%d,id=%s.\n", 
//                                                 tRedisConnInfo.szIPAddr, tRedisConnInfo.nPort, tRedisConnInfo.szConnGUID);
                                        }
                                    }
                                } 
                                else 
                                { 
                                    m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection::GetClusterInfo receive unknown string value:%s at k=%d\n", 
                                        replyTmpTmp->str, k);
                                } 
                            } 
                            else if (REDIS_REPLY_INTEGER == replyTmpTmp->type) 
                            { 
                                if (1 == k)
                                {
                                    if (!bSlotsRangeEnd)
                                    { //bSlotsRangeEnd没置为true时，说明获取到的为第一个节点信息，即主节点
                                        tNodeInfo.masterConn.nPort = replyTmpTmp->integer;
                                    }
                                    else
                                    {//从节点信息
                                        tRedisConnInfo.nPort = replyTmpTmp->integer;
                                    }
                                } 
                                else 
                                { 
                                    m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection::GetClusterInfo receive unknown int value:%d at k=%d\n", 
                                        replyTmpTmp->integer, k); 
                                } 
                            } 
                        }
                        bSlotsRangeEnd = true;      //在主节点连接信息处理结束后将节点范围结束标志位置true，之后不会再出现int型的数据
                    } 
                    else 
                    { 
                        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection::GetClusterInfo receive unknown msg type=%d.while i=%d,j=%d\n", 
                            replyTmp->element[j]->type, i, j); 
                    } 
                }
                //数据有效性校验
                if (tNodeInfo.nodeSlots.size() > 0 && strlen(tNodeInfo.masterConn.szIPAddr) > 0 && tNodeInfo.masterConn.nPort != 0)
                {
                    vecRet.push_back(tNodeInfo);
                }
            } 
            else 
            { 
                m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisConnection::GetClusterInfo receive unknown msg type=%d.while i=%d \n", 
                    replyTmp->type, i); 
            } 
        }
    }
    else//REDIS_REPLY_ERROR
    {
        m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "server cluster support disabled.\n");
    }
    return vecRet;
}

void CRedisConnection::PrintServerData()
{
    vecMCServerInfo::iterator iteAddr = m_vecServerinfo.begin();
    m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "server base address:\n");
    for (; iteAddr != m_vecServerinfo.end(); ++iteAddr)
    {
        m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "\thost:%s,port:%d\n", iteAddr->first.c_str(), iteAddr->second);
    }
    if (!m_bConnStatus)
    {
        m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "server not connected!\n");
        return;
    }
    m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "server %s cluster\n", IsCluster() ? "is" : "not");
    if (IsCluster())
    {
        m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "cluster nodes:%d\n", m_nNodeNum);
        for (int i = 0; i < m_nNodeNum; ++i)
        {
            m_pcNodeArr[i].PrintNodeInfo();
        }
    }
}
