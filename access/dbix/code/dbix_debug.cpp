#if WIN32 && (_MSC_VER <= 1200)
#pragma warning(disable : 4786)
#endif

#include "dbix_debug.h"
#include "osp.h"
#include "ospext/ospext.h"
//#include "cms/ospext/osplog.h"
//#include "cms/ospext/ospdbg.h"
#include "cms/cmsassert.h"
#include "connect_pool.h"
#include "task_queue.h"
#include "thread_pool.h"

//extern void InitDbixResultCbMsgDesc();

DBIX_NS_BEGIN

// extern CConnectPool g_cConnectPool;
// extern CTaskQueue g_cTaskQueue;
// extern CThreadPool g_cThreadPool;

void log(TLogLvl tLogLvl, const s8* lpcstrFormat, ...)
{
    //assert(enLOGLVL_ERROR != tLogLvl);

    s8 szBuf[1024];
    va_list pvlist;
    va_start(pvlist, lpcstrFormat);
    s32 nLen = vsnprintf(szBuf, sizeof(szBuf) -1, lpcstrFormat, pvlist);
    if( nLen <= 0 || nLen >= (s32)sizeof(szBuf) ) szBuf[sizeof(szBuf)-1] = '\0';
    va_end(pvlist);

    CMS_ASSERT_WITH_MSG((enLOGLVL_ERROR != tLogLvl), szBuf);

    //GLOBELOG(DBIX, (u8)tLogLvl, "%s\n", szBuf);
}

// API void dbixlog(u8 byLvl)
// {
//     OspSetModLogLev(DBIX, byLvl, "DBIX");
// }
// 
// API void dbixstat()
// {
//     OspPrintf(TRUE, FALSE, " dbix全局配置信息================================================\n");
//     OspPrintf(TRUE, FALSE, " 结果回调函数地址:%s\n", (g_tDBIXConf.cbDBResult!=NULL? "有效":"无效"));
//     OspPrintf(TRUE, FALSE, " 数据库操作线程数目:%u\n", g_tDBIXConf.dwThreadNum);
//     OspPrintf(TRUE, FALSE, " 数据库连接数目最大值:%u\n", g_tDBIXConf.dwConnectionMaxNum);
// 
//     OspPrintf(TRUE, FALSE, "\n");
//     g_cConnectPool.Dump();
//     OspPrintf(TRUE, FALSE, "\n");
//     g_cTaskQueue.Dump();
//     OspPrintf(TRUE, FALSE, "\n");
//     g_cThreadPool.Dump();
// }
// 
// #define DBIX_VER "dbix v1.0  Compile Time: %s, %s\r\n", __DATE__, __TIME__
// API void dbixver()
// {
//     OspPrintf(TRUE, FALSE, " ===========================================================\n");
//     OspPrintf(TRUE, FALSE, DBIX_VER);
// }
// 
// API void dbixhelp()
// {
//     OspPrintf(TRUE, FALSE, " 帮助命令===========================================================\n");
//     OspPrintf(TRUE, FALSE, " dbixhelp         : 显示帮助信息\n");
//     OspPrintf(TRUE, FALSE, " dbixver          : 显示版本信息\n");
//     OspPrintf(TRUE, FALSE, " dbixstat         : 显示状态数据\n");
//     OspPrintf(TRUE, FALSE, " dbixlog(level)   : 控制日志级别. level数值, 1--重要错误 2--关键流程 3--警告错误 7--调试级别\n");
//     OspPrintf(TRUE, FALSE, " dbi库帮助命令列表 ：dbiver, dblogon, dblogoff, pstat, cstat\n");
// }

void RegisterDbixCmd()
{
#ifdef _LINUX_
    /*OspRegCommand("dbixhelp", (void*)dbixhelp, "dbixhelp");
    OspRegCommand("dbixver", (void*)dbixver, "dbixver");
    OspRegCommand("dbixstat", (void*)dbixstat, "dbixstat");
    OspRegCommand("dbixlog", (void*)dbixlog, "dbixlog");*/
#endif

  //  ::InitDbixResultCbMsgDesc();
}

void DbixLoglvl(TLogLvl tLvl)
{
    //zzmdbixlog(tLvl);
}

DBIX_NS_END
