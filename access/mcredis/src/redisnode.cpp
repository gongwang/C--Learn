#include "redisconnection.h"
#include "mcredis.h"
#include "stdlib.h"
#define DEFAULT_CONNECT_TIMEOUT     1000        //Ĭ�ϵ��������ӳ�ʱʱ��
CRedisNode::CRedisNode() : m_nTimeout(0), m_bHaveSlave(false), m_bConnStatus(false), m_bInit(false)
{

}

CRedisNode::~CRedisNode()
{
    RedisDisConnect();
}
/*====================================================================
������ ��Init
���� ����ʼ�����ӽڵ���Ϣ
�㷨ʵ�֣�
����˵����
         TRedisNodeInfo &tNodeInfo �ڵ���Ϣ��ע�⣺���ô˺����ɹ���
         ������Ϣ���ٿ��ã���Ϊ�ڲ�ʹ�õ�stl��swap����
         std::string &strPassword �������ݿ�����룬ִ����Ϻ��Կ���
����ֵ˵�� ��bool 
====================================================================*/
bool CRedisNode::Init(CMCRedis *pcMcRedis,  TRedisNodeInfo &tNodeInfo, std::string &strPassword, unsigned int nTimeout )
{
    if (m_bInit)
    {
        m_pcMcParent->PrintLog(MC_LOG_WARN_LEVEL, "CRedisNode has been inited!\n");
        return false;
    }
    if (NULL == pcMcRedis)
    {
        return false;
    }
    m_pcMcParent = pcMcRedis;
    //��������ȫ��������
    m_tNodeInfo.nodeSlots.swap(tNodeInfo.nodeSlots);
    memcpy(&m_tNodeInfo.masterConn, &tNodeInfo.masterConn, sizeof(m_tNodeInfo.masterConn));
    m_tNodeInfo.masterConn.pRedisCtx = NULL;
    m_tNodeInfo.slaveConn.swap(tNodeInfo.slaveConn);
    //����������ݣ�ȷ��������ֵ��ȫ����
    memset(&tNodeInfo.masterConn, 0, sizeof(tNodeInfo.masterConn));

    m_nTimeout = nTimeout;
    if (m_nTimeout == 0)
    {
        //���ó�ʱĬ��ֵ
        m_nTimeout = DEFAULT_CONNECT_TIMEOUT;
    }
    m_strPassword.assign(strPassword);
    if (m_tNodeInfo.slaveConn.size() > 0)
    {
        m_bHaveSlave = true;
    }
    m_bInit = true;
    m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "CRedisNode inite success!\n");
    return true;
}

