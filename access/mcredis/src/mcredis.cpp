
#include "utility/uuid.h"
#include "mcredis.h"
#include <stdlib.h>

#ifdef _LINUX_
#include <dlfcn.h>
#else
#endif	//endif _LINUX_
//
char g_szMCRedisErrorDesc[200];
predisReaderCreate g_predisReaderCreate = NULL;
predisReaderFree g_predisReaderFree = NULL;
predisReaderFeed g_predisReaderFeed = NULL;
predisReaderGetReply g_predisReaderGetReply = NULL;
pfreeReplyObject g_pfreeReplyObject = NULL;
predisvFormatCommand g_predisvFormatCommand = NULL;
predisFormatCommand g_predisFormatCommand = NULL;
predisFormatCommandArgv g_predisFormatCommandArgv = NULL;
predisConnect g_predisConnect = NULL;
predisConnectWithTimeout g_predisConnectWithTimeout = NULL;
predisConnectNonBlock g_predisConnectNonBlock = NULL;
predisConnectBindNonBlock g_predisConnectBindNonBlock = NULL;
predisConnectUnix g_predisConnectUnix = NULL;
predisConnectUnixWithTimeout g_predisConnectUnixWithTimeout = NULL;
predisConnectUnixNonBlock g_predisConnectUnixNonBlock = NULL;
predisConnectFd g_predisConnectFd = NULL;
predisSetTimeout g_predisSetTimeout = NULL;
predisEnableKeepAlive g_predisEnableKeepAlive = NULL;
predisFree g_predisFree = NULL;
predisFreeKeepFd g_predisFreeKeepFd = NULL;
predisBufferRead g_predisBufferRead = NULL;
predisBufferWrite g_predisBufferWrite = NULL;
predisGetReply g_predisGetReply = NULL;
predisGetReplyFromReader g_predisGetReplyFromReader = NULL;
predisAppendFormattedCommand g_predisAppendFormattedCommand = NULL;
predisvAppendCommand g_predisvAppendCommand = NULL;
predisAppendCommand g_predisAppendCommand = NULL;
predisAppendCommandArgv g_predisAppendCommandArgv = NULL;
predisvCommand g_predisvCommand = NULL;
predisCommand g_predisCommand = NULL;
predisCommandArgv g_predisCommandArgv = NULL;
//定时器线程，用于进行instanceid刷新和定时保活
void* McRedisTimerThread(void* args)
{
    //主要工作是定时对数据连接进行保活
    CMCRedis *pcMcRedis = (CMCRedis*)args;
    if (NULL == pcMcRedis)
    {
        printf("KeepAliveThread  err ,param is null\n");
        return NULL;
    }
    //pthread_detach(pthread_self());//将状态改为unjoinable状态，确保线程退出时资源的自动释放
    int nCounter = 0;
    string strInstKey = pcMcRedis->GetInstId();
    while (pcMcRedis->IsLogined())
    {
        //每一次timer处理都判断一次redis连接状态
        if (pcMcRedis->IsRedisConnected() && 0 == nCounter % 30)
        {
            //只有注册了断链回调，才会开线程自动保活，否则需要外部调用keepalive手动保活
            if(pcMcRedis->HasDisconCb())
            {
                if(!pcMcRedis->KeepAlive())
                {
                    pcMcRedis->OnDisconnect();
                }
            }
        }
        if (pcMcRedis->IsRedisConnected() && 0 == nCounter % (INSTID_EXPIRE_TIME / 2))
        {
            if(MC_ERR_NOT_EXISTS == pcMcRedis->Exists(strInstKey))
            {
                //id不存在，重新写入
                pcMcRedis->Set(strInstKey, strInstKey, MC_SET_DEFAULT, INSTID_EXPIRE_TIME);
            }
            pcMcRedis->SetExpireTime(strInstKey,INSTID_EXPIRE_TIME);
        }
        ++nCounter;
        if (nCounter >= 0xFFFF)
        {
            nCounter = 0;
        }
        sleep(1);
    }
    return NULL;
}

