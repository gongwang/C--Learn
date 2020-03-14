#include "mqinterface.h"
#include <stdio.h>
#include <string>
#include "mqkafkaserver.h"
#include "mqkafkasender.h"
#include "mqkafkareceiver.h"
#include "mqkafkamanager.h"
#include <stdarg.h>

CMqKafkaManager* CMqKafkaManager::s_pCManagerInst = NULL;
CMqKafkaManager* GetManager()
{
    return CMqKafkaManager::instance();
}
//producer拉取（poll）消息的线程
void* PoolMsgThread(void* args)
{
    CMqKafkaManager *pcManager = (CMqKafkaManager*)args;
    if (NULL == pcManager)
    {
        return NULL;
    }
    while (TRUE != pcManager->GetStopFlag()) 
    {
        //将耗时的大锁改为轮询时的小锁，通过轮询对象是否一致来区分是否已轮询
        SmartPtr<CMqKafkaServer> pServer(NULL); 
        bool bFindFlag = true; 
        do{
            {//先走一遍轮询，查询上次遍历到的节点位置
                CThreadAutoLock cLock(pcManager->m_clmLock);
                std::vector<SmartPtr<CMqKafkaServer> >::iterator iteServer = pcManager->m_vecpServer.begin();
                while(iteServer != pcManager->m_vecpServer.end())
                {
                    if (NULL == pServer || bFindFlag)
                    {//标志位为true，代表上一个是已轮询的server对象，赋值并跳出当前循环
                        pServer = *iteServer;
                        bFindFlag = false;
                        break;
                    }
                    if (pServer == *iteServer)
                    {
                        bFindFlag = true;
                    }
                    ++iteServer;
                }
            }
            if (bFindFlag)
            {//没有其他节点了，退出循环
                break;
            }
            if (SERVER_ONLINE == pServer->GetState())
            {
                pServer->PollSenderMsg();
            }
        }while(1);
        
#ifdef _MSC_VER
        Sleep(1000);
#else
        sleep(1);
#endif
    }
    return 0;
}
CMqKafkaManager::CMqKafkaManager()
     :m_pLogCB(NULL)
     ,m_pDownCB(NULL)
     ,m_bInit(FALSE)
     ,m_pUsr(NULL)
     ,m_bStopFlag(FALSE)
{
    m_cIndexGenerater.Init(0xffff);
}

CMqKafkaManager::~CMqKafkaManager()
{
    m_bStopFlag = TRUE;
    //wait sender poll msg thread exit
    pthread_join(m_tSenderPoolThread,NULL);
}

CMqKafkaManager *CMqKafkaManager::instance()
{
    if (NULL == s_pCManagerInst)
    {
        s_pCManagerInst = new CMqKafkaManager;

        if (NULL == s_pCManagerInst)
        {
            return NULL;
        }
    }

    return s_pCManagerInst;
}

void CMqKafkaManager::destroy()
{
    delete s_pCManagerInst;
    s_pCManagerInst = NULL;
}