BOOL32 CRedisNode::RedisConnect()
{
    //����������ؽڵ�
    m_bConnStatus = FALSE;
    bool bSlaveOk = true;  
    redisContext *pCtx = NULL;
    m_tNodeInfo.masterConn.pRedisCtx = NULL;
    pCtx = ConnectWithTimeout(m_tNodeInfo.masterConn);
    if(NULL == pCtx)
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "RedisConnect master node failed!\n");
        return FALSE;
    }
    if(!Auth(pCtx))
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "RedisConnect master node password error!\n");
        g_predisFree(pCtx);
        return FALSE;
    }
    m_tNodeInfo.masterConn.pRedisCtx = pCtx;
    m_tNodeInfo.masterConn.bConnected = true;
    std::vector<TRedisConnInfo>::iterator iteSlave = m_tNodeInfo.slaveConn.begin();
    for (; iteSlave != m_tNodeInfo.slaveConn.end(); ++iteSlave)
    {
        iteSlave->pRedisCtx = NULL;
        pCtx = ConnectWithTimeout(*iteSlave);
        if (NULL == pCtx)
        {
            bSlaveOk = false;
        }
        else
        {
            if(!Auth(pCtx))
            {
                m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "RedisConnect slave node password error!\n");
                g_predisFree(pCtx);
                bSlaveOk = false;
            }
            else
            {
                CThreadAutoLock cLock(m_clmLock);     //��������Ϊ���߳�ͬʱʹ��hiredis����ᵼ�����ݴ���
                //�ӽڵ����Ӻ�����ֶ�ָ��Ϊreadonlyģʽ�����ܻ�����ݷ���
                bool bSuccess = false;
                redisReply *reply = static_cast<redisReply *>(g_predisCommand(pCtx, "READONLY"));
                if(NULL != reply)
                {
                    if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp(reply->str, "OK"))
                    {
                        iteSlave->pRedisCtx = pCtx;
                        iteSlave->bConnected = true;
                        bSuccess = true;
                    }
                    else
                    {
                        bSuccess = false;
                        
                    }
                    g_pfreeReplyObject(reply);
                }
                else
                {
                    bSuccess = false;
                }
                if (!bSuccess)
                {
                    g_predisFree(pCtx);
                }
            }
        }
        
    }
    if (bSlaveOk)
    {
        m_bConnStatus = TRUE;
        return TRUE;
    }
    m_bConnStatus = 2;
    return 2;       //�������ӣ�����2
}
//����disconnect֮���ܵ���reconnect����Ҫ����connect
BOOL32 CRedisNode::RedisReConnect()
{
    //����������ؽڵ� 
    //��ֱ�ӵ���connect����Ϊ����hiredis��Ҫ�ȿ��µ����Ӳ��ܶϿ�������
    //����ᵼ������ʧ��
    m_bConnStatus = FALSE;
    bool bSlaveOk = true;  
    redisContext *pCtx = NULL;
    pCtx = ConnectWithTimeout(m_tNodeInfo.masterConn);
    if(NULL == pCtx)
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "RedisConnect master node failed!\n");
        return FALSE;
    }
    if(!Auth(pCtx))
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "RedisConnect master node password error!\n");
        g_predisFree(pCtx);
        return FALSE;
    }
    if (NULL != m_tNodeInfo.masterConn.pRedisCtx )
    {
        g_predisFree(m_tNodeInfo.masterConn.pRedisCtx);
    }
    m_tNodeInfo.masterConn.bConnected = true;
    m_tNodeInfo.masterConn.pRedisCtx = pCtx;
    std::vector<TRedisConnInfo>::iterator iteSlave = m_tNodeInfo.slaveConn.begin();
    for (; iteSlave != m_tNodeInfo.slaveConn.end(); ++iteSlave)
    {
        iteSlave->bConnected = false;
        pCtx = ConnectWithTimeout(*iteSlave);
        if (NULL == pCtx)
        {
            bSlaveOk = false;
        }
        else
        {
            if(!Auth(pCtx))
            {
                m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "RedisConnect slave node password error!\n");
                g_predisFree(pCtx);
                bSlaveOk = false;
            }
            else
            {
                if (NULL != iteSlave->pRedisCtx)
                {
                    g_predisFree(iteSlave->pRedisCtx);
                }
                iteSlave->pRedisCtx = pCtx;
                iteSlave->bConnected = true;
            }
        }

    }
    if (bSlaveOk)
    {
        return TRUE;
    }
    return 2;       //�������ӣ�����2
}

bool CRedisNode::RedisDisConnect()
{
    std::vector<TRedisConnInfo>::iterator iteSlave = m_tNodeInfo.slaveConn.begin();
    for (; iteSlave != m_tNodeInfo.slaveConn.end(); ++iteSlave)
    {
        if (NULL == iteSlave->pRedisCtx)
        {
            continue;
        }
        g_predisFree(iteSlave->pRedisCtx);
        iteSlave->pRedisCtx = NULL;
        iteSlave->bConnected = false;
    }
    if (NULL != m_tNodeInfo.masterConn.pRedisCtx)
    {
        g_predisFree(m_tNodeInfo.masterConn.pRedisCtx);
        m_tNodeInfo.masterConn.pRedisCtx = NULL;
        m_tNodeInfo.masterConn.bConnected = false;
    }
    m_bConnStatus = FALSE;
    return true;
}

