#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "mqkafkareceiver.h"
#include "mqkafkaserver.h"
#include "mqkafkamanager.h"
#include "mqinterface.h"
#include "utility/uuid.h"

#ifdef _WIN32
# ifndef snprintf
# define snprintf _snprintf
# endif
#endif //_WIN32
    
//组订阅时对组的变更会导致rebalance
static void receiver_rebalance_cb (rd_kafka_t *rk, rd_kafka_resp_err_t err,
    rd_kafka_topic_partition_list_t *c_partitions, void *opaque) 
{
    int i = 0;
    CMqKafkaReceiver *pCls = NULL;
    if (NULL != opaque)
    {
        pCls = (CMqKafkaReceiver*)opaque;
    }
    switch (err)
    {
    case RD_KAFKA_RESP_ERR__ASSIGN_PARTITIONS:
        if (pCls->m_nOffset != 0)
        {
            for (i = 0; i < c_partitions->cnt; ++i)
            {
                //对每一个分区，都要设置偏移地址
                IRdKafka::_topic_partition_list_set_offset(c_partitions, c_partitions->elems[i].topic, 
                    c_partitions->elems[i].partition, pCls->m_nOffset);
            }
        }
        IRdKafka::_assign(rk, c_partitions);
        if (NULL != pCls)
        {
            pCls->SetTopicPartitionList(c_partitions);
        }
        break;
    case RD_KAFKA_RESP_ERR__REVOKE_PARTITIONS:
        IRdKafka::_assign(rk, NULL);
        if (NULL != pCls)
        {
            pCls->ResetTopicPartitionList();
        }
        break;
    default:
        IRdKafka::_assign(rk, NULL);
        if (NULL != pCls)
        {
            pCls->ResetTopicPartitionList();
        }
        break;
    }
}

