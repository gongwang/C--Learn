
#include "mqinterface.h"
#include <stdio.h>
#include <string>
#include "mqkafkamanager.h"
#include "mqkafkasender.h"
#include "mqkafkareceiver.h"
#include "mqkafkaserver.h"
//consumer��producerͨ�ûص�������ͳһ����������
//rd_kafka_yield����ǿ���жϻص�
//kafka��־�ص���levelΪ��־�ȼ���1-7,7���ȼ�
//ֻ�е������õȼ�����־���ܻص������������õȼ�Ϊ4����ֻ��1-4�ܴ�ӡ����
/* kafka�ڲ��ȼ���
#define LOG_EMERG   0
#define LOG_ALERT   1
#define LOG_CRIT    2
#define LOG_ERR     3
#define LOG_WARNING 4
#define LOG_NOTICE  5
#define LOG_INFO    6
#define LOG_DEBUG   7
facΪ������־�Ķ������ͣ���thread��broker��
*/
void common_kafka_log_cb (const rd_kafka_t *rk, int level,
    const char *fac, const char *buf) 
{
    eMQLoglev elev;
    switch (level)
    {
    case 4: elev = MQ_LOG_WARN_LEVEL; break;
    case 5:
    case 6: elev = MQ_LOG_NORMAL_LEVEL; break;
    case 7: elev = MQ_LOG_DBG_LEVEL; break;
    default:
        elev = MQ_LOG_ERR_LEVEL;
    }
//     if (!rk) 
//     {
        GetManager()->PrintLog(elev, "rdkafkaerror:%s:%s\n", fac, buf);
//         return;
//     }
//     //��ȡ����ǰrk���û�����
//     void *opaque = rd_kafka_opaque(rk);
//     RdKafka::HandleImpl *handle = static_cast<RdKafka::HandleImpl *>(opaque);
//     RdKafka::EventImpl event(RdKafka::Event::EVENT_LOG,
//         RdKafka::ERR_NO_ERROR,
//         static_cast<RdKafka::Event::Severity>(level),
//         fac, buf);
//     handle->event_cb_->event_cb(event);
}

//����ص�����δ�뵽���ʹ�ã�ֱ�Ӵ�ӡ��־ error_cb_trampoline
void common_kafka_err_cb (rd_kafka_t *rk, int err,
    const char *reason, void *opaque) 
{
    CMqKafkaBase * pBaseCls = (CMqKafkaBase*)opaque;
    switch (err)
    {
    case RD_KAFKA_RESP_ERR__ALL_BROKERS_DOWN://��������Ⱥ����崻�
        pBaseCls->OnDisConnect();
        break;
    case RD_KAFKA_RESP_ERR__DESTROY:        //����broker����
    //case RD_KAFKA_RESP_ERR__TRANSPORT:      //��Ϣ����ʧ��
    case RD_KAFKA_RESP_ERR__RESOLVE:        //������������ַʧ��
    case RD_KAFKA_RESP_ERR__PARTITION_EOF:  //������Ϣĩβ
    case RD_KAFKA_RESP_ERR__UNKNOWN_PARTITION:
    case RD_KAFKA_RESP_ERR__FS:             /** File or filesystem error */
    case RD_KAFKA_RESP_ERR__UNKNOWN_TOPIC:
    case RD_KAFKA_RESP_ERR__INVALID_ARG:
    case RD_KAFKA_RESP_ERR__QUEUE_FULL:
        break;
    }
    
    GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "rdkafkaerror:%s\n", IRdKafka::_err2str(static_cast<rd_kafka_resp_err_t>(err)));
}

//�����ȼ��Ļص���throttle_cb_trampoline 
void common_kafka_throttle_cb(rd_kafka_t *rk, const char *broker_name,
    int32_t broker_id,
    int throttle_time_ms,
    void *opaque) 
{
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "rdkafka throttle:brokername:%s,brokerid:%d,time:%d\n", 
        broker_name, broker_id, throttle_time_ms);
//     RdKafka::HandleImpl *handle = static_cast<RdKafka::HandleImpl *>(opaque);
//     RdKafka::EventImpl event(RdKafka::Event::EVENT_THROTTLE);
//     event.str_ = broker_name;
//     event.id_ = broker_id;
//     event.throttle_time_ = throttle_time_ms;
//     handle->event_cb_->event_cb(event);
}

