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
    static CMqKafkaManager* instance();  // 返回CMqKafkaManager唯一的实例
    static void destroy();
    BOOL32  IsInit(){return m_bInit;}
    s32     Init(MQLogCallBack pLogCallBack, MQServerDownCallBack pServerDownCallBack, void* pUser);
    MQHANDLE LoginKafkaServer(PTMQServerInfo ptServerArr, u32 nServerAddrNum, LPCSTR szUsername , LPCSTR szPassword, s32 nTimeout);
    SmartPtr<CMqKafkaServer> FindServer(MQHANDLE hServerHandle);
    //销毁连接到指定服务器的连接池
    s32 LogoutKafkaServer(MQHANDLE hServerHandle);
    //获取目标状态
    eServerState GetMQState(MQHANDLE hServerHandle);
    //获取目标所有topic
    s32 GetAllTopics(MQHANDLE hServerHandle, s32 nTimeout, PTMQTopicInfo pTopicArr, u32 &nTopicCnt);
    //触发断链事件
    void OnServerDown(SmartPtr<CMqKafkaServer> pcServer);
    void PrintLog(eMQLoglev eLev, const s8* const szFormat, ... );

    void PrintServerData(MQHANDLE hServerHandle);

    BOOL32 GetStopFlag(){return m_bStopFlag;}
public:
    //互斥锁设置为公共变量是因为消息拉取线程要使用这个锁
    CThreadLockMutex m_clmLock;       
    std::vector<SmartPtr<CMqKafkaServer> > m_vecpServer;
private:
    //创建server的唯一id
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
    pthread_t     m_tSenderPoolThread;    //保存poll数据线程句柄
    BOOL32        m_bStopFlag;            //用来判断是否需要终止线程的标志位
};
CMqKafkaManager* GetManager();
#endif  //_MQKAFKAMANAGER_H_