#ifdef _LINUX_
#define MCREDISEXPFUNC(dll,funcname) dlsym(dll,funcname)
#define MCREDISRELEASEDLL(dll) dlclose(dll)
#elif defined _WIN32
#define MCREDISEXPFUNC(dll,funcname) GetProcAddress(dll,TEXT(funcname))
#define MCREDISRELEASEDLL(dll) FreeLibrary(dll)
#endif	//endif _LINUX_
//**dll全局导出函数部分
/**===================================================================
函数名 ：MCInit
功能 ： 内存缓冲库初始化
算法实现：
参数说明：
返回值说明 ：BOOL32，标识初始化是否成功
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/8/1       V0.2         刘春光
====================================================================*/
MC_API s32 MCInit()
{
    //使用动态加载方式加载hiredis库
#ifdef _LINUX_
    void* hDll=NULL;
    hDll = dlopen("libhiredis.so",RTLD_NOW );
#elif defined _WIN32
    HINSTANCE hDll = LoadLibrary(TEXT("hiredis.dll"));
#endif	//endif _LINUX_
    if(NULL == hDll)
    {
        return MC_ERR_INIT_FAILED;
    }
    g_predisReaderCreate = (predisReaderCreate)MCREDISEXPFUNC(hDll, "redisReaderCreate");
    g_predisReaderFree = (predisReaderFree)MCREDISEXPFUNC(hDll, "redisReaderFree");
    g_predisReaderFeed = (predisReaderFeed)MCREDISEXPFUNC(hDll, "redisReaderFeed");
    g_predisReaderGetReply = (predisReaderGetReply)MCREDISEXPFUNC(hDll, "redisReaderGetReply");
    g_pfreeReplyObject = (pfreeReplyObject)MCREDISEXPFUNC(hDll, "freeReplyObject");
    g_predisvFormatCommand = (predisvFormatCommand)MCREDISEXPFUNC(hDll, "redisvFormatCommand");
    g_predisFormatCommand = (predisFormatCommand)MCREDISEXPFUNC(hDll, "redisFormatCommand");
    g_predisFormatCommandArgv = (predisFormatCommandArgv)MCREDISEXPFUNC(hDll, "redisFormatCommandArgv");
    g_predisConnect = (predisConnect)MCREDISEXPFUNC(hDll, "redisConnect");
    g_predisConnectWithTimeout = (predisConnectWithTimeout)MCREDISEXPFUNC(hDll, "redisConnectWithTimeout");
    g_predisConnectNonBlock = (predisConnectNonBlock)MCREDISEXPFUNC(hDll, "redisConnectNonBlock");
    g_predisConnectBindNonBlock = (predisConnectBindNonBlock)MCREDISEXPFUNC(hDll, "redisConnectBindNonBlock");
    g_predisConnectUnix = (predisConnectUnix)MCREDISEXPFUNC(hDll, "redisConnectUnix");
    g_predisConnectUnixWithTimeout = (predisConnectUnixWithTimeout)MCREDISEXPFUNC(hDll, "redisConnectUnixWithTimeout");
    g_predisConnectUnixNonBlock = (predisConnectUnixNonBlock)MCREDISEXPFUNC(hDll, "redisConnectUnixNonBlock");
    g_predisConnectFd = (predisConnectFd)MCREDISEXPFUNC(hDll, "redisConnectFd");
    g_predisSetTimeout = (predisSetTimeout)MCREDISEXPFUNC(hDll, "redisSetTimeout");
    g_predisEnableKeepAlive = (predisEnableKeepAlive)MCREDISEXPFUNC(hDll, "redisEnableKeepAlive");
    g_predisFree = (predisFree)MCREDISEXPFUNC(hDll, "redisFree");
    g_predisFreeKeepFd = (predisFreeKeepFd)MCREDISEXPFUNC(hDll, "redisFreeKeepFd");
    g_predisBufferRead = (predisBufferRead)MCREDISEXPFUNC(hDll, "redisBufferRead");
    g_predisBufferWrite = (predisBufferWrite)MCREDISEXPFUNC(hDll, "redisBufferWrite");
    g_predisGetReply = (predisGetReply)MCREDISEXPFUNC(hDll, "redisGetReply");
    g_predisGetReplyFromReader = (predisGetReplyFromReader)MCREDISEXPFUNC(hDll, "redisGetReplyFromReader");
    g_predisAppendFormattedCommand = (predisAppendFormattedCommand)MCREDISEXPFUNC(hDll, "redisAppendFormattedCommand");
    g_predisvAppendCommand = (predisvAppendCommand)MCREDISEXPFUNC(hDll, "redisvAppendCommand");
    g_predisAppendCommand = (predisAppendCommand)MCREDISEXPFUNC(hDll, "redisAppendCommand");
    g_predisAppendCommandArgv = (predisAppendCommandArgv)MCREDISEXPFUNC(hDll, "redisAppendCommandArgv");
    g_predisvCommand = (predisvCommand)MCREDISEXPFUNC(hDll, "redisvCommand");
    g_predisCommand = (predisCommand)MCREDISEXPFUNC(hDll, "redisCommand");
    g_predisCommandArgv = (predisCommandArgv)MCREDISEXPFUNC(hDll, "redisCommandArgv");
    printf("g_predisConnectWithTimeout:%p,g_predisCommand:%p,g_predisvCommand:%p\n",
        g_predisConnectWithTimeout, g_predisCommand, g_predisvCommand);
    //MCREDISRELEASEDLL(hDll);
    return MC_ERR_NO;
}
/**===================================================================
函数名 ：MCGetErrDesc
功能 ： 获取错误码的描述字符串
算法实现：
参数说明：
         s32 nErrCode     错误码
返回值说明 ：s8* 错误码字符串
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/8/1       V0.2         刘春光
====================================================================*/
MC_API const s8* MCGetErrDesc(s32 nErrCode)
{
    switch (nErrCode)
    {
    case MC_ERR_NO:
        return "无错误";
    case MC_ERR_INIT_FAILED:
#ifdef _LINUX_
        return dlerror(); 
#elif defined _WIN32
        return itoa(GetLastError(), g_szMCRedisErrorDesc, 10);
#endif	//endif _LINUX_
    case MC_ERR_UNKNOWN:
        return "未知错误";
    case MC_ERR_EXISTS:
        return "数据存在";
    case MC_ERR_NOT_EXISTS:
        return "数据不存在";
    case MC_ERR_SDK:
        return "SDK内部错误";
    case MC_ERR_INVALID_PARAM:
        return "参数错误";
    case MC_ERR_WRONG_LOGIC:
        return "逻辑顺序错误";
    case MC_ERR_TIMEOUT:
        return "超时";
    case MC_ERR_SERVER_DOWN:
        return "服务器连接掉线";
    }
}
/**===================================================================
函数名 ：MCCreateObject
功能 ： 创建一个连接对象
算法实现：
参数说明：
返回值说明 ：IMCInterface接口的指针
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/8/1       V0.2         刘春光
====================================================================*/
MC_API IMCInterface* MCCreateObject()
{
    return new CMCRedis();
}
/**===================================================================
函数名 ：MCReleaseObject
功能 ： 删除指定的连接对象
算法实现：
参数说明：
        IMCInterface* pInterface    接口的指针
返回值说明 ：
---------------------------------------------------------------------
修改记录：
日 期           版本         修改人             走读人             修改记录
2017/8/1       V0.2         刘春光
====================================================================*/
MC_API s32 MCReleaseObject(IMCInterface* pInterface)
{
    if (NULL != pInterface)
    {
        delete pInterface;
    }
    return MC_ERR_NO;
}
//**class具体实现部分
CMCRedis::CMCRedis() : m_bLogined(false), m_pfLogCb(NULL), m_pDisconCb(NULL), m_pUser(NULL), m_strInstId(INSTID_KEY_PREFIX)
{
    memset(&m_pthread, 0, sizeof(m_pthread));
    //生成uuid
    CUUID uuid;
    m_strInstId += uuid;
}
CMCRedis::~CMCRedis()
{
    Logout();
}
BOOL32 CMCRedis::Init( MCLogCallBack pLogCallBack, MCOnDisconnectCB pDisConnectCB, void* pUser )
{
    if (NULL == pLogCallBack)
    {
        return FALSE;
    }
    m_pfLogCb = pLogCallBack;
    m_pDisconCb = pDisConnectCB;
    m_pUser = pUser;
    return TRUE;
}