//kafka״̬��Ϣ�ص� stats_cb_trampoline 
int common_kafka_stats_cb(rd_kafka_t *rk, char *json, size_t json_len,
    void *opaque) 
{
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "rdkafka stats:%s\n", 
        json);
//     RdKafka::HandleImpl *handle = static_cast<RdKafka::HandleImpl *>(opaque);
//     RdKafka::EventImpl event(RdKafka::Event::EVENT_STATS,
//         RdKafka::ERR_NO_ERROR,
//         RdKafka::Event::EVENT_SEVERITY_INFO,
//         NULL, json);
//     handle->event_cb_->event_cb(event);
    return 0;
}

CMqKafkaServer::CMqKafkaServer()
{
    m_bState = SERVER_OFFLINE;
    m_nServerId = 0;
    m_prkMetadata = NULL;
    m_cIndexGenerater.Init(0xffff);
}

CMqKafkaServer::~CMqKafkaServer()
{
    if (NULL != m_prkMetadata)
    {
        IRdKafka::_metadata_destroy(m_prkMetadata);
        m_prkMetadata = NULL;
    }
}

BOOL32 CMqKafkaServer::LoginServer(MQHANDLE nServerId, PTMQServerInfo ptServerArr, u32 nServerAddrNum, const s8* szUsername , const s8* szPassword, s32 nTimeout)
{
    if (NULL != m_prkServer || NULL != m_prkConf)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::Connect:ERR,m_prkServer is not NULL\n");
        return FALSE;
    }
    if (nServerId <= 0)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::LoginServer:ERR,serverid invalid:%d\n", nServerId);
        return FALSE;
    }
//    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaServer::LoginServer:class handle:%d\n", nServerId);
    m_nServerId = nServerId;
    m_nConnectTimeout = nTimeout;
    /* ��ƴ��brokers�ַ���*/
    PTMQServerInfo ptInfo = NULL;
    char szPort[20] = {0};
    for (unsigned int i = 0; i <nServerAddrNum; ++i)
    {
        if (0 != i)
        {
            m_strBrokers.append(",");
        }
        //���ݽṹ���С������ת����֤��չ������
        ptInfo = ptServerArr;
        if (0 == ptInfo->dwSize)
        {
            ptInfo = ptServerArr + i;
        }
        else
        {
            ptInfo = (PTMQServerInfo)(((char*)ptServerArr) + (i * ptInfo->dwSize));
        }
        if(strlen(ptInfo->szServerHost) == 0 || ptInfo->nServerPort == 0)
        {
            GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::Connect:ERR,ptServerArr[%d] host:%s,port:%d invalid\n",
                i, ptInfo->szServerHost, ptInfo->nServerPort);
            continue;
        }
        memset(szPort, 0, sizeof(szPort));
        sprintf(szPort, "%d", ptInfo->nServerPort);
        m_strBrokers.append(ptInfo->szServerHost);
        m_strBrokers.append(":");
        m_strBrokers.append(szPort);
    }

    /* Add brokers */
    if (m_strBrokers.empty()) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::Connect:ERR,server addr is null,brokers:%s\n"
            , m_strBrokers.c_str());
        return FALSE;
    }

    char errstr[512] = {0};
    rd_kafka_resp_err_t err = RD_KAFKA_RESP_ERR_NO_ERROR;
    //����kafka����Ĭ��ֵ
    m_prkConf = IRdKafka::_conf_new();
    IRdKafka::_conf_set_opaque(m_prkConf, this);
    //����client.id������Ĭ��ֵ��дMQ_KAFKA_SERVER_DEF_CLIENT_ID
    IRdKafka::_conf_set(m_prkConf, "client.id", MQ_KAFKA_SERVER_DEF_CLIENT_ID, errstr, sizeof(errstr));
    //�ر�10���ӵ�Ĭ�϶Ͽ��������Ӵ�ӡ
    IRdKafka::_conf_set(m_prkConf, "log.connection.close", "false", errstr, sizeof(errstr));
    IRdKafka::_conf_set_log_cb(m_prkConf,common_kafka_log_cb);
    IRdKafka::_conf_set_error_cb(m_prkConf,common_kafka_err_cb);
    IRdKafka::_conf_set_throttle_cb(m_prkConf,common_kafka_throttle_cb);
    IRdKafka::_conf_set_stats_cb(m_prkConf,common_kafka_stats_cb);

    return Connect(nTimeout);
}
//�������ӷ���������ȡmetadata����
BOOL32 CMqKafkaServer::Connect(s32 nTimeout)
{
    char errstr[512] = {0};
    rd_kafka_resp_err_t err = RD_KAFKA_RESP_ERR_NO_ERROR;
    //����һ��kafka����
    rd_kafka_conf_t *pmetaconf = IRdKafka::_conf_dup(m_prkConf);
    /* ����kafka consumer��� */
    if (!(m_prkServer = IRdKafka::_new(RD_KAFKA_CONSUMER, pmetaconf,        //RD_KAFKA_PRODUCER
        errstr, sizeof(errstr)))) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::Connect:ERR,create kafka consumer failed:%s\n",
            errstr);
        return FALSE;
    }
    /* Add brokers */
    if ( IRdKafka::_brokers_add(m_prkServer, m_strBrokers.c_str()) == 0) {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::Connect:ERR,No valid brokers specified,brokers:%s\n"
            , m_strBrokers.c_str());
        return FALSE;
    }
    /* Fetch metadata */
    err = IRdKafka::_metadata(m_prkServer, 1, NULL,
        &m_prkMetadata, nTimeout);
    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::Connect:ERR, Failed to acquire metadata: %s\n",
            IRdKafka::_err2str(err));
        IRdKafka::_destroy(m_prkServer);
        m_prkServer = NULL;
        m_bState = SERVER_OFFLINE;
        return FALSE;
    }
    m_bState = SERVER_ONLINE;
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaServer::Connect: success\n");
    return TRUE;
}

