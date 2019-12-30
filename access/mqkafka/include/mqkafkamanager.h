#ifndef _MQKAFKAMANAGER_H_
#define _MQKAFKAMANAGER_H_
#include "mqkafkadefine.h"
#include "threadlock.h"
#include <vector>
#include "indexgenerater.h"
#define SERVERHANDLE(_TopicHandle)  ((MQHANDLE)((((u32)(_TopicHandle)) >> 16) & 0xffff))
#define MAX_LOG_LEN     10240
class CMqKafkaServer;
class CMqKafkaManager
{
public:
    CMqKafkaManager();
    ~CMqKafkaManager();
    static CMqKafkaManager* instance();  // ����CMqKafkaManagerΨһ��ʵ��
    static void destroy();
    BOOL32  IsInit(){return m_bInit;}
    s32     Init(MQLogCallBack pLogCallBack, MQServerDownCallBack pServerDownCallBack, void* pUser);
    MQHANDLE LoginKafkaServer(PTMQServerInfo ptServerArr, u32 nServerAddrNum, LPCSTR szUsername , LPCSTR szPassword, s32 nTimeout);
    SmartPtr<CMqKafkaServer> FindServer(MQHANDLE hServerHandle);
    //�������ӵ�ָ�������������ӳ�
    s32 LogoutKafkaServer(MQHANDLE hServerHandle);
    //��ȡĿ��״̬
    eServerState GetMQState(MQHANDLE hServerHandle);
    //��ȡĿ������topic
    s32 GetAllTopics(MQHANDLE hServerHandle, s32 nTimeout, PTMQTopicInfo pTopicArr, u32 &nTopicCnt);
    //���������¼�
    void OnServerDown(SmartPtr<CMqKafkaServer> pcServer);
    void PrintLog(eMQLoglev eLev, const s8* const szFormat, ... );

    void PrintServerData(MQHANDLE hServerHandle);

    BOOL32 GetStopFlag(){return m_bStopFlag;}
public:
    //����������Ϊ������������Ϊ��Ϣ��ȡ�߳�Ҫʹ�������
    CThreadLockMutex m_clmLock;       
    std::vector<SmartPtr<CMqKafkaServer> > m_vecpServer;
private:
    //����server��Ψһid
    MQHANDLE CreateServerId(void);
    void DeleteServerId(MQHANDLE nTopicId);
private:
    static CMqKafkaManager* s_pCManagerInst;
private:
    MQLogCallBack m_pLogCB;
    MQServerDownCallBack m_pDownCB;
    void*        m_pUsr;
    BOOL32 m_bInit;

    CIndexGenerater m_cIndexGenerater;
    pthread_t     m_tSenderPoolThread;    //����poll�����߳̾��
    BOOL32        m_bStopFlag;            //�����ж��Ƿ���Ҫ��ֹ�̵߳ı�־λ
};
CMqKafkaManager* GetManager();
#endif  //_MQKAFKAMANAGER_H_