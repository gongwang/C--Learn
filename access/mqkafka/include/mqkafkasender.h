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
    //��������ʼ��sender
    s32 Init(MQTOPICHANDLE nSenderId, CMqKafkaServer *pcMqServer, const s8* szTopic, TMQSenderParam &tSendParam, s32 nTimeout);
    //������Ϣ
    s32 SendMessageToMQ(const u8* szValue, s32 nValueLen, const u8* szKey, s32 nKeyLen, bool bSync, bool bType, s32 nTimeout);
    //ɾ����sender
    s32 UnInit();

    //���Ӻ��������ó������������������֮�������
    s32 Connect(s32 nTimeout);
    BOOL32 DisConnect();
    //�����������¼�
    virtual void OnDisConnect() ;
    //��������Ϣ�¼�������
    virtual void OnMessageCb(eMqKafkaMsg eMsgType, int nErrNo, int nMsgParam, void* pMsgVal, int nMsgLen);
    const MQTOPICHANDLE GetSenderId(){ return m_nSenderId;}
private:
    BOOL32 m_bInit;
    BOOL32 m_bConnect;          //�Ƿ�ɹ����ӷ�����
    MQTOPICHANDLE    m_nSenderId;
    s8     m_szTopic[MAX_NAME_LEN];
    CMqKafkaServer  *m_pcMqServer;       //��ǰsender������mqkafkaserver��(������)ָ��
    MQEventCallBack  m_pEventCb;     //�¼��ص�
    void*           m_pEventUser;       //�ص�ʱ�ش����û�����
    s32 m_nMaxQueueNum;               //�첽����ʱ��Ϣ���еĴ�С��Ĭ���������
    s32 m_nMaxQueueRetainTime;        //�첽����ʱ���е���󻺴�ʱ�䣬Ĭ�������޸�
    rd_kafka_topic_t *m_prkTopic;     //rdkafkaʹ�õ�topic���
    int              m_nMsgState;     //����dr_cb��״̬�ش���ʹ�ó�Ա������Ϊ�˷�ֹ��ַ����
};
#endif  //_MQKAFKASENDER_H_