BOOL32 CMqKafkaServer::ReConnect(s32 nTimeout)
{
    if (m_bState == SERVER_ONLINE)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::ReConnect: server now online\n");
        return FALSE;
    }
    //�Ƚ�������ض�������ӶϿ�
    /*����ondisconnect������������Ϊondisconnect���ܻ���receiver��SubscribeThread������
    �ᵼ��ִ��receiver��disconnect�ر�SubscribeThread�߳�ʧ�ܣ�����������crash��
    */
    do{
        CThreadAutoLock cLock(m_clmLock);
        //����sender��receiver�������
        std::vector<SmartPtr<CMqKafkaSender> >::iterator iteSender = m_vecpSender.begin();
        while (iteSender != m_vecpSender.end())
        {
            (*iteSender)->DisConnect();
            ++iteSender;
        }
        std::vector<SmartPtr<CMqKafkaReceiver> >::iterator iteReceiver =  m_vecpReceiver.begin();
        while (iteReceiver != m_vecpReceiver.end())
        {
            (*iteReceiver)->DisConnect(true);
            ++iteReceiver;
        }
    }while(false);
    if (NULL == m_prkConf)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::ReConnect: conf is null\n");
        return FALSE;
    }
    if (m_strBrokers.empty())
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::ReConnect: m_strBrokers is empty\n");
        return FALSE;
    }
    if (NULL != m_prkMetadata)
    {
        IRdKafka::_metadata_destroy(m_prkMetadata);
        m_prkMetadata = NULL;
    }
    if (NULL != m_prkServer)
    {
        IRdKafka::_destroy(m_prkServer);
        m_prkServer = NULL;
    }
    if (TRUE != Connect(nTimeout))
    {
        return FALSE;
    }
    //����д�����sender��receiver������������һ��
    do 
    {
        CThreadAutoLock cLock(m_clmLock);
        //����sender��receiver�������
        std::vector<SmartPtr<CMqKafkaSender> >::iterator iteSender = m_vecpSender.begin();
        while (iteSender != m_vecpSender.end())
        {
            (*iteSender)->Connect((*iteSender)->m_nConnectTimeout);
            ++iteSender;
        }
        std::vector<SmartPtr<CMqKafkaReceiver> >::iterator iteReceiver =  m_vecpReceiver.begin();
        while (iteReceiver != m_vecpReceiver.end())
        {
            (*iteReceiver)->Connect((*iteReceiver)->m_nConnectTimeout);
            if ((*iteReceiver)->m_bSubFlag)
            {
                //���֮ǰ�Ѿ���ʼ���գ�������������
                (*iteReceiver)->StartReveive();
            }
            ++iteReceiver;
        }
    } while (false);
    return TRUE;
}

