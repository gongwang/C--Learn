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
//     OspPrintf(TRUE, FALSE, " dbixȫ��������Ϣ================================================\n");
//     OspPrintf(TRUE, FALSE, " ����ص�������ַ:%s\n", (g_tDBIXConf.cbDBResult!=NULL? "��Ч":"��Ч"));
//     OspPrintf(TRUE, FALSE, " ���ݿ�����߳���Ŀ:%u\n", g_tDBIXConf.dwThreadNum);
//     OspPrintf(TRUE, FALSE, " ���ݿ�������Ŀ���ֵ:%u\n", g_tDBIXConf.dwConnectionMaxNum);
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
//     OspPrintf(TRUE, FALSE, " ��������===========================================================\n");
//     OspPrintf(TRUE, FALSE, " dbixhelp         : ��ʾ������Ϣ\n");
//     OspPrintf(TRUE, FALSE, " dbixver          : ��ʾ�汾��Ϣ\n");
//     OspPrintf(TRUE, FALSE, " dbixstat         : ��ʾ״̬����\n");
//     OspPrintf(TRUE, FALSE, " dbixlog(level)   : ������־����. level��ֵ, 1--��Ҫ���� 2--�ؼ����� 3--������� 7--���Լ���\n");
//     OspPrintf(TRUE, FALSE, " dbi����������б� ��dbiver, dblogon, dblogoff, pstat, cstat\n");
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