s32 CMCRedis::Login( vecMCServerInfo &vServer, std::string &strUserName, std::string &strPassword, s32 nTimeout /*= 1000 */ )
{
    if(m_bLogined) return MC_ERR_WRONG_LOGIC;
    if(!m_cRedisCon.Init(this, vServer, strPassword, nTimeout))
    {
        return MC_ERR_SDK;
    }
    s32 nLoginRet = m_cRedisCon.Connect();
    if (MC_ERR_NO != nLoginRet)
    {
        return nLoginRet;
    }
    m_bLogined = true;
    //创建定时器线程
    int ret = pthread_create(&m_pthread, NULL, McRedisTimerThread, this);
    if (ret != 0)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "pthread create McRedisTimerThread failed: error_code=%d\n", ret );
        m_cRedisCon.DisConnect();
        return MC_ERR_SDK;
    }
    return MC_ERR_NO;
}

s32 CMCRedis::ReConnect( s32 nTimeout )
{
    return m_cRedisCon.ReConnect();
}

s32 CMCRedis::Logout()
{
    if(m_bLogined)
    {
        m_bLogined = false;
        s32 nRet = m_cRedisCon.DisConnect();
        //等待定时线程退出
        pthread_join(m_pthread,NULL);
        return nRet;
    }
    return MC_ERR_NO;
}

s32 CMCRedis::GetState()
{
    return m_bLogined && IsRedisConnected();
}

void CMCRedis::PrintServerData()
{
    if (!m_bLogined)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "PrintServerData ERROR: server not logined!");
        return;
    }
    m_cRedisCon.PrintServerData();
}

s32 CMCRedis::GetString(std::string &strKey, OUT std::string &strValue)
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "GetString get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "GET %b", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetString command no response!\n");
        return MC_ERR_SDK;
    }
    bool bResult = true;
    if (REDIS_REPLY_NIL == reply->type )
    {
        strValue.clear();
        nResult = MC_ERR_NOT_EXISTS;
    }else if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_STRING != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetString wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        strValue.assign(reply->str);
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::MultiGetString(std::vector<std::string> &vecKeys, OUT std::vector<std::string> &vecValues)
{
    s32 nResult = MC_ERR_NO;
    if (0 == vecKeys.size())
    {
        return MC_ERR_INVALID_PARAM;
    }
    vecValues.clear();
    redisReply *reply = NULL;
    if (!m_cRedisCon.IsCluster())
    {
        //服务器不是集群，直接执行mget就可以
        CRedisNode *pNode = m_cRedisCon.GetNode(vecKeys[0]);
        if (NULL == pNode)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "MultiGetString get connection failed!\n");
            return MC_ERR_SDK;
        }
        std::vector<std::string>::iterator iteKey = vecKeys.begin();
        for (; iteKey != vecKeys.end(); ++iteKey)
        {
            pNode->AppendCommand(REDIS_SLAVE, "GET %b", iteKey->c_str(), iteKey->length());
        }
        for (int j = 0; j < vecKeys.size(); ++j)
        {
            if (!pNode->GetReply(REDIS_SLAVE,&reply))
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::MultiGetString command exec error!\n");
                std::string nullStr;
                vecValues.push_back(nullStr);
                continue;
            }
            if (NULL == reply)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::MultiGetString command no response!\n");
                std::string nullStr;
                vecValues.push_back(nullStr);
                continue;
            }
            if (REDIS_REPLY_NIL == reply->type)
            {
                nResult = MC_ERR_NOT_EXISTS;
            }else if (REDIS_REPLY_ERROR == reply->type)
            {
                nResult = MC_ERR_SDK;
            }
            else if (REDIS_REPLY_STRING != reply->type)
            {
                if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
                {
                    //事务添加成功
                    nResult = MC_ERR_IN_QUEUE;
                }
                else
                {
                    PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetMultiString wrong return type:%d\n", reply->type);
                    nResult = MC_ERR_WRONG_TYPE;
                }
            }
            else
            {
                std::string str(reply->str, reply->len);
                vecValues.push_back(str);
            }
            g_pfreeReplyObject((void*)reply);
        }
    }
    else
    {
        //服务器是集群，需要根据key所属节点，单独获取每个值
        //为了和单节点效果相同，查询到的数据和key顺序一致，每个key单独查询
        CRedisNode *pNode = NULL;
        std::vector<std::string>::iterator iteKey = vecKeys.begin();
        for (; iteKey != vecKeys.end(); ++iteKey)
        {
            pNode = m_cRedisCon.GetNode(*iteKey);
            if (NULL == pNode)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "MultiGetString get connection failed!\n");
                return MC_ERR_SDK;
            }
            pNode->ExecCommand(REDIS_SLAVE, reply, "GET %b", iteKey->c_str(), iteKey->length());
            if (NULL == reply)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::MultiGetString command no response!\n");
                return MC_ERR_SDK;
            }
            if (REDIS_REPLY_STRING == reply->type)
            {
                std::string strValue(reply->str, reply->len);
                vecValues.push_back(strValue);
            }
            else
            {
                if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
                {
                    //事务添加成功
                    nResult = MC_ERR_IN_QUEUE;
                }
                else
                {
                    PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetMultiString wrong return type:%d\n", reply->type);
                    nResult = MC_ERR_WRONG_TYPE;
                }
            }
            g_pfreeReplyObject((void*)reply);
        }
    }
    return nResult;
}