s32 CMqKafkaManager::Init(MQLogCallBack pLogCallBack, MQServerDownCallBack pServerDownCallBack, void* pUser)
{
    if (!IRdKafka::LoadDll())
    {
        PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaManager::Init:load rdkafka failed!");
        return MQ_ERR_UNKNOWN;
    }
    m_pLogCB = pLogCallBack;
    m_pDownCB = pServerDownCallBack;
    m_pUsr = pUser;
    //create a thread to poll producer message per second
    m_bStopFlag = FALSE;
    int ret = pthread_create(&m_tSenderPoolThread, NULL, PoolMsgThread, this);
    if (ret != 0)
    {
        PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaManager::Init:create SenderPoolThread failed: error_code=%d", ret );
        return MQ_ERR_UNKNOWN;
    }
    m_bInit = TRUE;
    return MQ_ERR_NO;
}
MQHANDLE CMqKafkaManager::LoginKafkaServer(PTMQServerInfo ptServerArr, u32 nServerAddrNum, LPCSTR szUsername , LPCSTR szPassword, s32 nTimeout)
{
    MQHANDLE nNewHandle = 0;
    {//只在创建索引和加入集合时加锁，耗时的登陆操作不加锁
        CThreadAutoLock cLock(m_clmLock);
        nNewHandle = CreateServerId();
        if (0 == nNewHandle)
        {
            //GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateSender: ERR, create topichandle failed \n");
            return 0;
        }
    }
    CMqKafkaServer *pcServer = new CMqKafkaServer();
    BOOL32 bRet = pcServer->LoginServer(nNewHandle, ptServerArr, nServerAddrNum, szUsername, szPassword, nTimeout);
    if (bRet != TRUE)
    {
        PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateReceiver: failed!\n");
        DeleteServerId(nNewHandle);
        return 0;
    }
    {//只在创建索引和加入集合时加锁，耗时的登陆操作不加锁
        CThreadAutoLock cLock(m_clmLock);
        m_vecpServer.push_back(SmartPtr<CMqKafkaServer>(pcServer));
    }
    return nNewHandle;
}
SmartPtr<CMqKafkaServer> CMqKafkaManager::FindServer(MQHANDLE hServerHandle)
{
    CThreadAutoLock cLock(m_clmLock);
    std::vector<SmartPtr<CMqKafkaServer> >::iterator iteServer = m_vecpServer.begin();
    while(iteServer != m_vecpServer.end())
    {
        if ((*iteServer)->GetServerId() == hServerHandle)
        {
            break;
        }
        ++iteServer;
    }
    if (m_vecpServer.end() == iteServer)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaManager::FindServer: ERR,can't find handle:%d\n", hServerHandle);
        return SmartPtr<CMqKafkaServer>(NULL);
    }
    return *iteServer;
}
//销毁连接到指定服务器的连接池
s32 CMqKafkaManager::LogoutKafkaServer(MQHANDLE hServerHandle)
{
    SmartPtr<CMqKafkaServer> pcServer(NULL);
    {//遍历时加锁，耗时的注销操作不加锁
        CThreadAutoLock cLock(m_clmLock);
        m_vecpServer.push_back(SmartPtr<CMqKafkaServer>(pcServer));
        //不使用FindServer是因为要使用迭代器删除vector中的指针
        std::vector<SmartPtr<CMqKafkaServer> >::iterator iteServer = m_vecpServer.begin();
        while(iteServer != m_vecpServer.end())
        {
            if ((*iteServer)->GetServerId() == hServerHandle)
            {
                pcServer = *iteServer;
                break;
            }
            ++iteServer;
        }
        if (m_vecpServer.end() == iteServer)
        {
            GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::DeleteSender: ERR,can't find handle:%d\n", hServerHandle);
            return MQ_ERR_HANDLE_NOTFOUND;
        }
        m_vecpServer.erase(iteServer);
    }
    pcServer->Logout();
    //delete pcServer;
    DeleteServerId(hServerHandle);
    return MQ_ERR_NO;
}
//获取目标状态
eServerState CMqKafkaManager::GetMQState(MQHANDLE hServerHandle)
{
    SmartPtr<CMqKafkaServer> pcServer = FindServer(hServerHandle);
    if (NULL == pcServer)
    {
        return SERVER_NOTFOUND;
    }
    return pcServer->GetState();
}
//触发断链事件
void CMqKafkaManager::OnServerDown(SmartPtr<CMqKafkaServer> pcServer)
{
    if (NULL == pcServer)
    {
        return;
    }
    if(NULL != m_pDownCB)
    {
        m_pDownCB(pcServer->GetServerId(), m_pUsr);
    }
}
//获取目标所有topic
s32 CMqKafkaManager::GetAllTopics(MQHANDLE hServerHandle, s32 nTimeout, PTMQTopicInfo pTopicArr, u32 &nTopicCnt)
{
    SmartPtr<CMqKafkaServer> pcServer = FindServer(hServerHandle);
    if (NULL == pcServer)
    {
        return MQ_ERR_HANDLE_NOTFOUND;
    }
    const rd_kafka_metadata_t* prkmetadata = NULL;
    if(FALSE == pcServer->GetMetadata(prkmetadata))
    {
        PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaManager::GetAllTopics failed!");
        return MQ_ERR_SERVER_DOWN;
    }
    nTopicCnt = prkmetadata->topic_cnt;
    if (NULL == pTopicArr)
    {
        return MQ_ERR_NO;
    }
    for (int i = 0; i < prkmetadata->topic_cnt; ++i)
    {
        const struct rd_kafka_metadata_topic *t = &prkmetadata->topics[i];
        PTMQTopicInfo pTopicInfo = pTopicArr + i;
        if (0 != pTopicArr->dwSize)
        {
            pTopicInfo = (PTMQTopicInfo)(((s8*)pTopicArr) + (pTopicArr->dwSize * i));
        }
        strncpy(pTopicInfo->szTopicName, t->topic, sizeof(pTopicInfo->szTopicName));
    }
    return MQ_ERR_NO;
}

void CMqKafkaManager::PrintLog(eMQLoglev eLev, const s8* const szFormat, ... )
{
    if (NULL == m_pLogCB)
    {
        return;
    }
    int nBufSize = MAX_LOG_LEN;
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
    m_pLogCB(eLev, buf, strlen(buf), m_pUsr);
    delete[] buf;
}

