#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "mqkafkasender.h"
#include "mqkafkaserver.h"
#include "mqkafkamanager.h"
#include "mqinterface.h"

enum eSyncMsgState
{
    SENDER_SYNC_DEFAULT_STATE = RD_KAFKA_RESP_ERR_END_ALL + 1,
};
void sender_dr_cb (rd_kafka_t *rk,
    void *payload, size_t len,
    rd_kafka_resp_err_t err,
    void *opaque, void *msg_opaque) {
    //opaqueΪ��ǰtopicͨ��rd_kafka_conf_set_opaque�����������õ��û�����
    if(NULL != msg_opaque)
    {
        int *pstatus = (int*)msg_opaque;
        *pstatus = err;
    }
}

static void sender_dr_msg_cb (rd_kafka_t *rk,
    const rd_kafka_message_t *
    rkmessage,
    void *opaque) {
        //opaqueΪ��ǰtopicͨ��rd_kafka_conf_set_opaque�����������õ��û�����
        //ÿ����Ϣ���û�����Ϊrkmessage->_private
        if(NULL != rkmessage->_private)
        {
            int *pstatus = (int*)rkmessage->_private;
            *pstatus = rkmessage->err;
        }
}

CMqKafkaSender::CMqKafkaSender()
{
    m_bInit = FALSE;
    m_bConnect = FALSE;
    m_nSenderId = 0;
    m_pcMqServer = NULL;
    m_pEventCb = NULL;
    m_pEventUser = NULL;
    m_nMaxQueueNum = 0;
    m_nMaxQueueRetainTime = 0; 
    memset(m_szTopic, 0, sizeof(m_szTopic));
    m_prkTopic = NULL;
    m_nMsgState = 0;
}
CMqKafkaSender::~CMqKafkaSender()
{
    if (m_bInit)
    {
        UnInit();
    }
}
//��������ʼ��sender
s32 CMqKafkaSender::Init(MQTOPICHANDLE nSenderId, CMqKafkaServer *pcMqServer, const s8* szTopic, TMQSenderParam &tSendParam, s32 nTimeout)
{
    if (m_bInit)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Init error: class already inited!\n");
        return MQ_ERR_HANDLE_ALREADY_EXISTS;
    }
    if (0 == nSenderId || NULL == pcMqServer || NULL == szTopic || NULL == tSendParam.pEventCb || 0 == strlen(szTopic))
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Init error: param invalid! senderid:%d,pcserver:%X,psztopic:%X,pEventCb:%X\n",
            nSenderId, pcMqServer, szTopic, tSendParam.pEventCb);
        return MQ_ERR_INVALID_PARAM;
    }
    if (0 > tSendParam.nMaxQueueNum || 10000000 < tSendParam.nMaxQueueNum)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Init error: param invalid! tSendParam.nMaxQueueNum:%d must between [1 - 10000000]\n",
            tSendParam.nMaxQueueNum);
        return MQ_ERR_INVALID_PARAM;
    }
    if (0 > tSendParam.nMaxQueueRetainTime || 900000 < tSendParam.nMaxQueueRetainTime)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Init error: param invalid! tSendParam.nMaxQueueRetainTime must between [0 - 900000]\n");
        return MQ_ERR_INVALID_PARAM;
    }
    m_nSenderId = nSenderId;
    m_pcMqServer = pcMqServer;
    m_pEventCb = tSendParam.pEventCb;
    m_pEventUser = tSendParam.pUser;
    m_nMaxQueueNum = tSendParam.nMaxQueueNum;
    m_nMaxQueueRetainTime = tSendParam.nMaxQueueRetainTime; 
    m_nConnectTimeout = nTimeout;
    strcpy(m_szTopic, szTopic);
    m_prkConf = IRdKafka::_conf_new();
    //����conf����
    /*����dr_msg_cb ��dr_cb,������msg_cb����������Ӧmsg_cb��������Ӧdr_cb������ֻ��Ҫ����һ���Ϳ���*/
    //rd_kafka_conf_set_dr_msg_cb(m_prkConf, sender_dr_msg_cb);
    IRdKafka::_conf_set_dr_cb(m_prkConf, sender_dr_cb);
    IRdKafka::_conf_set_opaque(m_prkConf, this);
    //����client.id�������������Ϊ�գ�Ĭ��ֵ��дMQ_KAFKA_PRODUCER_CLIENT_ID
    char errstr[512] = {0};
    if (strlen(tSendParam.szName) > 0)
    {
        IRdKafka::_conf_set(m_prkConf, "client.id", tSendParam.szName, errstr, sizeof(errstr));
    }
    else
    {
        IRdKafka::_conf_set(m_prkConf, "client.id", MQ_KAFKA_PRODUCER_CLIENT_ID, errstr, sizeof(errstr));
    }
    //�ر�10���ӵ�Ĭ�϶Ͽ��������Ӵ�ӡ
    IRdKafka::_conf_set(m_prkConf, "log.connection.close", "false", errstr, sizeof(errstr));
    //���ö�����Ϣ�߼�����
    char szNumber[512] = {0};
    if (0 != m_nMaxQueueNum)
    {
        sprintf(szNumber, "%d", m_nMaxQueueNum);
        if(IRdKafka::_conf_set(m_prkConf, "queue.buffering.max.messages", szNumber, errstr, sizeof(errstr)) !=
            RD_KAFKA_CONF_OK)
        {
            GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Init set maxQueueNum error: %s\n",
                errstr);
            return MQ_ERR_SDK;
        }
    }
    if (0 != m_nMaxQueueRetainTime)
    {
        sprintf(szNumber, "%d", m_nMaxQueueRetainTime);
        if(IRdKafka::_conf_set(m_prkConf, "queue.buffering.max.ms", szNumber, errstr, sizeof(errstr)) !=
            RD_KAFKA_CONF_OK)
        {
            GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Init set maxQueueRetainTime error: %s\n",
                errstr);
            return MQ_ERR_SDK;
        }
    }
    IRdKafka::_conf_set_log_cb(m_prkConf,common_kafka_log_cb);
    IRdKafka::_conf_set_error_cb(m_prkConf,common_kafka_err_cb);
    IRdKafka::_conf_set_throttle_cb(m_prkConf,common_kafka_throttle_cb);
    IRdKafka::_conf_set_stats_cb(m_prkConf,common_kafka_stats_cb);
    
    m_bInit = TRUE;
    return Connect(nTimeout);
}
//������Ϣ
s32 CMqKafkaSender::SendMessageToMQ(const u8* szValue, s32 nValueLen, const u8* szKey, s32 nKeyLen, bool bSync, bool bType, s32 nTimeout)
{
    if (!m_bInit || NULL == m_prkTopic)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::SendMessageToMQ error: uninit or topic is null!\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    if (!m_bConnect)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::SendMessageToMQ error: sender not connected!\n");
        return MQ_ERR_SERVER_DOWN;
    }