s32 CMCRedis::GetInt( std::string &strKey, OUT s32 &nValue )
{
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    nValue = 0;
    std::string strValue;
    s32 nResult = GetString(strKey, strValue);
    if (MC_ERR_NO == nResult)
    {
        nValue = atoi(strValue.c_str());
    }else if (MC_ERR_NOT_EXISTS == nResult )
    {
        nValue = 0;
    }
    return nResult;
}

s32 CMCRedis::GetSets( std::string &strKey, OUT std::vector<std::string> &vecValue )
{
    //TODO smembers会一口气返回所有数据，后期将命令改为sscan可以返回指定数目的数据
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    vecValue.clear();
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "GetString get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "SMEMBERS %b", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetString command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_NIL == reply->type)
    {
        nResult = MC_ERR_NOT_EXISTS;
    }else if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_ARRAY != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetSets wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        for (size_t i = 0; i<reply->elements; ++i) 
        {
            if (REDIS_REPLY_ERROR == reply->element[i]->type)
            {
                continue;
            }
            std::string str(reply->element[i]->str, reply->element[i]->len);
            vecValue.push_back(str);
        }
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::GetList( std::string &strKey, OUT std::vector<std::string> &vecValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    vecValue.clear();
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "GetString get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "LRANGE %b 0 -1", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetString command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_NIL == reply->type)
    {
        nResult = MC_ERR_NOT_EXISTS;
    }else if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_ARRAY != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetList wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        for (size_t i = 0; i<reply->elements; ++i) 
        {
            if (REDIS_REPLY_ERROR == reply->element[i]->type)
            {
                continue;
            }
            std::string str(reply->element[i]->str, reply->element[i]->len);
            vecValue.push_back(str);
        }
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::GetHash( std::string &strKey, OUT std::map<std::string, std::string> &mapValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    mapValue.clear();
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "GetString get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "HGETALL %b", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetString command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_NIL == reply->type)
    {
        nResult = MC_ERR_NOT_EXISTS;
    }else if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_ARRAY != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetHash wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        std::string strKey;
        for (size_t i = 0; i<reply->elements; ++i) 
        {
            if (REDIS_REPLY_ERROR == reply->element[i]->type)
            {
                nResult = MC_ERR_SDK;
                mapValue.clear();
                break;
            }
            std::string str(reply->element[i]->str, reply->element[i]->len);
            if (0 == i % 2)
            {
                strKey = str;
            }
            else
            {
                mapValue.insert(std::make_pair(strKey, str));
            }
        }
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::Append(std::string &strKey, std::string &strValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "GetString get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, "APPEND %b %b", strKey.c_str(), strKey.length(), strValue.c_str(), strValue.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetString command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_INTEGER != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_NO;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Append wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::Incr( std::string &strKey, OUT s32 &nValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "GetString get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, "INCR %b", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetString command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_INTEGER != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Incr wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        nValue = reply->integer;
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::Decr( std::string &strKey, OUT s32 &nValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "GetString get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, "DECR %b", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::GetString command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_INTEGER != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Decr wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        nValue = reply->integer;
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::Set( std::string &strKey, std::string &strValue, eMCSetType tSet /*= MC_SET_DEFAULT*/, u32 nexptime /*= 0 */ )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "Set get connection failed!\n");
        return MC_ERR_SDK;
    }
    std::string strCommand("SET %b %b");
    if (nexptime != 0)
    {
        s8 szExpTime[12] = {0};
        strCommand.append(" EX ");
        sprintf(szExpTime, "%d", nexptime);
        strCommand.append(szExpTime);
    }
    if (MC_SET_DEFAULT != tSet)
    {
        if (MC_SET_EXIST == tSet)
        {
            strCommand.append(" XX");
        }
        else
        {
            strCommand.append(" NX");
        }
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, strCommand.c_str(), strKey.c_str(), strKey.length(), strValue.c_str(), strValue.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Set command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_STATUS != reply->type)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Set wrong return type:%d\n", reply->type);
        nResult = MC_ERR_SDK;
    }
    else
    {
        //success return OK
        if (0 == strcasecmp("OK", reply->str) || 0 == strcasecmp("QUEUED", reply->str))
        {
            nResult = MC_ERR_NO;
        }
        else
        {
            nResult = MC_ERR_SDK;
        }
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::SetExpireTime( std::string &strKey, u32 nexptime /*= 0*/ )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "SetExpireTime get connection failed!\n");
        return MC_ERR_SDK;
    }
    std::string strCommand("EXPIRE %b ");
    if (nexptime != 0)
    {
        s8 szExpTime[12] = {0};
        sprintf(szExpTime, "%d", nexptime);
        strCommand.append(szExpTime);;
    }
    else
    {
        //取消超时设置，需要使用PERSIST key命令
        strCommand.assign("PERSIST %b ");
    }
    
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, strCommand.c_str(), strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::SetExpireTime command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_INTEGER != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_NO;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::SetExpireTime wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        if (0 == reply->integer)
        {
            //设置失败或key已超时
            nResult = MC_ERR_NOT_EXISTS;
        }
        else
        {
            nResult = MC_ERR_NO;
        }
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::Exists(std::string &strKey)
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "Set get connection failed!\n");
        return MC_ERR_SDK;
    }
    std::string strCommand("EXISTS %b");
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, strCommand.c_str(), strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Set command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else
    {
        if (1 == reply->integer)
        {//数据存在
            nResult = MC_ERR_NO;
        }
        else
        {//数据不存在
            nResult = MC_ERR_NOT_EXISTS;
        }
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::ListGet( std::string &strKey, s32 nIndex, OUT std::string &strValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "ListGet get connection failed!\n");
        return MC_ERR_SDK;
    }
    strValue.clear();
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "LINDEX %b %d", strKey.c_str(), strKey.length(), nIndex);
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListGet command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_STRING != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListGet wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        strValue.assign(reply->str, reply->len);
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::ListSet( std::string &strKey, s32 nIndex, std::string &strValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "ListSet get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, "LSET %b %d %b", strKey.c_str(), strKey.length(), nIndex, strValue.c_str(), strValue.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListSet command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_STATUS != reply->type)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListSet wrong return type:%d\n", reply->type);
        nResult = MC_ERR_WRONG_TYPE;
    }
    else
    {
        //success return OK
        if (0 == strcasecmp("OK", reply->str) || 0 == strcasecmp("QUEUED", reply->str))
        {
            nResult = MC_ERR_NO;
        }
        else
        {
            nResult = MC_ERR_SDK;
        }
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::ListLen( std::string &strKey, OUT s32 &nValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "ListLen get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "LLEN %b", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListLen command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_INTEGER != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListLen wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        nValue = reply->integer;
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::ListLPush( std::string &strKey, std::vector<std::string> &vecValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length() || 0 == vecValue.size())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "ListLPush get connection failed!\n");
        return MC_ERR_SDK;
    }
    std::vector<std::string>::iterator iteVal = vecValue.begin();
    redisReply *reply = NULL;
    for (; iteVal != vecValue.end(); ++iteVal)
    {
        pNode->ExecCommand(REDIS_MASTER, reply, "LPUSH %b %b", strKey.c_str(), strKey.length(), iteVal->c_str(), iteVal->length());
        if (NULL == reply)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListLPush command no response!\n");
            return MC_ERR_SDK;
        }
        if (REDIS_REPLY_ERROR == reply->type)
        {
            nResult = MC_ERR_SDK;
        }
        else if (REDIS_REPLY_INTEGER != reply->type)
        {
            if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
            {
                //事务添加成功
                nResult = MC_ERR_NO;
            }
            else
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListLPush wrong return type:%d\n", reply->type);
                nResult = MC_ERR_WRONG_TYPE;
            }
        }
        else
        {

        }
        g_pfreeReplyObject((void*)reply);
    }
    return nResult;
}

