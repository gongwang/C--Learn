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
    //������ɾ��������
    MQTOPICHANDLE CreateSender(const s8* pszTopic, TMQSenderParam &tSendParam, s32 nTimeout);
    SmartPtr<CMqKafkaSender> FindSender(MQTOPICHANDLE handle);
    s32 DeleteSender(MQTOPICHANDLE handle);
    void PollSenderMsg();
    //������ɾ��������
    MQTOPICHANDLE CreateReceiver(PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQReceiverParam &tReceiverParam, s32 nTimeout);
    MQTOPICHANDLE CreatePreciseReceiver(PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQPreciseReceiverParam &tReceiverParam, s32 nTimeout);
    SmartPtr<CMqKafkaReceiver> FindReceiver(MQTOPICHANDLE handle);
    s32 DeleteReceiver(MQTOPICHANDLE handle);
    //��ȡĿ��״̬
    eServerState GetState();
    //��ȡkafkaserver��ϸ��Ϣ
    BOOL32 GetMetadata(const rd_kafka_metadata_t* &prdMetadata);
    //�������ӵ�ָ�������������ӳ�
    BOOL32 Logout();
    //��������
    BOOL32 ReConnect(s32 nTimeout);
    //��ȡ���������ӵ�ַ�ַ���
    std::string GetBrokerStr();
    //
    rd_kafka_t* GetRKServer();

    //�����������¼�
    virtual void OnDisConnect() ;
    //��������Ϣ�¼�������
    virtual void OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen);
    //receiver����groupnameʱʹ��
    s32 GetReceiverCount(){return m_vecpReceiver.size();}
private:
    //����sender��recever��Ψһid
    MQTOPICHANDLE CreateTopicId();
    void DeleteTopicId(MQTOPICHANDLE nTopicId);
    //��������
    BOOL32 Connect(s32 nTimeout);
private:
    eServerState m_bState;
    MQHANDLE    m_nServerId;
    std::string m_strBrokers;       //ƴ�Ӻõķ����������ַ�������������ʹ��
    const rd_kafka_metadata_t *m_prkMetadata;
    std::vector<SmartPtr<CMqKafkaSender> > m_vecpSender;
    std::vector<SmartPtr<CMqKafkaReceiver> > m_vecpReceiver;
    CThreadLockMutex m_clmLock;
    CIndexGenerater m_cIndexGenerater;
};
//consumer��producerͨ�ûص�������ͳһ����������
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
extern void common_kafka_log_cb (const rd_kafka_t *rk, int level,  const char *fac, const char *buf) ;

//����ص�����δ�뵽���ʹ�ã�ֱ�Ӵ�ӡ��־ error_cb_trampoline
extern void common_kafka_err_cb (rd_kafka_t *rk, int err, const char *reason, void *opaque) ;

//�����ȼ��Ļص���throttle_cb_trampoline 
extern void common_kafka_throttle_cb(rd_kafka_t *rk, const char *broker_name, int32_t broker_id, int throttle_time_ms, void *opaque) ;
//kafka״̬��Ϣ�ص� stats_cb_trampoline 
extern int common_kafka_stats_cb(rd_kafka_t *rk, char *json, size_t json_len, void *opaque) ;
#endif  //_MQKAFKASERVER_H_