BOOL32 CMqKafkaServer::Logout()
{
    //����sender��receiver�������
    CThreadAutoLock cLock(m_clmLock);
    do 
    {
        std::vector<SmartPtr<CMqKafkaSender> >::iterator iteSender = m_vecpSender.begin();
        while (iteSender != m_vecpSender.end())
        {
            //ɾ������ᵼ�µ�����ʧЧ��ÿ��ɾ��֮������ѭ��
            DeleteSender((*iteSender)->GetSenderId());
            break;
        }
        std::vector<SmartPtr<CMqKafkaReceiver> >::iterator iteReceiver =  m_vecpReceiver.begin();
        while (iteReceiver != m_vecpReceiver.end())
        {
            //ɾ������ᵼ�µ�����ʧЧ��ÿ��ɾ��֮������ѭ��
            DeleteReceiver((*iteReceiver)->GetReceiverId());
            break;
        }
    } while (m_vecpSender.size() > 0 || m_vecpReceiver.size() > 0);
    if (NULL != m_prkMetadata)
    {
        IRdKafka::_metadata_destroy(m_prkMetadata);
        m_prkMetadata = NULL;
    }
    if (NULL != m_prkServer)
    {
        IRdKafka::_destroy(m_prkServer);
        m_prkServer = NULL;
    }
    if (NULL != m_prkConf)
    {
        IRdKafka::_conf_destroy(m_prkConf);
        m_prkConf = NULL;
    }
    return TRUE;
}

BOOL32 CMqKafkaServer::GetMetadata(const rd_kafka_metadata_t* &prdMetadata)
{
    if (m_bState == SERVER_ONLINE)
    {
        prdMetadata = m_prkMetadata;
        return TRUE;
    }
    prdMetadata = NULL;
    return FALSE;
}

eServerState CMqKafkaServer::GetState()
{
    return m_bState;
}