s32 CMCRedis::ListLPop( std::string &strKey, OUT std::string &strValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    strValue.clear();
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "ListLPop get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, "LPOP %b", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListLPop command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_STRING != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListLPop wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        strValue.assign(reply->str, reply->len);
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::ListRPush( std::string &strKey, std::vector<std::string> &vecValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "ListRPush get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    std::vector<std::string>::iterator iteVal = vecValue.begin();
    for (; iteVal != vecValue.end(); ++iteVal)
    {
        pNode->ExecCommand(REDIS_MASTER, reply, "RPUSH %b %b", strKey.c_str(), strKey.length(), iteVal->c_str(), iteVal->length());
        if (NULL == reply)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListRPush command no response!\n");
            return MC_ERR_SDK;
        }
        if (REDIS_REPLY_ERROR == reply->type)
        {
            nResult = MC_ERR_SDK;
        }
        else if (REDIS_REPLY_INTEGER != reply->type)
        {
            if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
            {
                //事务添加成功
                nResult = MC_ERR_NO;
            }
            else
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListRPush wrong return type:%d\n", reply->type);
                nResult = MC_ERR_WRONG_TYPE;
            }
        }
        else
        {

        }
        g_pfreeReplyObject((void*)reply);
    }
    return nResult;
}

s32 CMCRedis::ListRPop( std::string &strKey, OUT std::string &strValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    strValue.clear();
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "ListRPop get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, "RPOP %b", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListRPop command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_STRING != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::ListRPop wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        strValue.assign(reply->str, reply->len);
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::SetsAdd( std::string &strKey, std::vector<std::string> &vecValue, u32 nexptime /*= 0 */ )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "SetsAdd get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    std::vector<std::string>::iterator iteVal = vecValue.begin();
    for (; iteVal != vecValue.end(); ++iteVal)
    {
        pNode->ExecCommand(REDIS_MASTER, reply, "SADD %b %b", strKey.c_str(), strKey.length(), iteVal->c_str(), iteVal->length());
        if (NULL == reply)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::SetsAdd command no response!\n");
            return MC_ERR_SDK;
        }
        if (REDIS_REPLY_ERROR == reply->type)
        {
            nResult = MC_ERR_SDK;
        }
        else if (REDIS_REPLY_INTEGER != reply->type)
        {
            if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
            {
                //事务添加成功
                nResult = MC_ERR_NO;
            }
            else
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::SetsAdd wrong return type:%d\n", reply->type);
                nResult = MC_ERR_WRONG_TYPE;
            }
        }
        else
        {

        }
        g_pfreeReplyObject((void*)reply);
    }
    return nResult;
}

s32 CMCRedis::SetsRem( std::string &strKey, std::vector<std::string> &vecValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "SetsRem get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    std::vector<std::string>::iterator iteVal = vecValue.begin();
    for (; iteVal != vecValue.end(); ++iteVal)
    {
        pNode->ExecCommand(REDIS_MASTER, reply, "SREM %b %b", strKey.c_str(),strKey.length(), 
            iteVal->c_str(), iteVal->length());
        if (NULL == reply)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::SetsRem command no response!\n");
            return MC_ERR_SDK;
        }
        if (REDIS_REPLY_ERROR == reply->type)
        {
            nResult = MC_ERR_SDK;
        }
        else if (REDIS_REPLY_INTEGER != reply->type)
        {
            if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
            {
                //事务添加成功
                nResult = MC_ERR_NO;
            }
            else
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::SetsRem wrong return type:%d\n", reply->type);
                nResult = MC_ERR_WRONG_TYPE;
            }
        }
        else
        {
        }
        g_pfreeReplyObject((void*)reply);
    }
    
    
    return nResult;
}

