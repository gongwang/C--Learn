#ifndef _MQKAFKARECEIVER_H_
#define _MQKAFKARECEIVER_H_
#include <vector>
#include <map>
#include <string>
#include "mqkafkadefine.h"
#include "mqinterface.h"
#define MQ_KAFKA_CONSUMER_CLIENT_ID "mqkafka-consumer"
//consumerʹ�ã�����topic_partition��Ϣ�Ľṹ�壬������¼������û�н������
typedef struct tagMqKafkaConsumerTPInfo
{
    char         topic[MAX_NAME_LEN];             /**< Topic name */
    int32_t      partition;         /**< Partition */
    int64_t      offset;            /**< Offset */
    bool         bEof;              /**< �Ƿ񵽴���Ϣĩβ*/
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
    //��������ʼ��
    s32 Init(MQTOPICHANDLE nReceiverId, CMqKafkaServer *pcMqServer, PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQReceiverParam &tReceiverParam, s32 nTimeout);
    s32 Init(MQTOPICHANDLE nReceiverId, CMqKafkaServer *pcMqServer, PTMQTopicInfo pTopicArr, u32 nTopicNum, TMQPreciseReceiverParam &tReceiverParam, s32 nTimeout);
    s32 GetReceiverInfo(TMQReceiverInfo &tReceiverInfo);
    s32 StartReveive(s32 nOffset = 0);
    //�����Ƿ�������־λ����������Ҫ��Ҫ����m_bSubFlag״̬
    s32 StopReceive(bool bIsReconnect = false);
    //ɾ����receiver
    s32 UnInit();
    //ʹ�ò����е�topic_partition_list�滻���е�topic_partition_list
    BOOL32 SetTopicPartitionList(rd_kafka_topic_partition_list_t *partitions);
    //���topic_partition_list
    BOOL32 ResetTopicPartitionList(){ m_vecTpList.clear(); return TRUE;}
    //���յ���Ϣ֮����д���
    void OnMessage(rd_kafka_message_t *msg);

    //���Ӻ��������ó������������������֮�������
    s32 Connect(s32 nTimeout);
    BOOL32 DisConnect(bool bIsReconnect = false);
    //�����������¼�
    virtual void OnDisConnect() ;
    //��������Ϣ�¼�������
    virtual void OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen);
    const MQTOPICHANDLE GetReceiverId(){ return m_nReceiverId;}
    //��������ύ��ǰ�������Ľ���ƫ�Ƶ�ַ
    s32 Commit(const char* topic, int partition, long offset, BOOL32 async);
    s32 Seek(const char* topic, int partition, long offset, s32 nTimeout);
    void OnCommitCb(rd_kafka_resp_err_t err, rd_kafka_topic_partition_list_t *c_offsets);
private:
    void PushTopic(std::string name, rd_kafka_topic_t* rk_t);
    rd_kafka_topic_t* GetTopicByName(const char* pszTopic);
public:
    bool             m_bOnSubscribe;        //��ǰ�Ƿ�����Ϣ����״̬
    bool             m_bSubFlag;            //�Ƿ�������Ϣ���ģ�����һ����������������û�̬����һ�����̶߳���״̬
    s32    m_nMsgTimeout;                //��Ϣ��ȡʱ�ĳ�ʱʱ��
    s32              m_nOffset;          //��Ϣ����ʼƫ�ƣ����ڶ��ĳɹ�������ƫ�Ƶ�ַ
private:
    BOOL32 m_bInit;
    BOOL32 m_bConnect;          //�Ƿ�ɹ����ӷ�����
    MQTOPICHANDLE    m_nReceiverId;
    s8     m_szGroup[MAX_NAME_LEN*2];       //groupid����Ϣ������group
    ConsumerTPList   m_vecTpList;        //��¼��ע�ᵽ��ǰconsumer�ķ�����Ϣ������ͳ�ƶ��ٸ�������Ϣȫ���������
    CMqKafkaServer  *m_pcMqServer;       //��ǰsender������mqkafkaserver��(������)ָ��
    MQEventCallBack  m_pEventCb;     //�¼��ص�
    void*           m_pEventUser;       //�ص�ʱ�ش����û�����
    MQMessageCallBack m_pMsgCb;       //���ݻص�
    void*           m_pMsgUser;      //���ݻص����û�����
    MQPreciseMessageCallBack m_pPreciseMsgCb;   //��ƫ���������ݻص�
    MQCommitCallBack        m_pCommitCb;        //ƫ�����ύ����ص�
    BOOL32          m_bAutoCommit;      //��Ϣƫ���Ƿ��Զ��ύ�ı�־λ
    BOOL32          m_bPrecise ;        //�Ƿ��Ǿ��ܽ�����(PreciseReceiver)
    pthread_t        m_tSubScribeThread;    //���涩��poll�����߳̾��
    rd_kafka_topic_partition_list_t *m_prkTopicList;     //rdkafkaʹ�õ�topics���
    //���ж��ĵ�topics��Ϣ����ͨ��m_prkTopics�±�ֱ�ӷ���
    //��map������Ϣ�ϱ���rd_kafka_topic_t�����topic�Ķ�Ӧ��ϵ����Ҫ����seek�Ŀ��ٶ�λ
    std::map<std::string, rd_kafka_topic_t*> m_mMsgTopics;
};

#endif  //_MQKAFKARECEIVER_H_