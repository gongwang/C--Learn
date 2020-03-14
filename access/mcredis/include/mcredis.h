
#ifndef _MCREDIS_H_
#define _MCREDIS_H_

#include "redisconnection.h"
#ifdef _MSC_VER
#include "pthread_w32.h"
#include <Windows.h>
#else
#include <pthread.h>
#endif
#ifdef _MSC_VER
#define sleep(nseconds) Sleep(nseconds * 1000);
#define strcasecmp  stricmp
#endif

#define MAX_LOG_LENGTH          2000    //日志记录的最大长度
#define INSTID_EXPIRE_TIME      60      //实例id超时时间,单位秒，默认设置1分钟，刷新时间为这个值的一半
#define INSTID_KEY_PREFIX              "inside:"   //instid数据前缀

class CMCRedis : public IMCInterface
{
public:
    CMCRedis();
    ~CMCRedis();

    virtual BOOL32 Init( MCLogCallBack pLogCallBack, MCOnDisconnectCB pDisConnectCB, void* pUser );

    virtual s32 Login( vecMCServerInfo &vServer, std::string &strUserName, std::string &strPassword, s32 nTimeout = 1000 );

    virtual s32 ReConnect( s32 nTimeout );

    virtual s32 Logout();

    virtual s32 GetState();

    virtual void PrintServerData();

    virtual std::string& GetInstId(){return m_strInstId;}

    virtual s32 GetString( std::string &strKey, OUT std::string &strValue );

    virtual s32 MultiGetString( std::vector<std::string> &vecKeys, OUT std::vector<std::string> &vecValues);

    virtual s32 GetInt( std::string &strKey, OUT s32 &nValue );
    //对于复合元素，redis返回的都是array数组，当key不存在时返回数组大小为0，并不会返回error
    virtual s32 GetSets( std::string &strKey, OUT std::vector<std::string> &vecValue );
    virtual s32 GetList( std::string &strKey, OUT std::vector<std::string> &vecValue );
    virtual s32 GetHash( std::string &strKey, OUT std::map<std::string, std::string> &mapValue );

    virtual s32 Append( std::string &strKey, std::string &strValue);

    virtual s32 Incr( std::string &strKey, OUT s32 &nValue);

    virtual s32 Decr( std::string &strKey, OUT s32 &nValue );

    virtual s32 Set( std::string &strKey, std::string &strValue, eMCSetType tSet = MC_SET_DEFAULT, u32 nexptime = 0 );

    virtual s32 SetExpireTime(std::string &strKey, u32 nexptime = 0);

    virtual s32 Exists(std::string &strKey);

    virtual s32 ListGet( std::string &strKey, s32 nIndex, OUT std::string &strValue );

    virtual s32 ListSet( std::string &strKey, s32 nIndex, std::string &strValue );

    virtual s32 ListLen( std::string &strKey, OUT s32 &nValue );

    virtual s32 ListLPush( std::string &strKey, std::vector<std::string> &vecValue );

    virtual s32 ListLPop( std::string &strKey, OUT std::string &strValue );

    virtual s32 ListRPush( std::string &strKey, std::vector<std::string> &vecValue );

    virtual s32 ListRPop( std::string &strKey, OUT std::string &strValue );

    virtual s32 SetsAdd( std::string &strKey, std::vector<std::string> &vecValue, u32 nexptime = 0 );

    virtual s32 SetsRem( std::string &strKey, std::vector<std::string> &vecValue );

    virtual s32 SetsExists( std::string &strKey, std::string &strValue, OUT bool &bExist );

    virtual s32 HashGet( std::string &strKey, std::string &strField, OUT std::string &strValue );

    virtual s32 HashSet( std::string &strKey, std::string &strField, std::string &strValue );

    virtual s32 HashDel( std::string &strKey, std::string &strField );

    virtual s32 HashGetKeys( std::string &strKey, OUT std::vector<std::string> &vecFields );

    virtual s32 HashExists( std::string &strKey, std::string &strField, OUT bool &bExist );

    virtual s32 HashSetAll( std::string &strKey, std::map<std::string,std::string> &hashValue, u32 nexptime = 0 );

    virtual s32 DelKeys( std::vector<std::string> &vecKeys );

    virtual s32 Watch( std::vector<std::string> &vecKeys );
    virtual s32 Multi();
    virtual s32 Exec();

    virtual s32 Discard();

    void PrintLog(eMCLoglev eLevel, const s8* const szFormat, ...);

    bool IsLogined(){return m_bLogined;}
    bool HasDisconCb(){return NULL != m_pDisconCb;}
    bool IsRedisConnected();
    BOOL32 KeepAlive();
    void OnDisconnect();

private:
    MCLogCallBack       m_pfLogCb;
    void*              m_pUser;
    CRedisConnection    m_cRedisCon;
    bool                m_bLogined;
    MCOnDisconnectCB    m_pDisconCb;
    pthread_t           m_pthread;
    //当前redis实例的uuid
    std::string m_strInstId;
};
#endif //_MCREDIS_H_