s32 CMCRedis::SetsExists( std::string &strKey, std::string &strValue, OUT bool &bExist )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "SetsExists get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "SISMEMBER %b %b", strKey.c_str(), strKey.length(), strValue.c_str(), strValue.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::SetsExists command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_INTEGER != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::SetsExists wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        bExist = reply->integer;
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::HashGet( std::string &strKey, std::string &strField, OUT std::string &strValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "HashGet get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "HGET %b %b", strKey.c_str(), strKey.length(), strField.c_str(), strField.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashGet command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_NIL == reply->type)
    {//key不存在，值置空
        strValue.clear();
    }
    else if (REDIS_REPLY_STRING != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashGet wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        strValue.assign(reply->str, reply->len);
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::HashSet( std::string &strKey, std::string &strField, std::string &strValue )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "HashSet get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, "HSET %b %b %b", strKey.c_str(), strKey.length(), 
        strField.c_str(), strField.length(), strValue.c_str(), strValue.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashSet command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_INTEGER != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_NO;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashSet wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        //1 means new failed ,0 means existed field
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::HashDel( std::string &strKey, std::string &strField )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "HashDel get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_MASTER, reply, "HDEL %b %b", strKey.c_str(), strKey.length(), strField.c_str(), strField.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashDel command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_INTEGER != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_NO;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashDel wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        //返回成功删除的field数目
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::HashGetKeys( std::string &strKey, OUT std::vector<std::string> &vecFields )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "HashGetKeys get connection failed!\n");
        return MC_ERR_SDK;
    }
    vecFields.clear();
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "HKEYS %b", strKey.c_str(), strKey.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashGetKeys command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_ARRAY != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        for (size_t i = 0; i<reply->elements; ++i) 
        {
            if (REDIS_REPLY_NIL == reply->element[i]->type || REDIS_REPLY_ERROR == reply->element[i]->type)
            {
                continue;
            }
            std::string str(reply->element[i]->str, reply->element[i]->len);
            vecFields.push_back(str);
        }
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::HashExists( std::string &strKey, std::string &strField, OUT bool &bExist )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "HashExists get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    pNode->ExecCommand(REDIS_SLAVE, reply, "HEXISTS %b %b", strKey.c_str(), strKey.length(), strField.c_str(), strField.length());
    if (NULL == reply)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashExists command no response!\n");
        return MC_ERR_SDK;
    }
    if (REDIS_REPLY_ERROR == reply->type)
    {
        nResult = MC_ERR_SDK;
    }
    else if (REDIS_REPLY_INTEGER != reply->type)
    {
        if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
        {
            //事务添加成功
            nResult = MC_ERR_IN_QUEUE;
        }
        else
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashExists wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
    }
    else
    {
        bExist = reply->integer;
    }
    g_pfreeReplyObject((void*)reply);
    return nResult;
}

s32 CMCRedis::HashSetAll( std::string &strKey, std::map<std::string,std::string> &hashValue, u32 nexptime /*= 0 */ )
{
    s32 nResult = MC_ERR_NO;
    if (0 == strKey.length())
    {
        return MC_ERR_INVALID_PARAM;
    }
    CRedisNode *pNode = m_cRedisCon.GetNode(strKey);
    if (NULL == pNode)
    {
        PrintLog(MC_LOG_ERR_LEVEL, "HashSetAll get connection failed!\n");
        return MC_ERR_SDK;
    }
    redisReply *reply = NULL;
    std::map<std::string,std::string>::iterator iteHash = hashValue.begin();
    for (; iteHash != hashValue.end(); ++iteHash)
    {
        if (0 == iteHash->first.length() /*|| 0 == iteHash->second.length()*/)
        {
            continue;
        }
        pNode->ExecCommand(REDIS_MASTER, reply, "HSET %b %b %b", strKey.c_str(), strKey.length(),
            iteHash->first.c_str(), iteHash->first.length(), 
            iteHash->second.c_str(), iteHash->second.length());
        if (NULL == reply)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashSetAll command no response!\n");
            return MC_ERR_SDK;
        }
        if (REDIS_REPLY_ERROR == reply->type)
        {
            nResult = MC_ERR_SDK;
        }
        else if (REDIS_REPLY_INTEGER != reply->type)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::HashSetAll wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
        else
        {
            //success return OK
//             if (0 == strcasecmp("OK", reply->str) || 0 == strcasecmp("QUEUED", reply->str))
//             {
//                 nResult = MC_ERR_NO;
//             }
//             else
//             {
//                 nResult = MC_ERR_SDK;
//             }
        }
        g_pfreeReplyObject((void*)reply);
        reply = NULL;
    }
    
    
    return nResult;
}

