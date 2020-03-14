#ifndef _MQKAFKARECEIVER_H_
#define _MQKAFKARECEIVER_H_
#include <vector>
#include <map>
#include <string>
#include "mqkafkadefine.h"
#include "mqinterface.h"
#define MQ_KAFKA_CONSUMER_CLIENT_ID "mqkafka-consumer"
//consumer使用，保存topic_partition信息的结构体，用来记录数据有没有接收完毕
typedef struct tagMqKafkaConsumerTPInfo
{
    char         topic[MAX_NAME_LEN];             /**< Topic name */
    int32_t      partition;         /**< Partition */
    int64_t      offset;            /**< Offset */
    bool         bEof;              /**< 是否到达消息末尾*/
    tagMqKafkaConsumerTPInfo()
    {
        memset(topic, 0, sizeof(topic));
        partition = 0;
        offset = 0;
        bEof = false;
    }
}TMqKafkaConsumerTPInfo,*PTMqKafkaConsumerTPInfo;
typedef std::vector<TMqKafkaConsumerTPInfo> ConsumerTPList;
class CMqKafkaServer;
class CMqKafkaReceiver : public CMqKafkaBase
{
public:
    CMqKafkaReceiver();
    ~CMqKafkaReceiver();
    //创建并初始化
    s32 Init(MQTOPICHANDLE nReceiverId, CMqKafkaServer *pcMqServer, PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQReceiverParam &tReceiverParam, s32 nTimeout);
    s32 Init(MQTOPICHANDLE nReceiverId, CMqKafkaServer *pcMqServer, PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQPreciseReceiverParam &tReceiverParam, s32 nTimeout);
    s32 GetReceiverInfo(TMQReceiverInfo &tReceiverInfo);
    s32 StartReveive(s32 nOffset = 0);
    //增加是否重连标志位，用来决定要不要重置m_bSubFlag状态
    s32 StopReceive(bool bIsReconnect = false);
    //删除本receiver
    s32 UnInit();
    //使用参数中的topic_partition_list替换现有的topic_partition_list
    BOOL32 SetTopicPartitionList(rd_kafka_topic_partition_list_t *partitions);
    //清空topic_partition_list
    BOOL32 ResetTopicPartitionList(){ m_vecTpList.clear(); return TRUE;}
    //接收到消息之后进行处理
    void OnMessage(rd_kafka_message_t *msg);

    //连接函数单独拿出来，方便服务器断链之后的重连
    s32 Connect(s32 nTimeout);
    BOOL32 DisConnect(bool bIsReconnect = false);
    //服务器断链事件
    virtual void OnDisConnect() ;
    //服务器消息事件处理函数
    virtual void OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen);
    const MQTOPICHANDLE GetReceiverId(){ return m_nReceiverId;}
    //向服务器提交当前接收器的接收偏移地址
    s32 Commit(const char* topic, int partition, long offset, BOOL32 async);
    s32 Seek(const char* topic, int partition, long offset, s32 nTimeout);
    void OnCommitCb(rd_kafka_resp_err_t err, rd_kafka_topic_partition_list_t *c_offsets);
private:
    void PushTopic(std::string name, rd_kafka_topic_t* rk_t);
    rd_kafka_topic_t* GetTopicByName(const char* pszTopic);
public:
    bool             m_bOnSubscribe;        //当前是否处于消息订阅状态
    bool             m_bSubFlag;            //是否开启了消息订阅，和上一个的区别是这个是用户态，上一个是线程订阅状态
    s32    m_nMsgTimeout;                //消息拉取时的超时时间
    s32              m_nOffset;          //消息的起始偏移，用于订阅成功后设置偏移地址
private:
    BOOL32 m_bInit;
    BOOL32 m_bConnect;          //是否成功连接服务器
    MQTOPICHANDLE    m_nReceiverId;
    s8     m_szGroup[MAX_NAME_LEN*2];       //groupid，消息所属的group
    ConsumerTPList   m_vecTpList;        //记录的注册到当前consumer的分区信息，用来统计多少个分区消息全部接收完毕
    CMqKafkaServer  *m_pcMqServer;       //当前sender所属的mqkafkaserver类(服务器)指针
    MQEventCallBack  m_pEventCb;     //事件回调
    void*           m_pEventUser;       //回调时回传的用户数据
    MQMessageCallBack m_pMsgCb;       //数据回调
    void*           m_pMsgUser;      //数据回调的用户数据
    MQPreciseMessageCallBack m_pPreciseMsgCb;   //带偏移量的数据回调
    MQCommitCallBack        m_pCommitCb;        //偏移量提交结果回调
    BOOL32          m_bAutoCommit;      //消息偏移是否自动提交的标志位
    BOOL32          m_bPrecise ;        //是否是精密接收器(PreciseReceiver)
    pthread_t        m_tSubScribeThread;    //保存订阅poll数据线程句柄
    rd_kafka_topic_partition_list_t *m_prkTopicList;     //rdkafka使用的topics句柄
    //所有订阅的topics信息可以通过m_prkTopics下标直接访问
    //用map保存消息上报的rd_kafka_topic_t句柄和topic的对应关系，主要用于seek的快速定位
    std::map<std::string, rd_kafka_topic_t*> m_mMsgTopics;
};

#endif  //_MQKAFKARECEIVER_H_