static void receiver_offset_commit_cb (rd_kafka_t *rk, rd_kafka_resp_err_t err,
    rd_kafka_topic_partition_list_t *c_offsets, void *opaque) 
{
    if (NULL != opaque)
    {
        CMqKafkaReceiver *pCls = (CMqKafkaReceiver*)opaque;
        pCls->OnCommitCb(err, c_offsets);
    }
}
//注意：经测试，使用subscribe不会触发consumer_cb回调，只会在rd_kafka_consumer_poll函数返回消息
void receiver_consume_cb(rd_kafka_message_t *msg, void *opaque) 
{
    if (NULL != opaque)
    {
        CMqKafkaReceiver *pCls = (CMqKafkaReceiver*)opaque;
        pCls->OnMessage(msg);
    }
}
CMqKafkaReceiver::CMqKafkaReceiver()
{
    m_bInit = FALSE;
    m_nReceiverId = 0;
    memset(m_szGroup, 0, sizeof(m_szGroup));
    m_nMsgTimeout = 1000;                //消息拉取时的超时时间
    m_pcMqServer = NULL;       //当前sender所属的mqkafkaserver类(服务器)指针
    m_pEventCb = NULL;     //事件回调
    m_pEventUser = NULL;       //回调时回传的用户数据
    m_pMsgCb = NULL;       //数据回调
    m_pMsgUser = NULL;      //数据回调的用户数据
    m_prkTopicList = NULL;
    m_bConnect = FALSE;
    m_bOnSubscribe = false;
    m_bSubFlag = false;
    m_bPrecise = FALSE;
    m_bAutoCommit = TRUE;
    m_pPreciseMsgCb = NULL;
    m_pCommitCb = NULL;
}
CMqKafkaReceiver::~CMqKafkaReceiver()
{
    if (m_bInit)
    {
        UnInit();
    }
    if (NULL != m_prkTopicList)
    {
        IRdKafka::_topic_partition_list_destroy(m_prkTopicList);
        m_prkTopicList = NULL;
    }
    if (m_vecTpList.size() > 0)
    {
        m_vecTpList.clear();
    }
}
//创建并初始化
s32 CMqKafkaReceiver::Init(MQTOPICHANDLE nReceiverId, CMqKafkaServer *pcMqServer, PTMQTopicInfo pTopicArr, 
    u32 nTopicNum, TMQReceiverParam &tReceiverParam, s32 nTimeout)
{
    if (m_bInit)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Init error: class already inited!\n");
        return MQ_ERR_HANDLE_ALREADY_EXISTS;
    }
    if (0 == nReceiverId || NULL == pcMqServer || NULL == pTopicArr || 
        0 == nTopicNum || NULL == tReceiverParam.pEventCb || 0 == nTimeout)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Init error: param invalid! receiverid:%d,pcserver:%X,pTopicArr:%X,pEventCb:%X\n",
            nReceiverId, pcMqServer, pTopicArr, tReceiverParam.pEventCb);
        return MQ_ERR_INVALID_PARAM;
    }
    if (nTopicNum > MAX_TOPIC_NUM)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Init error: param invalid! nTopicNum(%d) > MAX_TOPIC_NUM(%d)\n",
            nTopicNum , MAX_TOPIC_NUM);
        return MQ_ERR_INVALID_PARAM;
    }
    m_nReceiverId = nReceiverId;
    m_pcMqServer = pcMqServer;
    m_pEventCb = tReceiverParam.pEventCb;
    m_pEventUser = tReceiverParam.pUser;
    m_pMsgCb = tReceiverParam.pMessageCb;
    m_pMsgUser = tReceiverParam.pUser;
    m_nMsgTimeout = tReceiverParam.nMsgTimeout;
    if (0 == m_nMsgTimeout)
    {
        m_nMsgTimeout = MQ_DEFAULLT_TIMEOUT;
    }
    m_nConnectTimeout = nTimeout;
    strcpy(m_szGroup, tReceiverParam.szGroup);
    m_prkConf = IRdKafka::_conf_new();
    //设置conf参数
    //设置client.id参数，如果参数为空，默认值填写MQ_KAFKA_PRODUCER_CLIENT_ID
    char errstr[512] = {0};
    if (strlen(tReceiverParam.szName) > 0)
    {
        IRdKafka::_conf_set(m_prkConf, "client.id", tReceiverParam.szName, errstr, sizeof(errstr));
    }
    else
    {
        IRdKafka::_conf_set(m_prkConf, "client.id", MQ_KAFKA_CONSUMER_CLIENT_ID, errstr, sizeof(errstr));
    }
    //关闭10分钟的默认断开空闲连接打印
    IRdKafka::_conf_set(m_prkConf, "log.connection.close", "false", errstr, sizeof(errstr));
    m_prkTopicList = IRdKafka::_topic_partition_list_new(nTopicNum);   //参数为要订阅的topics总数，可以同时订阅多个topic
    for (u32 i = 0; i < nTopicNum; ++i)
    {
        PTMQTopicInfo pCurTopicInfo = pTopicArr+ i;
        if (0 != pTopicArr[0].dwSize)
        {
            pCurTopicInfo = (PTMQTopicInfo)((u8*)pTopicArr + (pTopicArr[0].dwSize * i));
        }
        IRdKafka::_topic_partition_list_add(m_prkTopicList, pCurTopicInfo->szTopicName, RD_KAFKA_PARTITION_UA);
    }
    //groupid处理,如果groupid为空，说明用户需要一个唯一的groupid用来接收所有数据
    if (strlen(m_szGroup) == 0)
    {
        /** 设置一个新的没有使用过的groupid，基础前缀为客户端name+defRecGroup，之后接uuid*/
        char szDefGroupId[MAX_NAME_LEN * 2] = {0};
        char szClientId[MAX_NAME_LEN] = {0};
        int  nNameLen = sizeof(szClientId);
        IRdKafka::_conf_get(m_prkConf, "client.id", szClientId, (size_t*)&nNameLen);
        sprintf(szDefGroupId, "%sdefRecGroup", szClientId);
        CUUID uuid;
        snprintf(m_szGroup, sizeof(m_szGroup) - 1, "%s%s", szDefGroupId, uuid.c_str());
        GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaReceiver::Init:set default groupid:%s\n"
            , m_szGroup);
    }
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaReceiver::Init:groupid:%s\n"
        , m_szGroup);
    IRdKafka::_conf_set(m_prkConf, "group.id", m_szGroup, NULL, 0);
    /* 必需设置offset为broker存储 */
    rd_kafka_topic_conf_t *topic_conf = IRdKafka::_topic_conf_new();
    if (IRdKafka::_topic_conf_set(topic_conf, "offset.store.method", "broker",
        errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Init:ERR,Failed to set offset.store.method to topic,err:%s\n"
            , errstr);
        return MQ_ERR_SDK;
    }
    /* Set default topic config for pattern-matched topics. */
    IRdKafka::_conf_set_default_topic_conf(m_prkConf, topic_conf);
    topic_conf = NULL;

    IRdKafka::_conf_set_log_cb(m_prkConf,common_kafka_log_cb);
    IRdKafka::_conf_set_error_cb(m_prkConf,common_kafka_err_cb);
    IRdKafka::_conf_set_throttle_cb(m_prkConf,common_kafka_throttle_cb);
    IRdKafka::_conf_set_stats_cb(m_prkConf,common_kafka_stats_cb);
    //consumer专用回调部分
    IRdKafka::_conf_set_opaque(m_prkConf, this);
    IRdKafka::_conf_set_rebalance_cb(m_prkConf, receiver_rebalance_cb);
    //rd_kafka_conf_set_offset_commit_cb(m_prkConf, receiver_offset_commit_cb);
    IRdKafka::_conf_set_consume_cb(m_prkConf,receiver_consume_cb);
    m_bInit = TRUE;
    return Connect(nTimeout);
}
s32 CMqKafkaReceiver::Init(MQTOPICHANDLE nReceiverId, CMqKafkaServer *pcMqServer, 
        PTMQTopicInfo pTopicArr, u32 nTopicNum, 
        TMQPreciseReceiverParam &tReceiverParam, s32 nTimeout)
{
    if (m_bInit)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Init error: class already inited!\n");
        return MQ_ERR_HANDLE_ALREADY_EXISTS;
    }
    if (0 == nReceiverId || NULL == pcMqServer || NULL == pTopicArr || 
          0 == nTopicNum || NULL == tReceiverParam.pEventCb || 0 == nTimeout)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Init error: param invalid! receiverid:%d,pcserver:%X,pTopicArr:%X,pEventCb:%X\n",
            nReceiverId, pcMqServer, pTopicArr, tReceiverParam.pEventCb);
        return MQ_ERR_INVALID_PARAM;
    }
    if (nTopicNum > MAX_TOPIC_NUM)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Init error: param invalid! nTopicNum(%d) > MAX_TOPIC_NUM(%d)\n",
            nTopicNum , MAX_TOPIC_NUM);
        return MQ_ERR_INVALID_PARAM;
    }
    m_nReceiverId = nReceiverId;
    m_pcMqServer = pcMqServer;
    m_pEventCb = tReceiverParam.pEventCb;
    m_pEventUser = tReceiverParam.pUser;
    m_pPreciseMsgCb = tReceiverParam.pMessageCb;
    m_pMsgUser = tReceiverParam.pUser;
    m_nMsgTimeout = tReceiverParam.nMsgTimeout;
    m_bAutoCommit = tReceiverParam.bAutoCommit;
    m_pCommitCb = tReceiverParam.pCommitCb;
    m_bPrecise = TRUE;
    if (0 == m_nMsgTimeout)
    {
        m_nMsgTimeout = MQ_DEFAULLT_TIMEOUT;
    }
    m_nConnectTimeout = nTimeout;
    strcpy(m_szGroup, tReceiverParam.szGroup);
    m_prkConf = IRdKafka::_conf_new();
    //设置conf参数
    //设置client.id参数，如果参数为空，默认值填写MQ_KAFKA_PRODUCER_CLIENT_ID
    char errstr[512] = {0};
    if (strlen(tReceiverParam.szName) > 0)
    {
        IRdKafka::_conf_set(m_prkConf, "client.id", tReceiverParam.szName, errstr, sizeof(errstr));
    }
    else
    {
        IRdKafka::_conf_set(m_prkConf, "client.id", MQ_KAFKA_CONSUMER_CLIENT_ID, errstr, sizeof(errstr));
    }
    //关闭10分钟的默认断开空闲连接打印
    IRdKafka::_conf_set(m_prkConf, "log.connection.close", "false", errstr, sizeof(errstr));
    m_prkTopicList = IRdKafka::_topic_partition_list_new(nTopicNum);   //参数为要订阅的topics总数，可以同时订阅多个topic
    for (u32 i = 0; i < nTopicNum; ++i)
    {
        PTMQTopicInfo pCurTopicInfo = pTopicArr+ i;
        if (0 != pTopicArr[0].dwSize)
        {
            pCurTopicInfo = (PTMQTopicInfo)((u8*)pTopicArr + (pTopicArr[0].dwSize * i));
        }
        IRdKafka::_topic_partition_list_add(m_prkTopicList, pCurTopicInfo->szTopicName, RD_KAFKA_PARTITION_UA);
    }
    //groupid处理,如果groupid为空，说明用户需要一个唯一的groupid用来接收所有数据
    if (strlen(m_szGroup) == 0)
    {
        /** 设置一个新的没有使用过的groupid，基础前缀为客户端name+defRecGroup，之后接uuid*/
        char szDefGroupId[MAX_NAME_LEN * 2] = {0};
        char szClientId[MAX_NAME_LEN] = {0};
        int  nNameLen = sizeof(szClientId);
        IRdKafka::_conf_get(m_prkConf, "client.id", szClientId, (size_t*)&nNameLen);
        sprintf(szDefGroupId, "%sdefRecGroup", szClientId);
        CUUID uuid;
        snprintf(m_szGroup, sizeof(m_szGroup) - 1, "%s%s", szDefGroupId, uuid.c_str());
        GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaReceiver::Init:set default groupid:%s\n"
            , m_szGroup);
    }
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaReceiver::Init:groupid:%s\n"
        , m_szGroup);
    IRdKafka::_conf_set(m_prkConf, "group.id", m_szGroup, NULL, 0);
    /* 必需设置offset为broker存储 */
    rd_kafka_topic_conf_t *topic_conf = IRdKafka::_topic_conf_new();
    if (IRdKafka::_topic_conf_set(topic_conf, "offset.store.method", "broker",
        errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Init:ERR,Failed to set offset.store.method to topic,err:%s\n"
            , errstr);
        return MQ_ERR_SDK;
    }
    //设置消息自动提交参数
    IRdKafka::_conf_set(m_prkConf, "enable.auto.commit", m_bAutoCommit ? "true" : "false", NULL, 0);
    /* Set default topic config for pattern-matched topics. */
    IRdKafka::_conf_set_default_topic_conf(m_prkConf, topic_conf);
    topic_conf = NULL;

    IRdKafka::_conf_set_log_cb(m_prkConf,common_kafka_log_cb);
    IRdKafka::_conf_set_error_cb(m_prkConf,common_kafka_err_cb);
    IRdKafka::_conf_set_throttle_cb(m_prkConf,common_kafka_throttle_cb);
    IRdKafka::_conf_set_stats_cb(m_prkConf,common_kafka_stats_cb);
    //consumer专用回调部分
    IRdKafka::_conf_set_opaque(m_prkConf, this);
    IRdKafka::_conf_set_rebalance_cb(m_prkConf, receiver_rebalance_cb);
    if (NULL != m_pCommitCb)
    {
        IRdKafka::_conf_set_offset_commit_cb(m_prkConf, receiver_offset_commit_cb);
    }
    IRdKafka::_conf_set_consume_cb(m_prkConf,receiver_consume_cb);
    m_bInit = TRUE;
    return Connect(nTimeout);
}
s32 CMqKafkaReceiver::GetReceiverInfo(TMQReceiverInfo &tReceiverInfo)
{
    if (!m_bInit)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::GetReceiverInfo error: uninit !\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    int  nValLen = sizeof(tReceiverInfo.szName);
    IRdKafka::_conf_get(m_prkConf, "client.id", tReceiverInfo.szName, (size_t*)&nValLen);
    nValLen = sizeof(tReceiverInfo.szGroup);
    IRdKafka::_conf_get(m_prkConf, "group.id", tReceiverInfo.szGroup, (size_t*)&nValLen);
    tReceiverInfo.nMsgTimeout = m_nMsgTimeout;
    tReceiverInfo.hServer = m_pcMqServer->GetServerId();
    for (int i = 0; i < m_prkTopicList->cnt; ++i)
    {
        tReceiverInfo.tTopicInfoArr[i].dwSize = sizeof(TMQTopicInfo);
        strcpy(tReceiverInfo.tTopicInfoArr[i].szTopicName, m_prkTopicList->elems[i].topic);
    }
    tReceiverInfo.dwTopicInfoArrNum = m_prkTopicList->cnt;
    if (tReceiverInfo.dwSize >= sizeof(TMQReceiverInfo))
    {//为了保证兼容性，只有最新的版本才会向里写入新增的bAutoCommit等信息
        tReceiverInfo.bAutoCommit = m_bAutoCommit;
    }
    return MQ_ERR_NO;      
}
//拉取（poll）数据的线程
void* SubscribeThread(void* args)
{
    CMqKafkaReceiver *pReceiver = (CMqKafkaReceiver*)args;
    if (NULL == pReceiver)
    {
        return NULL;
    }
    while (pReceiver->m_bOnSubscribe) 
    {
        rd_kafka_message_t *rkmessage;
        rkmessage = IRdKafka::_consumer_poll(pReceiver->m_prkServer, pReceiver->m_nMsgTimeout);
        if (rkmessage) {
            if (rkmessage->err == RD_KAFKA_RESP_ERR__PARTITION_EOF)
            {
                //如果需要，几路并判断pReceiver->m_vecTpList内的数据  不处理eof事件，由用户自己通过发送特殊消息的方式判断数据是否全部接受完毕。
                //关于EOF，经测试，每当数据为最后一条时，都会触发EOF，对于订阅就是几乎每一条数据都有一个EOF
//                 GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaReceiver::SubscribeThread:receive a partition EOF at topic:%s,partition:%d!\n",
//                     rd_kafka_topic_name(rkmessage->rkt), rkmessage->partition);
                //continue;
            }else if (RD_KAFKA_RESP_ERR__MSG_TIMED_OUT == rkmessage->err || RD_KAFKA_RESP_ERR__TIMED_OUT == rkmessage->err)
            {
                pReceiver->OnMessageCb(MQ_KAFKA_MSG_EVENT, MQ_ERR_RECEIVER_OUTDATA, 0, (void*)MQGetErrDesc(MQ_ERR_RECEIVER_OUTDATA), strlen(MQGetErrDesc(MQ_ERR_RECEIVER_OUTDATA)));
//                GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaReceiver::SubscribeThread:receive message timeout!\n");
            }else if (RD_KAFKA_RESP_ERR_NO_ERROR != rkmessage->err)
            {
                GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::SubscribeThread:receive ERR at topic:%s,partition:%d!err:%dd-%s\n",
                    NULL == rkmessage->rkt ? "" : IRdKafka::_topic_name(rkmessage->rkt), rkmessage->partition, rkmessage->err, IRdKafka::_message_errstr(rkmessage));
            }
            else
            { 
                pReceiver->OnMessage(rkmessage);
            }
            /* Return message to rdkafka */
            IRdKafka::_message_destroy(rkmessage);
        }
    }
    return NULL;
}
s32 CMqKafkaReceiver::StartReveive(s32 nOffse)
{
    if (!m_bInit)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::StartReveive error: uninit !\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    if (!m_bConnect)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::StartReveive error: receiver not connected!\n");
        return MQ_ERR_SERVER_DOWN;
    }
    if (m_bOnSubscribe)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::StartReveive:ERR,receiver has been started!\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    int nIdx = 0;
    switch (nOffse)
    {
    case FROM_BEGINNING:
        //
        m_nOffset = RD_KAFKA_OFFSET_BEGINNING;
    	break;
    case FROM_ENDING:
        m_nOffset = RD_KAFKA_OFFSET_END;
        break;
    case FROM_DEFAULT:
        m_nOffset = 0;
        break;
    default:
        //TODO 实现指定位置的偏移
        m_nOffset = 0;;
    }
    //rd_kafka_subscribe会在内部拷贝partitionlist，不需要另外拷贝一份
    rd_kafka_resp_err_t errRsp = IRdKafka::_subscribe(m_prkServer, m_prkTopicList);
    if (errRsp) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::StartReveive error: subscribe failed!err:%s\n",
            IRdKafka::_err2str(errRsp));
        return MQ_ERR_SDK;
    }
    //创建一个线程用来执行consumer_poll函数
    //多线程使用ptherad，不用为不同平台做特殊处理 注意：pthread_create会瞬间返回，不能在创建多个线程时使用同一个参数指针
    m_bOnSubscribe = true;
    int ret = pthread_create(&m_tSubScribeThread, NULL, SubscribeThread, this);
    if (ret != 0)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::StartReveive:create thread failed: error_code=%d", ret );
        return MQ_ERR_UNKNOWN;
    }
    m_bSubFlag = true;
    //当设置了consume_cb，执行comsumer_poll收到数据不会返回，只有收到事件才会返回
    return MQ_ERR_NO;
}
s32 CMqKafkaReceiver::StopReceive(bool bIsReconnect)
{
    if (!m_bInit)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::StopReceive error: uninit !\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    if (!m_bSubFlag)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::StopReceive:ERR,receiver has not been started!\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    if (m_bOnSubscribe)
    {
        m_bOnSubscribe = false;
        //线程返回后并没有释放资源,而是等待join后才释放的资源
        int nRet = pthread_join(m_tSubScribeThread,NULL);
        if (0 != nRet)
        {
            GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::StopReceive error: ScribeThread thread exit failed!errcode:%d\n",
                nRet);
            m_bOnSubscribe = true;  //取消订阅失败，复原订阅标志位
            return MQ_ERR_UNKNOWN;
        }
        //先退出线程，再取消订阅
        IRdKafka::_unsubscribe(m_prkServer);
        if (!bIsReconnect)
        {
            m_bSubFlag = false;
        }
    }
    return MQ_ERR_NO;
}
//删除本receiver
s32 CMqKafkaReceiver::UnInit()
{
    if (!m_bInit)
    {
        //GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::SendMessageToMQ error: uninit !\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    DisConnect();
    IRdKafka::_topic_partition_list_destroy(m_prkTopicList);
    m_prkTopicList = NULL;
    m_bInit = FALSE;
    return MQ_ERR_NO;
}
//使用参数中的topic_partition_list替换现有的topic_partition_list
BOOL32 CMqKafkaReceiver::SetTopicPartitionList(rd_kafka_topic_partition_list_t *partitions)
{
    //注：是替换而不是增加
    m_vecTpList.clear();
    return TRUE;
    for (int idx = 0; idx < partitions->cnt; ++idx)
    {
        TMqKafkaConsumerTPInfo ttpinfo;
        strcpy(ttpinfo.topic, partitions->elems[idx].topic);
        ttpinfo.partition = partitions->elems[idx].partition;
        ttpinfo.offset = partitions->elems[idx].offset;
        m_vecTpList.push_back(ttpinfo);
    }
}

//连接函数单独拿出来，方便服务器断链之后的重连
s32 CMqKafkaReceiver::Connect(s32 nTimeout)
{
    //信息校验
    if (!m_bInit)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Connect error: class not inited!\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    if (m_bConnect)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaSender::Connect error: receiver has connected!\n");
        return MQ_ERR_WRONG_LOGIC;
    }
    if (0 == m_nReceiverId || NULL == m_pcMqServer || NULL == m_pEventCb || 0 == strlen(m_szGroup))
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Connect error: param invalid! senderid:%d,pcserver:%X,szgroup:%s,pEventCb:%X\n",
            m_nReceiverId, m_pcMqServer, m_szGroup, m_pEventCb);
        return MQ_ERR_INVALID_PARAM;
    }
    //如果之前已经连结果，清理之前的数据连接
    if(m_prkServer)
    {
        IRdKafka::_unsubscribe(m_prkServer);
        IRdKafka::_consumer_close(m_prkServer);
        IRdKafka::_destroy(m_prkServer);
        m_prkServer = NULL;
    }
    //根据服务器状态决定要不要继续连接
    if (m_pcMqServer->GetState() == SERVER_OFFLINE)
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Connect:ERR,server not online!\n");
        return MQ_ERR_SERVER_DOWN;
    }
    //复制一份配置信息，原信息用于重连
    rd_kafka_conf_t *conf = IRdKafka::_conf_dup(m_prkConf);
    char errstr[512] = {0};
    if (!(m_prkServer = IRdKafka::_new(RD_KAFKA_CONSUMER, conf,        //
        errstr, sizeof(errstr)))) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Connect:ERR,create kafka producer failed:%s\n",
            errstr);
        return MQ_ERR_SDK;
    }
    //执行rd_kafka_new会导致 conf指向的对象被销毁，保险起见，将conf指针置空
    conf = NULL;
    //添加服务器地址串
    std::string strBrokers = m_pcMqServer->GetBrokerStr();
    if (strBrokers.empty() || IRdKafka::_brokers_add(m_prkServer, strBrokers.c_str()) == 0) 
    {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Connect:ERR,No valid brokers specified,brokers:%s\n"
            , strBrokers.c_str());
        IRdKafka::_destroy(m_prkServer);
        m_prkServer = NULL;
        return MQ_ERR_INVALID_PARAM;
    }
    //设置kafka内部默认日志等级
    IRdKafka::_set_log_level(m_prkServer, 3);
    /* Create topic */
    //创建发送句柄
    /* Redirect rd_kafka_poll() to consumer_poll() */
    IRdKafka::_poll_set_consumer(m_prkServer);
    //具体连接服务器挪到subscribe函数中，防止提前接收到不需要的数据
    
    GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaReceiver::Connect success\n");
    m_bConnect = TRUE;
    return MQ_ERR_NO;
}
BOOL32 CMqKafkaReceiver::DisConnect(bool bIsReconnect)
{
    //如果正在订阅状态，需要手动取消订阅，否则将会出现野线程
    if (m_bOnSubscribe && MQ_ERR_NO != StopReceive(bIsReconnect))
    {
        GetManager()->PrintLog(MQ_LOG_DBG_LEVEL, "CMqKafkaReceiver::DisConnect StopReceive failed!\n");
        return FALSE;
    }
    if(m_prkServer)
    {
        IRdKafka::_consumer_close(m_prkServer);
        IRdKafka::_destroy(m_prkServer);
        m_prkServer = NULL;
    }
    m_bConnect = FALSE;
    return MQ_ERR_NO;
}
//服务器断链事件
void CMqKafkaReceiver::OnDisConnect() 
{
//     if(m_pEventCb)
//     {
//         //typedef void (*MQEventCallBack)(MQTOPICHANDLE handle, s32 nErrno, s8* pszErr, s32 nErrLen, KD_PTR pUser);
//         m_pEventCb(m_nReceiverId, MQ_ERR_SERVER_DOWN, MQGetErrDesc(MQ_ERR_SERVER_DOWN), strlen(MQGetErrDesc(MQ_ERR_SERVER_DOWN)), m_pEventUser);
//     }
    //必须通知服务器断链消息，否则重连会出问题
    m_pcMqServer->OnDisConnect();
}
//服务器消息事件处理函数
void CMqKafkaReceiver::OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen)
{
    if(m_pEventCb)
    {
        if (MQ_KAFKA_MSG_EVENT == eMsgType)
        {
            //MQ_KAFKA_MSG_EVENT不用转换，直接触发
            m_pEventCb(m_nReceiverId, nErrNo, MQGetErrDesc(nErrNo), strlen(MQGetErrDesc(nErrNo)), m_pEventUser );
        }
        else if(MQ_KAFKA_MSG_ERROR == eMsgType)
        {
            //TODO kafka消息转换为mqkafka对外消息
        }
        //typedef void (*MQEventCallBack)(MQTOPICHANDLE handle, s32 nErrno, s8* pszErr, s32 nErrLen, KD_PTR pUser);

    }
}