//����sender��recever��Ψһid
MQTOPICHANDLE CMqKafkaServer::CreateTopicId()
{
    //�ֶ�����topicid�����򣺸�16λȡserverhandle��ֵ����16λ����ǰserver���е�topic��+1
    s32 nTopicId = m_cIndexGenerater.GetIndexCount() + 1;
    if ((nTopicId & 0xffff0000) > 0)
    {
        //��ǰ������sender��receiver������Ŀ����65535���������������
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateTopicId: failed,sender and receiver too much(>65535)\n");
        return 0;
    }
    nTopicId = m_cIndexGenerater.GenIndex();
    if (0 == nTopicId)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateTopicId: failed,genIdx = 0\n");
        return 0;
    }
    nTopicId += m_nServerId << 16;
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaServer::CreateTopicId:new topicid is %d\n", 
        nTopicId);
    return (MQTOPICHANDLE)nTopicId;
}
void CMqKafkaServer::DeleteTopicId(MQTOPICHANDLE nTopicId)
{
    u32 nIdx = nTopicId & 0xffff;
    m_cIndexGenerater.RevokeIndex(nIdx);
}
//������ɾ��������
MQTOPICHANDLE CMqKafkaServer::CreateSender(const s8* pszTopic, TMQSenderParam &tSendParam, s32 nTimeout)
{
    MQTOPICHANDLE nNewHandle = 0;
    {//������ΪС����ֻ��������źͲ�������ʱ�Ž�������
        CThreadAutoLock cLock(m_clmLock);
        nNewHandle = CreateTopicId();
        if (0 == nNewHandle)
        {
            //GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateSender: ERR, create topichandle failed \n");
            return 0;
        }
    }
    CMqKafkaSender *pcSender = new CMqKafkaSender();
    s32 nRet = pcSender->Init(nNewHandle, this, pszTopic, tSendParam, nTimeout);
    if (MQ_ERR_NO != nRet)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateSender: failed,errno=%d\n", nRet);
        DeleteTopicId(nNewHandle);
        return 0;
    }
    {
        CThreadAutoLock cLock(m_clmLock);
        m_vecpSender.push_back(SmartPtr<CMqKafkaSender>(pcSender));
    }
    return nNewHandle;
}
SmartPtr<CMqKafkaSender> CMqKafkaServer::FindSender(MQTOPICHANDLE handle)
{
    CThreadAutoLock cLock(m_clmLock);
    std::vector<SmartPtr<CMqKafkaSender> >::iterator iteSender = m_vecpSender.begin();
    while(iteSender != m_vecpSender.end())
    {
        if ((*iteSender)->GetSenderId() == handle)
        {
            break;
        }
        ++iteSender;
    }
    if (m_vecpSender.end() == iteSender)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::FindSender: ERR,can't find handle:%d\n", handle);
        return SmartPtr<CMqKafkaSender>(NULL);
    }
    return *iteSender;
}
s32 CMqKafkaServer::DeleteSender(MQTOPICHANDLE handle)
{
    SmartPtr<CMqKafkaSender> pcSender(NULL);
    {
        CThreadAutoLock cLock(m_clmLock);
        //��ʹ��FindSender����ΪҪʹ�õ�����ɾ��vector�е�ָ��
        std::vector<SmartPtr<CMqKafkaSender> >::iterator iteSender = m_vecpSender.begin();
        while(iteSender != m_vecpSender.end())
        {
            if ((*iteSender)->GetSenderId() == handle)
            {
                pcSender = *iteSender;
                m_vecpSender.erase(iteSender);
                break;
            }
            ++iteSender;
        }
        if (NULL == pcSender)
        {
            GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::DeleteSender: ERR,can't find handle:%d\n", handle);
            return MQ_ERR_HANDLE_NOTFOUND;
        }
    }
    pcSender->UnInit();
    //delete pcSender;
    DeleteTopicId(handle);
    return MQ_ERR_NO;
}
//������е�sender���ֶ�ִ��rd_kafka_poll�������Ի�ȡ���ǵ���Ϣ���¼�
void CMqKafkaServer::PollSenderMsg()
{
    //����ʱ�Ĵ�����Ϊ��ѯʱ��С����ͨ����ѯ�����Ƿ�һ���������Ƿ�����ѯ
    SmartPtr<CMqKafkaSender> pSender(NULL); 
    bool bFindFlag = true; 
    do{
        {//����һ����ѯ����ѯ�ϴα������Ľڵ�λ��
            CThreadAutoLock cLock(m_clmLock);
            std::vector<SmartPtr<CMqKafkaSender> >::iterator iteSender = m_vecpSender.begin();
            while(iteSender != m_vecpSender.end())
            {
                if (NULL == pSender || bFindFlag)
                {//��־λΪtrue��������һ��������ѯ��server���󣬸�ֵ��������ǰѭ��
                    pSender = *iteSender;
                    bFindFlag = false;
                    break;
                }
                if (pSender == *iteSender)
                {
                    bFindFlag = true;
                }
                ++iteSender;
            }
        }
        if (bFindFlag)
        {//û�������ڵ��ˣ��˳�ѭ��
            break;
        }
        if (SERVER_ONLINE == GetState() && NULL != pSender->m_prkServer)
        {
            IRdKafka::_poll(pSender->m_prkServer, 0); //����ֵΪ�����ص��ĸ���
        }
    }while(SERVER_ONLINE == GetState());
}
//������ɾ��������
MQTOPICHANDLE CMqKafkaServer::CreateReceiver(PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQReceiverParam &tReceiverParam, s32 nTimeout)
{
    MQTOPICHANDLE nNewHandle = 0;
    {
        CThreadAutoLock cLock(m_clmLock);
        nNewHandle = CreateTopicId();
        if (0 == nNewHandle)
        {
            //GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateSender: ERR, create topichandle failed \n");
            return 0;
        }
    }
    CMqKafkaReceiver *pcReceiver = new CMqKafkaReceiver();
    s32 nRet = pcReceiver->Init(nNewHandle, this, pTopicArr, nTopicNum, tReceiverParam, nTimeout);
    if (MQ_ERR_NO != nRet)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateReceiver: failed,errno=%d\n", nRet);
        DeleteTopicId(nNewHandle);
        return 0;
    }
    {
        CThreadAutoLock cLock(m_clmLock);
        m_vecpReceiver.push_back(SmartPtr<CMqKafkaReceiver>(pcReceiver));
    }
    return nNewHandle;
}

