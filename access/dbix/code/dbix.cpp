#if WIN32 && (_MSC_VER <= 1200)
#pragma warning(disable : 4786)
#endif

#include "inner_common.h"
#include "connect_pool.h"
#include "task_queue.h"
#include "thread_pool.h"
#include "dbi_layer.h"

DBIX_NS_BEGIN

extern void RegisterDbixCmd();

bool g_bInited = false;
TDBIXConf g_tDBIXConf;

CConnectPool g_cConnectPool;
CTaskQueue g_cTaskQueue;
CThreadPool g_cThreadPool;


bool DBIXInit( const TDBIXConf &tConf )
{
    if (g_bInited)
    {
        log(enLOGLVL_ERROR, "dbix reinited");
        return true;
    }

    do
    {
        if (tConf.dwThreadNum > tConf.dwConnectionMaxNum || tConf.dwThreadNum == 0 || tConf.cbDBResult == NULL)
        {
            log(enLOGLVL_ERROR, "invalid agrument");
            break;
        }
        //zzmdbixlog(enLOGLVL_WARN);

        g_tDBIXConf = tConf;

        if (!DBIInit(g_tDBIXConf.tDBConnectCfg.enDbType))
        {
            break;
        }

        if (!g_cTaskQueue.Init())
        {
            break;
        }

        if (!g_cConnectPool.Init(g_tDBIXConf.dwConnectionMaxNum, g_tDBIXConf.tDBConnectCfg))
        {
            break;
        }

        vector<CDBConnection*> cDBConns;
        u32 i;
        for (i = 0; i < tConf.dwThreadNum; ++i)
        {
            cDBConns.push_back(AllocateDBConnection());
            if (cDBConns.back() == NULL)
            {
                break;
            }
        }
        if (i != tConf.dwThreadNum)
        {
            break;
        }

        if (!g_cThreadPool.Init(&g_cTaskQueue, cDBConns))
        {
            break;
        }

        RegisterDbixCmd();

        g_bInited = true;
        log(enLOGLVL_CRITICAL_PATH, "dbix init success");
        return true;
    } while (false);

    DBIXQuit();
    log(enLOGLVL_CRITICAL_PATH, "dbix init fail");
    return false;
}

void DBIXQuit()
{
    g_cThreadPool.Quit();
    g_cConnectPool.Quit();
    g_cTaskQueue.Quit();

    g_bInited = false;

    log(enLOGLVL_CRITICAL_PATH, "dbix quit");
}

DBIX_NS_END