void CMqKafkaReceiver::OnMessage(rd_kafka_message_t *msg)
{
    std::string strTopicName = IRdKafka::_topic_name(msg->rkt);
    PushTopic(strTopicName, msg->rkt);
#ifndef NDEBUG
#ifdef _WIN32
# ifndef snprintf
# define snprintf _snprintf
# endif
#endif
    //NDEBUG宏在win和linux下都存在，但_DEBUG仅限对应平台
    //计算时间戳
    char szMqTime[50] = {0};
    timeb tcurrent;
    ftime(&tcurrent);
    tm *pTM =localtime(&tcurrent.time);
    snprintf(szMqTime, sizeof(szMqTime) - 1, "[%d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d.%0.3d] msgcb:%p ",
        pTM->tm_year+1900, pTM->tm_mon+1, pTM->tm_mday,
        pTM->tm_hour, pTM->tm_min, pTM->tm_sec, tcurrent.millitm, m_pMsgCb);
    std::string strMqLog("MqKafkaReceiver::OnMessage");
    strMqLog.append(szMqTime);
    if (NULL != msg)
    {
        strMqLog.append(" topic:").append( strTopicName);
        if (NULL != msg->key)
        {
            strMqLog.append(" key:").append((const char*)msg->key, msg->key_len);
        }
        if (NULL != msg->payload)
        {
            strMqLog.append(" value:").append((const char*)msg->payload,msg->len);
        }
    }
    strMqLog.append("\n");
    printf(strMqLog.c_str());
#endif
    if (NULL != msg && m_bOnSubscribe)//为了避免上层业务出现内存错误，只回调明确正在订阅的数据
    {
        TMQMessage tMessage;
        strcpy(tMessage.szTopic, strTopicName.c_str());
        tMessage.nKeyLen = msg->key_len;
        tMessage.nValueLen = msg->len;
        rd_kafka_timestamp_type_t tstype;
        int64_t timestamp;
        timestamp = IRdKafka::_message_timestamp(msg, &tstype);
        if (tstype != RD_KAFKA_TIMESTAMP_NOT_AVAILABLE) 
        {
            //                 if (tstype == RD_KAFKA_TIMESTAMP_CREATE_TIME);   //create time
            //                 else if (tstype == RD_KAFKA_TIMESTAMP_LOG_APPEND_TIME);  //log append time
            tMessage.timestamp.time = (time_t)(timestamp/1000);
            tMessage.timestamp.millitm = timestamp%1000;
        }
        if (0 != tMessage.nKeyLen)
        {
            tMessage.pszKey = (u8*)msg->key;
        }
        tMessage.pszValue = (u8*)msg->payload;
        if (m_bPrecise)
        {//无论是否自动提交(commit),只要是精密接收器，就通过精密回调上报数据
            if (NULL != m_pPreciseMsgCb)
            {
                if(m_pPreciseMsgCb(m_nReceiverId, &tMessage, msg->partition, msg->offset, m_pMsgUser) )
                {
                    //返回值非0 ，seek至当前位置
                    IRdKafka::_seek(msg->rkt, msg->partition, msg->offset, 0);
                    GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::OnMessage PreciseMsgCb return error,msg seek to %u\n", (uint32_t)msg->offset);
                    OnMessageCb(MQ_KAFKA_MSG_EVENT, MQ_ERR_RECEIVER_MSG_EXCEPTION, 0, (void*)MQGetErrDesc(MQ_ERR_RECEIVER_MSG_EXCEPTION), strlen(MQGetErrDesc(MQ_ERR_RECEIVER_MSG_EXCEPTION)));
                }
            }
        }
        else
        {
            if (NULL != m_pMsgCb) 
            {
                m_pMsgCb(m_nReceiverId, &tMessage, m_pMsgUser);
            }
        }
    }
}