bool CRedisNode::RedisReConSlave( TRedisConnInfo &tSlaveConnInfo )
{
    redisContext *pCtx = NULL;
    pCtx = ConnectWithTimeout(tSlaveConnInfo);
    if(NULL == pCtx)
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "RedisConnect master node failed!\n");
        return false;
    }
    if(!Auth(pCtx))
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "RedisConnect master node password error!\n");
        g_predisFree(pCtx);
        return false;
    }
    if (NULL != tSlaveConnInfo.pRedisCtx )
    {
        g_predisFree(tSlaveConnInfo.pRedisCtx);
    }
    tSlaveConnInfo.bConnected = true;
    tSlaveConnInfo.pRedisCtx = pCtx;
    return true;
}

BOOL32 CRedisNode::Ping()
{
    if (!m_bConnStatus)
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisNode::Ping ERR:node not connected!\n");
        return false;
    }
    CThreadAutoLock cLock(m_clmLock);     //��������Ϊ���߳�ͬʱʹ��hiredis����ᵼ�����ݴ���
    BOOL32 bResult = FALSE;
    //ping master node
    redisReply *reply = static_cast<redisReply *>(g_predisCommand(m_tNodeInfo.masterConn.pRedisCtx, "PING"));
    bool bRet = (NULL != reply);
    bResult = bRet;
    if(bRet)
    {
        g_pfreeReplyObject(reply);
        m_tNodeInfo.masterConn.bConnected = true;
    }
    bool bSlaveOk = true;
    std::vector<TRedisConnInfo>::iterator iteSlave = m_tNodeInfo.slaveConn.begin();
    for (; iteSlave != m_tNodeInfo.slaveConn.end(); ++iteSlave)
    {
        if (NULL != iteSlave->pRedisCtx &&iteSlave->bConnected)
        {
            reply = static_cast<redisReply *>(g_predisCommand(iteSlave->pRedisCtx, "PING"));
            bool bRet = (NULL != reply);
            if(bRet)
            {
                g_pfreeReplyObject(reply);
            }
            else
            {
                iteSlave->bConnected = false;
            }
        }
        if (NULL == iteSlave->pRedisCtx || false == iteSlave->bConnected)
        {
            //�ڵ����ڲ���slave�ڵ��Զ�����
            if(!RedisReConSlave(*iteSlave))
            {
                bSlaveOk = false;
            }
            continue;
        }
    }
    if (bResult && !bSlaveOk)
    {
        bResult = 2;
    }
    return bResult;
}