MQTOPICHANDLE CMqKafkaServer::CreatePreciseReceiver(PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQPreciseReceiverParam &tReceiverParam, s32 nTimeout)
{
    MQTOPICHANDLE nNewHandle = 0;
    {
        CThreadAutoLock cLock(m_clmLock);
        nNewHandle = CreateTopicId();
        if (0 == nNewHandle)
        {
            //GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateSender: ERR, create topichandle failed \n");
            return 0;
        }
    }
    CMqKafkaReceiver *pcReceiver = new CMqKafkaReceiver();
    s32 nRet = pcReceiver->Init(nNewHandle, this, pTopicArr, nTopicNum, tReceiverParam, nTimeout);
    if (MQ_ERR_NO != nRet)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::CreateReceiver: failed,errno=%d\n", nRet);
        DeleteTopicId(nNewHandle);
        return 0;
    }
    {
        CThreadAutoLock cLock(m_clmLock);
        m_vecpReceiver.push_back(SmartPtr<CMqKafkaReceiver>(pcReceiver));
    }
    return nNewHandle;
}

SmartPtr<CMqKafkaReceiver> CMqKafkaServer::FindReceiver(MQTOPICHANDLE handle)
{
    CThreadAutoLock cLock(m_clmLock);
    std::vector<SmartPtr<CMqKafkaReceiver> >::iterator iteReceiver = m_vecpReceiver.begin();
    while(iteReceiver != m_vecpReceiver.end())
    {
        if ((*iteReceiver)->GetReceiverId() == handle)
        {
            break;
        }
        ++iteReceiver;
    }
    if (m_vecpReceiver.end() == iteReceiver)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::FindReceiver: ERR,can't find handle:%d\n", handle);
        return NULL;
    }
    return *iteReceiver;
}
s32 CMqKafkaServer::DeleteReceiver(MQTOPICHANDLE handle)
{
    SmartPtr<CMqKafkaReceiver> pcReceiver(NULL);
    {
        CThreadAutoLock cLock(m_clmLock);
        std::vector<SmartPtr<CMqKafkaReceiver> >::iterator iteReceiver = m_vecpReceiver.begin();
        while(iteReceiver != m_vecpReceiver.end())
        {
            if ((*iteReceiver)->GetReceiverId() == handle)
            {
                pcReceiver = *iteReceiver;
                m_vecpReceiver.erase(iteReceiver);
                break;
            }
            ++iteReceiver;
        }
        //erase��iterator��ʧЧ��������������m_vecpReceiver.end()���Ƚ�
        if (NULL == pcReceiver)
        {
            GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::DeleteReceiver: ERR,can't find handle:%d\n", handle);
            return MQ_ERR_HANDLE_NOTFOUND;
        }
    }
    pcReceiver->UnInit();
    //delete pcReceiver;
    DeleteTopicId(handle);
    return MQ_ERR_NO;
}
//�����������¼�
void CMqKafkaServer::OnDisConnect()
{
    GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::OnDisConnect server unreachable\n");
    if (m_bState == SERVER_ONLINE)
    {
        m_bState = SERVER_OFFLINE;
        GetManager()->OnServerDown(this);
    }
    
}
//��������Ϣ�¼�������
void CMqKafkaServer::OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen)
{
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaServer::OnMessageCb type:%d, errno:%d,param:%d, val:%s\n",
        eMsgType, nErrNo, nMsgParam, (const char*)pMsgVal);
}

//��ȡ���������ӵ�ַ�ַ���
std::string CMqKafkaServer::GetBrokerStr()
{
    std::string strBrokers;
    const rd_kafka_metadata_t* prkmetadata = NULL;
    if(FALSE == GetMetadata(prkmetadata))
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaServer::GetBrokerStr failed! err:MQ_ERR_SERVER_DOWN");
        return strBrokers;
    }
    /* ƴ��brokers�ַ���*/
    char szPort[20] = {0};
    for (int i = 0 ; i < prkmetadata->broker_cnt ; i++)
    {
//         GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "\tbroker:%d at %s:%d\n",
//             prkmetadata->brokers[i].id, prkmetadata->brokers[i].host, prkmetadata->brokers[i].port);
        if (0 != i)
        {
            strBrokers.append(",");
        }
        memset(szPort, 0, sizeof(szPort));
        sprintf(szPort, "%d", prkmetadata->brokers[i].port);
        strBrokers.append(prkmetadata->brokers[i].host);
        strBrokers.append(":");
        strBrokers.append(szPort);
    }
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaServer::GetBrokerStr:%s\n"
        , strBrokers.c_str());
    return strBrokers;
}
//
rd_kafka_t* CMqKafkaServer::GetRKServer()
{
    return m_prkServer;
}