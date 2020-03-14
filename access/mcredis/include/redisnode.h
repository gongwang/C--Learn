#ifndef _REDIS_NODE_H_
#define _REDIS_NODE_H_
#include <vector>
#include "hiredisdef.h"
#include "mcinterface.h"
#include "threadlock.h"
class CMCRedis;
#define REDIS_SLOTS_NUM     16384           //3.0版本redis集群共有16384个槽位
#define MAX_IP_LEN          50
#define MAX_DBCONN_ID_LEN   64
#define OUT
enum eMCConnectType
{
    REDIS_MASTER = 0,
    REDIS_SLAVE,
};
/****redis连接结构：
一个数据库连接可以有i个集群节点(Node)，
每个Node有自己的槽列表(slots)，有一个主实例(master)和j个从实例(slave)
每个实例有自己的ip端口和集群唯一GUID
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
    char szIPAddr[MAX_IP_LEN];  //集群节点的IP地址
    int  nPort;                 //集群节点的端口号
    char szConnGUID[MAX_DBCONN_ID_LEN];     //集群节点的唯一ID
    redisContext *pRedisCtx;    //实际连接hiredis的指针
    bool bConnected;            //是否连接成功
    tagRedisConnInfo()
    {
        memset(this, 0, sizeof(tagRedisConnInfo));
        bConnected = false;
    }
}TRedisConnInfo;

typedef struct tagRedisNodeInfo
{
    TRedisConnInfo                  masterConn; //集群主节点的连接信息
    std::vector<TRedisConnInfo>     slaveConn;  //集群从节点信息数组
    std::vector<TRedisNodeSlotsRange> nodeSlots;//集群节点slots数组
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
    //slave节点手动重连
    bool RedisReConSlave(TRedisConnInfo &tSlaveConnInfo);

    BOOL32 Ping();
    //
    redisContext * const GetConnection(int nConType);
    unsigned int    HaveSlave() const   { return  m_bHaveSlave; }
    bool            GetConnStatus() const   { return  m_bConnStatus; }
    void            PrintNodeInfo();
    //
    bool CheckSlot(unsigned int nSlot);
    //获取数据的实际存储类型，如果回MC_TYPE_ERR说明数据不存在或格式不支持
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
    CMCRedis       *m_pcMcParent;       //连接所属对象的地址，用于打印日志信息
    //互斥锁
    CThreadLockMutex      m_clmLock;   
};

#endif //_REDIS_NODE_H_