redisContext * const CRedisNode::GetConnection( int nConType )
{
    if (!m_bConnStatus)
    {
        return NULL;
    }
    if (!m_bHaveSlave)
    {
        nConType = REDIS_MASTER;
    }
    if (REDIS_MASTER == nConType )
    {
        if (NULL != m_tNodeInfo.masterConn.pRedisCtx && m_tNodeInfo.masterConn.bConnected)
        {
            return m_tNodeInfo.masterConn.pRedisCtx;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return GetSlaveConnection();
    }
}

redisContext * const CRedisNode::GetSlaveConnection()
{
    //TODO �ò��ԶԸ����������ڵ���и��ؾ���
    redisContext *pCtx = NULL;
    if (m_bHaveSlave && m_bConnStatus)
    {
        redisContext **pValidCtx = new redisContext*[m_tNodeInfo.slaveConn.size()];
        int nValidCtxNum = 0;
        std::vector<TRedisConnInfo>::iterator iteSlave = m_tNodeInfo.slaveConn.begin();
        for (; iteSlave != m_tNodeInfo.slaveConn.end(); ++iteSlave)
        {
            if (NULL == iteSlave->pRedisCtx || false == iteSlave->bConnected)
            {
                continue;
            }
            pValidCtx[nValidCtxNum++] = iteSlave->pRedisCtx;
        }
        if (0 == nValidCtxNum)
        {
            delete[] pValidCtx;
            return GetConnection(REDIS_MASTER);
        }
        //get a random number
        srand(time(NULL));
        int nRandIdx = rand() % nValidCtxNum;
        pCtx = pValidCtx[nRandIdx];
        delete[] pValidCtx;
        return pCtx;
    }
    else
    {
        return NULL;
    }
}

bool CRedisNode::CheckSlot( unsigned int nSlot )
{
    std::vector<TRedisNodeSlotsRange>::iterator iteSlots = m_tNodeInfo.nodeSlots.begin();
    for (; iteSlots != m_tNodeInfo.nodeSlots.end() ;++iteSlots)
    {
        if (nSlot >= iteSlots->slotStart && nSlot <= iteSlots->slotEnd)
        {
            return true;
        }
    }
    return false;
}

eMCValueType CRedisNode::GetKeyType(const s8* pszKey)
{
    redisContext *pRedisCtx = GetConnection(REDIS_SLAVE);
    if (NULL == pRedisCtx)
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisNode::GetKeyType ERR: get connection failed! no valid connection\n");
        return MC_TYPE_ERR;
    }
    CThreadAutoLock cLock(m_clmLock);     //��������Ϊ���߳�ͬʱʹ��hiredis����ᵼ�����ݴ���
    redisReply *typeReply = static_cast<redisReply *>(g_predisCommand(pRedisCtx, "TYPE %s", pszKey));
    if (NULL == typeReply || REDIS_REPLY_STATUS != typeReply->type)
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisNode::GetKeyType ERR: command return invalid value\n");
        return MC_TYPE_ERR;
    }
    if (0 == strcasecmp(typeReply->str, "string"))
    {
        return MC_TYPE_STRING;
    }
    else if (0 == strcasecmp(typeReply->str, "hash"))
    {
        return MC_TYPE_HASH;
    }
    else if (0 == strcasecmp(typeReply->str, "set"))
    {
        return MC_TYPE_SETS;
    }
    else if (0 == strcasecmp(typeReply->str, "list"))
    {
        return MC_TYPE_LIST;
    }
    else if (0 == strcasecmp(typeReply->str, "none"))
    {
        m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "CRedisNode::GetKeyType FAILED: key %s not exist!\n", pszKey);
        return MC_TYPE_ERR;
    }
    else
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisNode::GetKeyType ERR: unsupported type!\n");
        return MC_TYPE_ERR;
    }
}

bool CRedisNode::ExecCommand( int nConType,OUT redisReply *&reply, const char* cmd, ...)
{
    redisContext *pRedisCtx = GetConnection(nConType);
    if (NULL == pRedisCtx)
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisNode::ExecCommand ERR: get connection failed! no valid connection\n");
        return false;
    }
    CThreadAutoLock cLock(m_clmLock);     //��������Ϊ���߳�ͬʱʹ��hiredis����ᵼ�����ݴ���
    va_list args;
    va_start(args, cmd);
    reply = static_cast<redisReply *>(g_predisvCommand(pRedisCtx, cmd, args));
    va_end(args);
    if (NULL == reply)
    {
        return false;
    }
    return true;
}
bool CRedisNode::AppendCommand( int nConType, const char* cmd, ...)
{
    redisContext *pRedisCtx = GetConnection(nConType);
    if (NULL == pRedisCtx)
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisNode::ExecCommand ERR: get connection failed! no valid connection\n");
        return false;
    }
    CThreadAutoLock cLock(m_clmLock);     //��������Ϊ���߳�ͬʱʹ��hiredis����ᵼ�����ݴ���
    va_list args;
    va_start(args, cmd);
    g_predisAppendCommand(pRedisCtx, cmd, args);
    va_end(args);
    return true;
}
bool CRedisNode::GetReply(int nConType, OUT redisReply **reply)
{
    redisContext *pRedisCtx = GetConnection(nConType);
    if (NULL == pRedisCtx)
    {
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisNode::ExecCommand ERR: get connection failed! no valid connection\n");
        return false;
    }
    CThreadAutoLock cLock(m_clmLock);     //��������Ϊ���߳�ͬʱʹ��hiredis����ᵼ�����ݴ���
    int nRet = g_predisGetReply(pRedisCtx, (void**)reply);
    if (REDIS_ERR == nRet)
    {
        return false;
    }
    return true;
}

