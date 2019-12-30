#ifndef _REDIS_NODE_H_
#define _REDIS_NODE_H_
#include <vector>
#include "hiredisdef.h"
#include "mcinterface.h"
#include "threadlock.h"
class CMCRedis;
#define REDIS_SLOTS_NUM     16384           //3.0�汾redis��Ⱥ����16384����λ
#define MAX_IP_LEN          50
#define MAX_DBCONN_ID_LEN   64
#define OUT
enum eMCConnectType
{
    REDIS_MASTER = 0,
    REDIS_SLAVE,
};
/****redis���ӽṹ��
һ�����ݿ����ӿ�����i����Ⱥ�ڵ�(Node)��
ÿ��Node���Լ��Ĳ��б�(slots)����һ����ʵ��(master)��j����ʵ��(slave)
ÿ��ʵ�����Լ���ip�˿ںͼ�ȺΨһGUID
****/
typedef struct tagRedisNodeSlotsRange
{
    int slotStart;
    int slotEnd;
    tagRedisNodeSlotsRange()
    {
        slotStart = 0;
        slotEnd = 0;
    }
}TRedisNodeSlotsRange;

typedef struct tagRedisConnInfo
{
    char szIPAddr[MAX_IP_LEN];  //��Ⱥ�ڵ��IP��ַ
    int  nPort;                 //��Ⱥ�ڵ�Ķ˿ں�
    char szConnGUID[MAX_DBCONN_ID_LEN];     //��Ⱥ�ڵ��ΨһID
    redisContext *pRedisCtx;    //ʵ������hiredis��ָ��
    bool bConnected;            //�Ƿ����ӳɹ�
    tagRedisConnInfo()
    {
        memset(this, 0, sizeof(tagRedisConnInfo));
        bConnected = false;
    }
}TRedisConnInfo;

typedef struct tagRedisNodeInfo
{
    TRedisConnInfo                  masterConn; //��Ⱥ���ڵ��������Ϣ
    std::vector<TRedisConnInfo>     slaveConn;  //��Ⱥ�ӽڵ���Ϣ����
    std::vector<TRedisNodeSlotsRange> nodeSlots;//��Ⱥ�ڵ�slots����
}TRedisNodeInfo;

class CRedisNode
{
public:
    CRedisNode();
    ~CRedisNode();
    //
    bool Init(CMCRedis *pcMcRedis, TRedisNodeInfo &tNodeInfo, std::string &strPassword, unsigned int nTimeout = 0);
    BOOL32 RedisConnect();
    BOOL32 RedisReConnect();
    bool RedisDisConnect();
    //slave�ڵ��ֶ�����
    bool RedisReConSlave(TRedisConnInfo &tSlaveConnInfo);

    BOOL32 Ping();
    //
    redisContext * const GetConnection(int nConType);
    unsigned int    HaveSlave() const   { return  m_bHaveSlave; }
    bool            GetConnStatus() const   { return  m_bConnStatus; }
    void            PrintNodeInfo();
    //
    bool CheckSlot(unsigned int nSlot);
    //��ȡ���ݵ�ʵ�ʴ洢���ͣ������MC_TYPE_ERR˵�����ݲ����ڻ��ʽ��֧��
    eMCValueType GetKeyType(const s8* pszKey);
    bool ExecCommand(int nConType,OUT redisReply *&reply, const char* cmd, ...);
    bool AppendCommand( int nConType, const char* cmd, ...);
    bool GetReply(int nConType, OUT redisReply **reply);
private:
    bool Auth(redisContext *pCtx);
    redisContext *ConnectWithTimeout(TRedisConnInfo &tConnInfo);
    redisContext * const GetSlaveConnection();
private:
    // redis connector context
    TRedisNodeInfo  m_tNodeInfo;
    std::string     m_strPassword;          // redis server password
    unsigned int    m_nTimeout;         // connect timeout second
    bool            m_bHaveSlave;          // redis role
    BOOL32          m_bConnStatus;      // redis connection status
    bool            m_bInit;            //
    //printlog param
    CMCRedis       *m_pcMcParent;       //������������ĵ�ַ�����ڴ�ӡ��־��Ϣ
    //������
    CThreadLockMutex      m_clmLock;   
};

#endif //_REDIS_NODE_H_