s32 CMCRedis::DelKeys( std::vector<std::string> &vecKeys )
{
    s32 nResult = MC_ERR_NO;
    if (0 == vecKeys.size())
    {
        return MC_ERR_INVALID_PARAM;
    }
    redisReply *reply = NULL;
    //经测试，集群使用del删除多条需要数据在一个slots内才行，否则只能单条单条删除
    if (!m_cRedisCon.IsCluster())
    {
        //非集群，直接全部删除
        CRedisNode *pNode = m_cRedisCon.GetNode(vecKeys[0]);
        if (NULL == pNode)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "DelKeys get connection failed!\n");
            return MC_ERR_SDK;
        }
        std::vector<std::string>::iterator iteKey = vecKeys.begin();
        for (; iteKey != vecKeys.end(); ++iteKey)
        {
            pNode->ExecCommand(REDIS_MASTER, reply, "DEL %b", iteKey->c_str(), iteKey->length());
            if (NULL == reply)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::DelKeys command no response!\n");
                return MC_ERR_SDK;
            }
            if (REDIS_REPLY_ERROR == reply->type)
            {
                nResult = MC_ERR_SDK;
            }
            else if (REDIS_REPLY_INTEGER != reply->type)
            {
                if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
                {
                    //事务添加成功
                    nResult = MC_ERR_NO;
                }
                else
                {
                    PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::DelKeys wrong return type:%d\n", reply->type);
                    nResult = MC_ERR_WRONG_TYPE;
                }
            }
            else
            {
                //返回被删除的key的数目
            }
            g_pfreeReplyObject((void*)reply);
            reply = NULL;
        }
        
    }
    else
    {
        //集群，循环删除每一条
        std::vector<std::string>::iterator iteKey = vecKeys.begin();
        for (; iteKey != vecKeys.end(); ++iteKey)
        {
            CRedisNode *pNode = m_cRedisCon.GetNode(*iteKey);
            if (NULL == pNode)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "DelKeys get connection failed!\n");
                return MC_ERR_SDK;
            }
            pNode->ExecCommand(REDIS_MASTER, reply, "DEL %b", iteKey->c_str(), iteKey->length());
            if (NULL == reply)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::DelKeys command no response!\n");
                continue;
            }
            if (REDIS_REPLY_ERROR == reply->type)
            {
                nResult = MC_ERR_SDK;
            }
            else if (REDIS_REPLY_INTEGER != reply->type)
            {
                if (REDIS_REPLY_STATUS == reply->type && 0 == strcasecmp("QUEUED", reply->str))
                {
                    //事务添加成功
                    nResult = MC_ERR_NO;
                }
                else
                {
                    PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::DelKeys wrong return type:%d\n", reply->type);
                    nResult = MC_ERR_WRONG_TYPE;
                }
            }
            else
            {
                //返回被删除的key的数目
            }
            g_pfreeReplyObject((void*)reply);
        }
    }

    //del指令不会反悔失败，只会跳过不存在的key
    return MC_ERR_NO;
}
//事务处理：先watch，再用multi开始事务，exec提交事务，discard和unwatch取消
s32 CMCRedis::Watch( std::vector<std::string> &vecKeys )
{
    s32 nResult = MC_ERR_NO;
    if (0 == vecKeys.size())
    {
        return MC_ERR_INVALID_PARAM;
    }
    redisReply *reply = NULL;
    if (!m_cRedisCon.IsCluster())
    {
        //非集群
        CRedisNode *pNode = m_cRedisCon.GetNode(vecKeys[0]);
        if (NULL == pNode)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "Watch get connection failed!\n");
            return MC_ERR_SDK;
        }
        std::vector<std::string>::iterator iteKey = vecKeys.begin();
        for (; iteKey != vecKeys.end(); ++iteKey)
        {
            pNode->ExecCommand(REDIS_MASTER, reply, "WATCH %b", iteKey->c_str(), iteKey->length());
            if (NULL == reply)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Watch command no response!\n");
                return MC_ERR_SDK;
            }
            if (REDIS_REPLY_ERROR == reply->type)
            {
                nResult = MC_ERR_SDK;
            }
            else if (REDIS_REPLY_STATUS != reply->type)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Watch wrong return type:%d\n", reply->type);
                nResult = MC_ERR_WRONG_TYPE;
            }
            else
            {
                //success return OK
                if (0 != strcasecmp("OK", reply->str))
                {
                    nResult = MC_ERR_SDK;
                }
            }
            g_pfreeReplyObject((void*)reply);
        }
        
    }
    else
    {
        //集群
        std::vector<std::string>::iterator iteKey = vecKeys.begin();
        for (; iteKey != vecKeys.end(); ++iteKey)
        {
            CRedisNode *pNode = m_cRedisCon.GetNode(*iteKey);
            if (NULL == pNode)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "Watch get connection failed!\n");
                return MC_ERR_SDK;
            }
            pNode->ExecCommand(REDIS_MASTER, reply, "WATCH %b", iteKey->c_str(), iteKey->length());
            if (NULL == reply)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Watch command no response!\n");
                continue;
            }
            if (REDIS_REPLY_ERROR == reply->type)
            {
                nResult = MC_ERR_SDK;
            }
            else if (REDIS_REPLY_STATUS != reply->type)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Watch wrong return type:%d\n", reply->type);
                nResult = MC_ERR_WRONG_TYPE;
            }
            else
            {
                //success return OK
                if (0 != strcasecmp("OK", reply->str))
                {
                    nResult = MC_ERR_SDK;
                }
            }
            g_pfreeReplyObject((void*)reply);
        }
    }
    return nResult;
}

s32 CMCRedis::Multi()
{
    s32 nResult = MC_ERR_NO;
    redisReply *reply = NULL;
    if (!m_cRedisCon.IsCluster())
    {
        //非集群
        CRedisNode *pNode = m_cRedisCon.GetNode(0);
        if (NULL == pNode)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "Multi get connection failed!\n");
            return MC_ERR_SDK;
        }
        pNode->ExecCommand(REDIS_MASTER, reply, "MULTI");
        if (NULL == reply)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Multi command no response!\n");
            return MC_ERR_SDK;
        }
        if (REDIS_REPLY_ERROR == reply->type)
        {
            nResult = MC_ERR_SDK;
        }
        else if (REDIS_REPLY_STATUS != reply->type)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Multi wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
        else
        {
            //success return OK
            if (0 != strcasecmp("OK", reply->str))
            {
                nResult = MC_ERR_SDK;
            }
        }
        g_pfreeReplyObject((void*)reply);
    }
    else
    {
        //集群
        for (int i = 0; i < m_cRedisCon.GetNodeNum(); ++i)
        {
            CRedisNode *pNode = m_cRedisCon.GetNode(i);
            if (NULL == pNode)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "Multi get connection failed!\n");
                return MC_ERR_SDK;
            }
            pNode->ExecCommand(REDIS_MASTER, reply, "MULTI");
            if (NULL == reply)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Multi command no response!\n");
                continue;
            }
            if (REDIS_REPLY_ERROR == reply->type)
            {
                nResult = MC_ERR_SDK;
            }
            else if (REDIS_REPLY_STATUS != reply->type)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Multi wrong return type:%d\n", reply->type);
                nResult = MC_ERR_WRONG_TYPE;
            }
            else
            {
                //success return OK
                if (0 != strcasecmp("OK", reply->str))
                {
                    nResult = MC_ERR_SDK;
                }
            }

            g_pfreeReplyObject((void*)reply);
        }
    }
    return nResult;
}