#ifndef NDEBUG
#ifdef _WIN32
# ifndef snprintf
# define snprintf _snprintf
# endif
#endif
    //NDEBUG����win��linux�¶����ڣ���_DEBUG���޶�Ӧƽ̨
    //����ʱ���
    char szMqTime[50] = {0};
    timeb tcurrent;
    ftime(&tcurrent);
    tm *pTM =localtime(&tcurrent.time);
    snprintf(szMqTime, sizeof(szMqTime) - 1, "[%d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d.%0.3d]",
        pTM->tm_year+1900, pTM->tm_mon+1, pTM->tm_mday,
        pTM->tm_hour, pTM->tm_min, pTM->tm_sec, tcurrent.millitm);
    std::string strMqLog("MqKafkaSender::SendMessage");
    strMqLog.append(szMqTime);
    strMqLog.append(" topic:").append(m_szTopic);
    if (NULL != szKey)
    {
        strMqLog.append(" key:").append((const char*)szKey, nKeyLen);
    }
    if (NULL != szValue)
    {
        strMqLog.append(" value:").append((const char*)szValue,nValueLen);
    }
    strMqLog.append("\n");
    printf(strMqLog.c_str());
#endif
    //COPY flag���裬���򲻻Ὣ���ݿ�����lib�ڲ������У�����ʹ�þ�������ʱ���������Ѿ��������
    s32 nMsgFlag = RD_KAFKA_MSG_F_BLOCK | RD_KAFKA_MSG_F_COPY;// RD_KAFKA_MSG_F_COPY;
    s32 nPartition = RD_KAFKA_PARTITION_UA;
    //conf->set("dr_cb", &ex_dr_cb, errstr);
    m_nMsgState = SENDER_SYNC_DEFAULT_STATE;
    if (bSync)
    {
        //TODO ͬ����������
        //RD_KAFKA_MSG_F_BLOCK������ͬ���������ݣ����Ǳ�֤���ݱ����뵽rdkafka����Ϣ������
        //������Ϣ�Ƿ��͵���������ͨ��dr_cb�õ���
        nMsgFlag = RD_KAFKA_MSG_F_BLOCK;
    }
    IRdKafka::_poll(m_prkServer, 0);
    /* Send/Produce message. */
	if ( IRdKafka::_produce(m_prkTopic, nPartition,
					nMsgFlag,
					/* Payload and length */
					(void*)szValue, nValueLen,
					/* Optional key and its length */
					szKey, nKeyLen,
					/* Message opaque, provided in
					* delivery report callback as
					* msg_opaque. */
					&m_nMsgState) == -1) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::SendMessageToMQ failed:topic %s partition %i: %s\n",
            IRdKafka::_topic_name(m_prkTopic), nPartition, IRdKafka::_err2str(IRdKafka::_last_error()));
		IRdKafka::_poll(m_prkServer, 0);
        return MQ_ERR_SDK;
	}
    if (bSync)
    {
        int nPassTime = 0;
        do{
            IRdKafka::_poll(m_prkServer, 100);    //provide -1 to wait indefinately until dr_cb called
            nPassTime += 100;
        }while(m_nMsgState == SENDER_SYNC_DEFAULT_STATE && nPassTime <= nTimeout);
        if (m_nMsgState == SENDER_SYNC_DEFAULT_STATE)
        {
            return MQ_ERR_TIMEOUT;
        }
    }
    IRdKafka::_poll(m_prkServer, 0);
    return MQ_ERR_NO;
}
//ɾ����sender
s32 CMqKafkaSender::UnInit()
{
    if (!m_bInit)
    {
        //GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::SendMessageToMQ error: uninit !\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    if(m_prkServer && IRdKafka::_outq_len(m_prkServer) > 0)
        IRdKafka::_poll(m_prkServer, 100);
    //���֮ǰ�Ѿ������������֮ǰ����������
    DisConnect();
    return MQ_ERR_NO;
}
//
s32 CMqKafkaSender::Connect(s32 nTimeout)
{
    //��ϢУ��
    if (!m_bInit)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Connect error: class not inited!\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    if (m_bConnect)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Connect error: class has connected!\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    if (0 == m_nSenderId || NULL == m_pcMqServer || NULL == m_pEventCb || 0 == strlen(m_szTopic))
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Connect error: param invalid! senderid:%d,pcserver:%X,sztopic:%s,pEventCb:%X\n",
            m_nSenderId, m_pcMqServer, m_szTopic, m_pEventCb);
        return MQ_ERR_INVALID_PARAM;
    }
    if (0 > m_nMaxQueueNum || 10000000 < m_nMaxQueueNum)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Connect error: param invalid! tSendParam.nMaxQueueNum must between [1 - 10000000]\n");
        return MQ_ERR_INVALID_PARAM;
    }
    if (0 > m_nMaxQueueRetainTime || 900000 < m_nMaxQueueRetainTime)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Connect error: param invalid! tSendParam.nMaxQueueRetainTime must between [0 - 900000]\n");
        return MQ_ERR_INVALID_PARAM;
    }
    //���֮ǰ�Ѿ������������֮ǰ����������
    if (m_prkTopic)
    {
        IRdKafka::_topic_destroy(m_prkTopic);
        m_prkTopic = NULL;
    }
    if(m_prkServer)
    {
        IRdKafka::_destroy(m_prkServer);
        m_prkServer = NULL;
    }
    char errstr[512] = {0};
    //����һ��������Ϣ��ԭ��Ϣ��������
    rd_kafka_conf_t *conf = IRdKafka::_conf_dup(m_prkConf);
    rd_kafka_topic_conf_t *topic_conf = IRdKafka::_topic_conf_new();
    if (!(m_prkServer = IRdKafka::_new(RD_KAFKA_PRODUCER, conf,        //RD_KAFKA_PRODUCER
        errstr, sizeof(errstr)))) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Connect:ERR,create kafka producer failed:%s\n", 
            errstr);
        return MQ_ERR_SDK;
    }
    //ִ��rd_kafka_new�ᵼ�� confָ��Ķ������٣������������confָ���ÿ�
    conf = NULL;
    //��ӷ�������ַ��
    std::string strBrokers = m_pcMqServer->GetBrokerStr();
    if (strBrokers.empty() || IRdKafka::_brokers_add(m_prkServer, strBrokers.c_str()) == 0) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Connect:ERR,No valid brokers specified,brokers:%s\n"
            , strBrokers.c_str());
        IRdKafka::_destroy(m_prkServer);
        m_prkServer = NULL;
        return MQ_ERR_INVALID_PARAM;
    }
    //����kafka�ڲ�Ĭ����־�ȼ�
    IRdKafka::_set_log_level(m_prkServer, 3);
    /* Create topic */
    //�������;��
    m_prkTopic = IRdKafka::_topic_new(m_prkServer, m_szTopic, topic_conf);
    if (NULL == m_prkTopic)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Connect error: create topic failed!err:%s\n",
            IRdKafka::_err2str( IRdKafka::_last_error()));
        return MQ_ERR_SDK;
    }
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaSender::Connect success\n");
    m_bConnect = TRUE;
    return MQ_ERR_NO;
}

