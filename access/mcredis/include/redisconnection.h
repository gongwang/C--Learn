#ifndef _REDIS_CONNECTION_H_
#define _REDIS_CONNECTION_H_
#include "redisnode.h"

//集群使用公式 CRC16(key) % 16384 来计算键 key 属于哪个槽， 其中 CRC16(key) 语句用于计算键 key 的 CRC16 校验和 
class CRedisConnection
{
public:
    CRedisConnection();
    ~CRedisConnection();
    bool Init(CMCRedis *pcMcRedis, vecMCServerInfo &vServer, std::string &strPassword, unsigned int nTimeout = 0);
    s32 Connect();
    s32 ReConnect();
    s32 DisConnect();
    BOOL32 KeepAlive();
    bool IsConnected(){return m_bConnStatus;}
    bool IsCluster(){return m_bIsCluster;}
    u32 GetNodeNum(){return m_nNodeNum;}
    CRedisNode* GetNode(std::string &strKey);
    CRedisNode* GetNode(u32 nNodeIndex);
    void PrintServerData();
private:
    redisContext *ConnServerNode(std::pair<std::string, u32> pairNode, std::string &strPassword);
    std::vector<TRedisNodeInfo> GetClusterInfo(redisContext* pCtx);
private:
    vecMCServerInfo   m_vecServerinfo;    //
    std::string     m_strPassword;
    CRedisNode     *m_pcNodeArr;        //
    bool            m_bIsCluster;       //  
    unsigned int    m_nNodeNum;         //
    bool            m_bConnStatus;      //
    unsigned int    m_nTimeout;         // connect timeout second
    bool            m_bInit;            //
    //printlog param
    CMCRedis       *m_pcMcParent;       //连接所属对象的地址，用于打印日志信息
};

#endif //_REDIS_CONNECTION_H_