s32 CMqKafkaReceiver::Commit(const char* topic, int partition, long offset, BOOL32 async)
{
    rd_kafka_topic_partition_list_t *offsets;
    rd_kafka_topic_partition_t *rktpar;
    rd_kafka_resp_err_t err;
    offsets = IRdKafka::_topic_partition_list_new(1);
    rktpar = IRdKafka::_topic_partition_list_add(
        offsets, topic,
        partition);
    rktpar->offset = offset+1;      //+1是因为rd_kafka_commit_message也进行了+1操作，本质是通知服务器从当前消息的下一条开始消费
    err = IRdKafka::_commit(m_prkServer, offsets, async);
    IRdKafka::_topic_partition_list_destroy(offsets);
    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::MQReceiverCommit:ERR, commit failed: %s\n",
            IRdKafka::_err2str(err));
        s32 nErrorCode = MQ_ERR_NO;
        switch (err)
        {
        case RD_KAFKA_RESP_ERR__TIMED_OUT:
            nErrorCode = MQ_ERR_TIMEOUT; break;
        case RD_KAFKA_RESP_ERR__DESTROY:        //对象已失效
            nErrorCode = MQ_ERR_SDK;break;
        case RD_KAFKA_RESP_ERR__UNKNOWN_GROUP:
            nErrorCode = MQ_ERR_INVALID_PARAM;break;
        default:
            nErrorCode = MQ_ERR_SDK;
        }
        return nErrorCode;
    }
    return MQ_ERR_NO;
}
s32 CMqKafkaReceiver::Seek(const char* topic, int partition, long offset, s32 nTimeout)
{
    rd_kafka_topic_t *rk_t = GetTopicByName(topic);
    if (NULL == rk_t)
    {
        return MQ_ERR_INVALID_PARAM;
    }
    rd_kafka_resp_err_t err;
    err = IRdKafka::_seek(rk_t, partition, offset, nTimeout);
    if (err != RD_KAFKA_RESP_ERR_NO_ERROR) {
        GetManager()->PrintLog(MQ_LOG_ERR_LEVEL, "CMqKafkaReceiver::Seek:ERR, seek failed: %s\n",
            IRdKafka::_err2str(err));
        s32 nErrorCode = MQ_ERR_NO;
        switch (err)
        {
        case RD_KAFKA_RESP_ERR__TIMED_OUT:
            nErrorCode = MQ_ERR_TIMEOUT; break;
        case RD_KAFKA_RESP_ERR__DESTROY:        //对象已失效
            nErrorCode = MQ_ERR_SDK;break;
        case RD_KAFKA_RESP_ERR__UNKNOWN_GROUP:
            nErrorCode = MQ_ERR_INVALID_PARAM;break;
        default:
            nErrorCode = MQ_ERR_SDK;
        }
        return nErrorCode;
    }
    return MQ_ERR_NO;
}
void CMqKafkaReceiver::OnCommitCb(rd_kafka_resp_err_t err, rd_kafka_topic_partition_list_t *c_offsets)
{
    if (NULL != m_pCommitCb && c_offsets->cnt > 0)
    {
        s32 nErrCode = MQ_ERR_NO;
        if (RD_KAFKA_RESP_ERR_NO_ERROR != err)
        {
            nErrCode = MQ_ERR_SDK;
        }
        //commit操作时offset做了加一操作，回调时再减回去
        m_pCommitCb(m_nReceiverId, c_offsets->elems[0].topic, c_offsets->elems[0].partition, c_offsets->elems[0].offset - 1, 
            nErrCode, m_pMsgUser);
    }
    
}

void CMqKafkaReceiver::PushTopic(std::string name, rd_kafka_topic_t* rk_t)
{
    std::map<std::string, rd_kafka_topic_t*>::iterator iteTopic = m_mMsgTopics.find(name);
    if (iteTopic == m_mMsgTopics.end())
    {
        m_mMsgTopics.insert(std::make_pair(name,rk_t));
    }
    else if(iteTopic->second != rk_t)
    {
        iteTopic->second = rk_t;
    }

}
rd_kafka_topic_t* CMqKafkaReceiver::GetTopicByName(const char* pszTopic)
{
    std::map<std::string, rd_kafka_topic_t*>::iterator iteTopic = m_mMsgTopics.find(std::string(pszTopic));
    if (iteTopic == m_mMsgTopics.end())
    {
        return NULL;
    }
    else
    {
        return iteTopic->second;
    }
}