s32 CMCRedis::Exec()
{
    s32 nResult = MC_ERR_NO;
    redisReply *reply = NULL;
    if (!m_cRedisCon.IsCluster())
    {
        //非集群
        CRedisNode *pNode = m_cRedisCon.GetNode(0);
        if (NULL == pNode)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "Exec get connection failed!\n");
            return MC_ERR_SDK;
        }
        pNode->ExecCommand(REDIS_MASTER, reply, "EXEC");
        if (NULL == reply)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Exec command no response!\n");
            return MC_ERR_SDK;
        }
        if (REDIS_REPLY_ERROR == reply->type)
        {
            nResult = MC_ERR_SDK;
        }
        else if (REDIS_REPLY_ARRAY != reply->type)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Exec wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
        else
        {
        }
        g_pfreeReplyObject((void*)reply);
    }
    else
    {
        //集群
        for (int i = 0; i < m_cRedisCon.GetNodeNum(); ++i)
        {
            CRedisNode *pNode = m_cRedisCon.GetNode(i);
            if (NULL == pNode)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "Exec get connection failed!\n");
                return MC_ERR_SDK;
            }
            pNode->ExecCommand(REDIS_MASTER, reply, "EXEC");
            if (NULL == reply)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Exec command no response!\n");
                continue;
            }
            if (REDIS_REPLY_ERROR == reply->type)
            {
                nResult = MC_ERR_SDK;
            }
            else if (REDIS_REPLY_ARRAY != reply->type)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Exec wrong return type:%d\n", reply->type);
                nResult = MC_ERR_WRONG_TYPE;
            }
            else
            {
                
            }

            g_pfreeReplyObject((void*)reply);
        }
    }
    return nResult;
}

s32 CMCRedis::Discard()
{
    s32 nResult = MC_ERR_NO;
    redisReply *reply = NULL;
    if (!m_cRedisCon.IsCluster())
    {
        //非集群
        CRedisNode *pNode = m_cRedisCon.GetNode(0);
        if (NULL == pNode)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "Discard get connection failed!\n");
            return MC_ERR_SDK;
        }
        pNode->ExecCommand(REDIS_MASTER, reply, "DISCARD");
        if (NULL == reply)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Discard command no response!\n");
            return MC_ERR_SDK;
        }
        if (REDIS_REPLY_ERROR == reply->type)
        {
            nResult = MC_ERR_SDK;
        }
        else if (REDIS_REPLY_STATUS != reply->type)
        {
            PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Discard wrong return type:%d\n", reply->type);
            nResult = MC_ERR_WRONG_TYPE;
        }
        else
        {
            //success return OK
            if (0 != strcasecmp("OK", reply->str))
            {
                nResult = MC_ERR_SDK;
            }
        }
        g_pfreeReplyObject((void*)reply);
    }
    else
    {
        //集群
        for (int i = 0; i < m_cRedisCon.GetNodeNum(); ++i)
        {
            CRedisNode *pNode = m_cRedisCon.GetNode(i);
            if (NULL == pNode)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "Discard get connection failed!\n");
                return MC_ERR_SDK;
            }
            pNode->ExecCommand(REDIS_MASTER, reply, "DISCARD");
            if (NULL == reply)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Discard command no response!\n");
                continue;
            }
            if (REDIS_REPLY_ERROR == reply->type)
            {
                nResult = MC_ERR_SDK;
            }
            else if (REDIS_REPLY_STATUS != reply->type)
            {
                PrintLog(MC_LOG_ERR_LEVEL, "CMCRedis::Discard wrong return type:%d\n", reply->type);
                nResult = MC_ERR_WRONG_TYPE;
            }
            else
            {
                //success return OK
                if (0 != strcasecmp("OK", reply->str))
                {
                    nResult = MC_ERR_SDK;
                }
            }

            g_pfreeReplyObject((void*)reply);
        }
    }
    return nResult;
}

void CMCRedis::PrintLog( eMCLoglev eLevel, const s8* const szFormat, ... )
{
    if (NULL == m_pfLogCb)
    {
        return;
    }
    int nBufSize = MAX_LOG_LENGTH;
    s8 *buf = new s8[nBufSize];
    if (NULL == buf)
    {
        return;
    }
    memset(buf, 0, nBufSize);
    va_list pvList; 
    va_start( pvList, szFormat); 
    s32 actLen = vsnprintf(buf, (nBufSize-1), szFormat, pvList); 
    if (actLen <= 0) 
    { 
        (buf)[(nBufSize-1)] = 0; 
        (buf)[(nBufSize-2)] = '\n'; 
#ifdef WIN32        //TODO linux没有unsic++的isleadbyte函数
        if (0 != isleadbyte((buf)[(nBufSize-3)])) 
        { 
            (buf)[(nBufSize-3)] = '\n'; 
            (buf)[(nBufSize-2)] = 0; 
        } 
#endif
    } 
    va_end( pvList ); 
    m_pfLogCb(eLevel, buf, strlen(buf), m_pUser);
    delete[] buf;
}

bool CMCRedis::IsRedisConnected()
{
    return m_cRedisCon.IsConnected();
}

BOOL32 CMCRedis::KeepAlive()
{
    return m_cRedisCon.KeepAlive();
}

void CMCRedis::OnDisconnect()
{
    if (NULL != m_pDisconCb)
    {
        m_pDisconCb(m_pUser);
    }
}






