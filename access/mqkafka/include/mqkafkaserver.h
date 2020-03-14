#ifndef _MQKAFKASERVER_H_
#define _MQKAFKASERVER_H_
#include "mqkafkadefine.h"
#include "indexgenerater.h"
#include "threadlock.h"

#define MQ_KAFKA_SERVER_DEF_CLIENT_ID "mqkafkaserver"
typedef struct tagMQServerInfo *PTMQServerInfo;
class CMqKafkaSender;
class CMqKafkaReceiver;
class CMqKafkaServer : public CMqKafkaBase
{
public:
    CMqKafkaServer();
    ~CMqKafkaServer();
    const MQHANDLE GetServerId(){ return m_nServerId;}
    BOOL32 LoginServer(MQHANDLE nServerId, PTMQServerInfo ptServerArr, u32 nServerAddrNum, const s8* szUsername , const s8* szPassword, s32 nTimeout);
    //创建和删除发送器
    MQTOPICHANDLE CreateSender(const s8* pszTopic, TMQSenderParam &tSendParam, s32 nTimeout);
    SmartPtr<CMqKafkaSender> FindSender(MQTOPICHANDLE handle);
    s32 DeleteSender(MQTOPICHANDLE handle);
    void PollSenderMsg();
    //创建和删除接收器
    MQTOPICHANDLE CreateReceiver(PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQReceiverParam &tReceiverParam, s32 nTimeout);
    MQTOPICHANDLE CreatePreciseReceiver(PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQPreciseReceiverParam &tReceiverParam, s32 nTimeout);
    SmartPtr<CMqKafkaReceiver> FindReceiver(MQTOPICHANDLE handle);
    s32 DeleteReceiver(MQTOPICHANDLE handle);
    //获取目标状态
    eServerState GetState();
    //获取kafkaserver详细信息
    BOOL32 GetMetadata(const rd_kafka_metadata_t* &prdMetadata);
    //销毁连接到指定服务器的连接池
    BOOL32 Logout();
    //断链重连
    BOOL32 ReConnect(s32 nTimeout);
    //获取服务器链接地址字符串
    std::string GetBrokerStr();
    //
    rd_kafka_t* GetRKServer();

    //服务器断链事件
    virtual void OnDisConnect() ;
    //服务器消息事件处理函数
    virtual void OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen);
    //receiver创建groupname时使用
    s32 GetReceiverCount(){return m_vecpReceiver.size();}
private:
    //创建sender和recever的唯一id
    MQTOPICHANDLE CreateTopicId();
    void DeleteTopicId(MQTOPICHANDLE nTopicId);
    //断链重连
    BOOL32 Connect(s32 nTimeout);
private:
    eServerState m_bState;
    MQHANDLE    m_nServerId;
    std::string m_strBrokers;       //拼接好的服务器连接字符串，断链重连使用
    const rd_kafka_metadata_t *m_prkMetadata;
    std::vector<SmartPtr<CMqKafkaSender> > m_vecpSender;
    std::vector<SmartPtr<CMqKafkaReceiver> > m_vecpReceiver;
    CThreadLockMutex m_clmLock;
    CIndexGenerater m_cIndexGenerater;
};
//consumer和producer通用回掉函数，统一放置在这里
//kafka日志回调，level为日志等级共1-7,7个等级
//只有低于设置等级的日志才能回调出来，如设置等级为4，则只有1-4能打印出来
/* kafka内部等级：
#define LOG_EMERG   0
#define LOG_ALERT   1
#define LOG_CRIT    2
#define LOG_ERR     3
#define LOG_WARNING 4
#define LOG_NOTICE  5
#define LOG_INFO    6
#define LOG_DEBUG   7
fac为产生日志的对象类型，如thread、broker等
*/
extern void common_kafka_log_cb (const rd_kafka_t *rk, int level,  const char *fac, const char *buf) ;

//错误回调，暂未想到如何使用，直接打印日志 error_cb_trampoline
extern void common_kafka_err_cb (rd_kafka_t *rk, int err, const char *reason, void *opaque) ;

//码流等级的回调？throttle_cb_trampoline 
extern void common_kafka_throttle_cb(rd_kafka_t *rk, const char *broker_name, int32_t broker_id, int throttle_time_ms, void *opaque) ;
//kafka状态信息回调 stats_cb_trampoline 
extern int common_kafka_stats_cb(rd_kafka_t *rk, char *json, size_t json_len, void *opaque) ;
#endif  //_MQKAFKASERVER_H_