s32 CMqKafkaSender::DisConnect()
{
    if (!m_bConnect)
    {
        return MQ_ERR_NO;
    }
    if (m_prkTopic)
    {
        IRdKafka::_topic_destroy(m_prkTopic);
        m_prkTopic = NULL;
    }
    if(m_prkServer)
    {
        IRdKafka::_destroy(m_prkServer);
        m_prkServer = NULL;
    }
    m_bConnect = FALSE;
    return MQ_ERR_NO;
}

//�����������¼�
void CMqKafkaSender::OnDisConnect() 
{
//     if(m_pEventCb)
//     {
//         //typedef void (*MQEventCallBack)(MQTOPICHANDLE handle, s32 nErrno, s8* pszErr, s32 nErrLen, KD_PTR pUser);
//         m_pEventCb(m_nSenderId, MQ_ERR_SERVER_DOWN, "server down", strlen("server down"), m_pEventUser);
//     }
    //����֪ͨ������������Ϣ�����������������
    m_pcMqServer->OnDisConnect();
}

void CMqKafkaSender::OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen)
{
    if(m_pEventCb)
    {
        //typedef void (*MQEventCallBack)(MQTOPICHANDLE handle, s32 nErrno, s8* pszErr, s32 nErrLen, KD_PTR pUser);
        
    }
}