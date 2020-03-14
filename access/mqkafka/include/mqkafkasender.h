#ifndef _MQKAFKASENDER_H_
#define _MQKAFKASENDER_H_
#include "mqkafkadefine.h"
#include "mqinterface.h"
#define MQ_KAFKA_PRODUCER_CLIENT_ID "mqkafka-producer"
class CMqKafkaServer;
class CMqKafkaSender : public CMqKafkaBase
{
public:
    CMqKafkaSender();
    ~CMqKafkaSender();
    //创建并初始化sender
    s32 Init(MQTOPICHANDLE nSenderId, CMqKafkaServer *pcMqServer, const s8* szTopic, TMQSenderParam &tSendParam, s32 nTimeout);
    //发送消息
    s32 SendMessageToMQ(const u8* szValue, s32 nValueLen, const u8* szKey, s32 nKeyLen, bool bSync, bool bType, s32 nTimeout);
    //删除本sender
    s32 UnInit();

    //连接函数单独拿出来，方便服务器断链之后的重连
    s32 Connect(s32 nTimeout);
    BOOL32 DisConnect();
    //服务器断链事件
    virtual void OnDisConnect() ;
    //服务器消息事件处理函数
    virtual void OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen);
    const MQTOPICHANDLE GetSenderId(){ return m_nSenderId;}
private:
    BOOL32 m_bInit;
    BOOL32 m_bConnect;          //是否成功连接服务器
    MQTOPICHANDLE    m_nSenderId;
    s8     m_szTopic[MAX_NAME_LEN];
    CMqKafkaServer  *m_pcMqServer;       //当前sender所属的mqkafkaserver类(服务器)指针
    MQEventCallBack  m_pEventCb;     //事件回调
    void*           m_pEventUser;       //回调时回传的用户数据
    s32 m_nMaxQueueNum;               //异步发送时消息队列的大小，默认无需更改
    s32 m_nMaxQueueRetainTime;        //异步发送时队列的最大缓存时间，默认无需修改
    rd_kafka_topic_t *m_prkTopic;     //rdkafka使用的topic句柄
    int              m_nMsgState;     //用于dr_cb的状态回传，使用成员变量是为了防止地址腐败
};
#endif  //_MQKAFKASENDER_H_