void CMqKafkaManager::PrintServerData(MQHANDLE hServerHandle)
{
    PrintLog(MQ_LOG_NORMAL_LEVEL, "librdkafka version %s (0x%08x)\n",
        IRdKafka::_version_str(), IRdKafka::_version());
    if (NULL == hServerHandle)
    {//打印所有
        std::vector<SmartPtr<CMqKafkaServer> > vecAllServer = m_vecpServer;
        std::vector<SmartPtr<CMqKafkaServer> >::iterator iteServer = vecAllServer.begin();
        while(iteServer != vecAllServer.end())
        {
            PrintServerData((*iteServer)->GetServerId());
            ++iteServer;
        }
        return;
    }
    //用PrintLog打印出制定服务器数据信息
    SmartPtr<CMqKafkaServer> pcServer = FindServer(hServerHandle);
    if (NULL == pcServer)
    {
        PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaManager::GetAllTopics failed!err:MQ_ERR_HANDLE_NOTFOUND");
        return ;//MQ_ERR_HANDLE_NOTFOUND
    }
    const rd_kafka_metadata_t* prkmetadata = NULL;
    if(FALSE == pcServer->GetMetadata(prkmetadata))
    {
        PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaManager::GetAllTopics failed! err:MQ_ERR_SERVER_DOWN");
        return ;
    }
    /* Iterate brokers */
    PrintLog(MQ_LOG_NORMAL_LEVEL, "CMqKafkaManager::PrintServerData handle:%d,brokers num:%d \n",
        hServerHandle, prkmetadata->broker_cnt);
    for (int i = 0 ; i < prkmetadata->broker_cnt ; i++)
    {
        PrintLog(MQ_LOG_NORMAL_LEVEL, "\tbroker:%d at %s:%d\n",
            prkmetadata->brokers[i].id, prkmetadata->brokers[i].host, prkmetadata->brokers[i].port);
    }
    /* Iterate topics */
    PrintLog(MQ_LOG_NORMAL_LEVEL, "\ttopics num:%d\n",
        prkmetadata->topic_cnt);
    for (int i = 0 ; i < prkmetadata->topic_cnt ; i++)
    {
        const struct rd_kafka_metadata_topic *t = &prkmetadata->topics[i];
        PrintLog(MQ_LOG_NORMAL_LEVEL, "\ttopic:%s with %d partitions:\n",
            t->topic, t->partition_cnt);
        if (t->err) 
        {
            PrintLog(MQ_LOG_NORMAL_LEVEL, "\t\terr:%s\n",
                IRdKafka::_err2str(t->err));
        }
        for (int j = 0 ; j < t->partition_cnt ; j++)
        {
            const struct rd_kafka_metadata_partition *p = &t->partitions[j];
            PrintLog(MQ_LOG_NORMAL_LEVEL, "\t\tpartition:%d, leader:%d,replicas:",
                p->id, p->leader);
            /* Iterate partition's replicas */
            for (int k = 0 ; k < p->replica_cnt ; k++)
                PrintLog(MQ_LOG_NORMAL_LEVEL, (k > 0 ? ",%d":"%d" ),
                p->replicas[k]);
            /* Iterate partition's ISRs */
            PrintLog(MQ_LOG_NORMAL_LEVEL, ", isrs: ",
                p->id, p->leader);
            for (int k = 0 ; k < p->isr_cnt ; k++)
                PrintLog(MQ_LOG_NORMAL_LEVEL, (k > 0 ? ",%d":"%d" ),
                p->isrs[k]);
            if (p->err)
                PrintLog(MQ_LOG_NORMAL_LEVEL, "err:%s\n",
                IRdKafka::_err2str(p->err));
            else
                PrintLog(MQ_LOG_NORMAL_LEVEL, "\n");
        }
    }
}

MQHANDLE CMqKafkaManager::CreateServerId()
{
    //手动生成serverid
    s32 nServerId = m_cIndexGenerater.GetIndexCount() + 1;
    if ((nServerId & 0xffff0000) > 0)
    {
        //当前创建的server数目大于65535，不允许继续创建
        PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaManager::CreateServerId: failed,server too much(>65535)\n");
        return 0;
    }
    nServerId = m_cIndexGenerater.GenIndex();
    if (0 == nServerId)
    {
        PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaManager::CreateServerId: failed,genIdx = 0\n");
        return 0;
    }
    PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaManager::CreateServerId:new serverid is %d\n", 
        nServerId);
    return (MQHANDLE)nServerId;
}

void CMqKafkaManager::DeleteServerId(MQHANDLE nServerId)
{
    m_cIndexGenerater.RevokeIndex(nServerId);
}