bool CRedisNode::Auth(redisContext *pCtx)
{
    bool bRet = false;
    if (0 == m_strPassword.length()) {
        bRet = true;
    } else {
        CThreadAutoLock cLock(m_clmLock);     //��������Ϊ���߳�ͬʱʹ��hiredis����ᵼ�����ݴ���
        redisReply *reply = static_cast<redisReply *>(g_predisCommand(pCtx, "AUTH %s", m_strPassword.c_str()));
        if ((NULL == reply) || (strcasecmp(reply->str, "OK") != 0)) {
            bRet = false;
        } else {
            bRet = true;    
        }
        g_pfreeReplyObject(reply);
    }
    m_pcMcParent->PrintLog(bRet ? MC_LOG_DBG_LEVEL : MC_LOG_ERR_LEVEL, "CRedisNode::auth  %s!\n",
        bRet ? "success" : "failed");
    return bRet;
}

redisContext * CRedisNode::ConnectWithTimeout(TRedisConnInfo &tConnInfo)
{
    CThreadAutoLock cLock(m_clmLock);     //��������Ϊ���߳�ͬʱʹ��hiredis����ᵼ�����ݴ���
    struct timeval timeoutVal;
    timeoutVal.tv_sec = m_nTimeout / 1000;              //����Ϊ��
    timeoutVal.tv_usec = (m_nTimeout % 1000) * 1000;    //΢��

    redisContext *ctx = NULL;
    ctx = g_predisConnectWithTimeout(tConnInfo.szIPAddr, tConnInfo.nPort, timeoutVal);
    if (NULL == ctx || ctx->err) {
        if (NULL != ctx) {
            g_predisFree(ctx);
            ctx = NULL;
        } else {

        }
        m_pcMcParent->PrintLog(MC_LOG_ERR_LEVEL, "CRedisNode::ConnectWithTimeout ERR:connect to node %s:%d failed!\n",
            tConnInfo.szIPAddr, tConnInfo.nPort);
    } 
    m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "CRedisNode connect to node %s:%d success!\n",
        tConnInfo.szIPAddr, tConnInfo.nPort);
    return ctx;
}

void CRedisNode::PrintNodeInfo()
{
    m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "\tmasternode:%s:%d id:%s.slots:",
        m_tNodeInfo.masterConn.szIPAddr, m_tNodeInfo.masterConn.nPort, m_tNodeInfo.masterConn.szConnGUID);
    std::vector<TRedisNodeSlotsRange>::iterator iteSlots = m_tNodeInfo.nodeSlots.begin();
    int i = 0;
    for (i = 0; iteSlots != m_tNodeInfo.nodeSlots.end(); ++iteSlots)
    {
        if (0 != i)
        {
            m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, ",");
        }
        m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "%d-%d", iteSlots->slotStart, iteSlots->slotEnd);
    }
    m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "\n");
    std::vector<TRedisConnInfo>::iterator iteSlave = m_tNodeInfo.slaveConn.begin();
    for (i = 0; iteSlave != m_tNodeInfo.slaveConn.end(); ++iteSlave)
    {
        m_pcMcParent->PrintLog(MC_LOG_DBG_LEVEL, "\t\tslavenodes:%s:%d id:%s\n",
            iteSlave->szIPAddr, iteSlave->nPort, iteSlave->szConnGUID);
    }
}
