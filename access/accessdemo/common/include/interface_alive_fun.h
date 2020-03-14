#ifndef _INTERFACE_ALIVE_FUN_H_
#define _INTERFACE_ALIVE_FUN_H_

#ifdef _WINDOWS_

#include "windows.h"
#include <map>

/************************************************************************/
/* 心跳检测回调函数                                                     */
/* 返回值：true：成功（在线）  false：失败（离线）                      */
/* 用途：定时调用次回调，如返回失败则认为设备离线，自动调用登录回调     */
/************************************************************************/
typedef bool (*DeviceAliveCheckCB)(void* pAliveData);

/************************************************************************/
/* 设备登录回调函数                                                     */
/* 返回值：true：成功（在线）  false：失败（离线）                      */
/* 用途：定时调用次回调，如返回失败则认为设备离线，自动调用登录回调     */
/************************************************************************/
typedef bool (*DeviceProcLoginCB)(void* pLoginData);


typedef struct tagAliveDeviceData
{
    DeviceAliveCheckCB m_pAliveCB;
    void* m_pAliveData;
    DeviceProcLoginCB m_pLoginCB;
    void* m_pLoginData;
    int m_nAliveTimer;
    DWORD m_dwThreadID;
    tagAliveDeviceData()
    {
        memset(this, 0, sizeof(tagAliveDeviceData));
    }
}TAliveDeviceData;

typedef struct tagAliveThreadInfo{
    DWORD m_dwThreadID;
    tagAliveThreadInfo()
    {
        memset(this, 0, sizeof(tagAliveThreadInfo));
    }
}TAliveThreadInfo;

extern bool g_bInitAlive;
extern CRITICAL_SECTION g_csCommonLock;
extern std::map<DWORD, TAliveThreadInfo> g_mapAliveThreadInfo;

///************************************************************************/
///* 1.初始化                                                               */
///************************************************************************/
//static int InitAliveProccess();
//
///************************************************************************/
///* 2.反初始化                                                             */
///************************************************************************/
//static int UninitAliveProccess();
//
///************************************************************************/
///* 3.创建设备心跳检测线程                                                 */
///* 参数：DeviceAliveCheckCB pAliveCB    心跳检测回调                    */
///*       void* pAliveData               心跳检测回调用户数据            */
///*       DeviceProcLoginCB pLoginCB     登录回调函数                    */
///*       void* pLoginData               登录回调用户数据                */
///*       int nAliveTimer                心跳检测时间间隔（毫秒）        */
///* 返回值：线程唯一标识（句柄）                                         */
///************************************************************************/
//static DWORD CreateCheckAliveProc(DeviceAliveCheckCB pAliveCB, void* pAliveData, DeviceProcLoginCB pLoginCB, void* pLoginData, int nAliveTimer = 30000);
//
///************************************************************************/
///* 4.创建设备心跳检测线程                                                 */
///* 参数：DWORD dwProcID 线程唯一标识（句柄），CreateCheckAliveProc返回值*/
///* 返回值：无                                                           */
///************************************************************************/
//static void DestroyCheckAliveProc(DWORD dwProcID);


/************************************************************************/
/************************************************************************/
/*************                函数实现部分                 **************/
/************************************************************************/
/************************************************************************/

static int InitAliveProccess()
{
    if(g_bInitAlive)
    {
        return 0;
    }
    g_bInitAlive = true;
    InitializeCriticalSection(&g_csCommonLock);
}

static int UninitAliveProccess()
{
    if(g_bInitAlive == false)
        return 0;

    g_bInitAlive = false;
    DeleteCriticalSection(&g_csCommonLock);
}

static void InsertAliveThreadInfo(DWORD dwThreadID, const TAliveThreadInfo &tAliveThreadInfo)
{
    if (!g_bInitAlive)
    {
        InitAliveProccess();
    }

	EnterCriticalSection(&g_csCommonLock);
	g_mapAliveThreadInfo[dwThreadID] = tAliveThreadInfo;
	LeaveCriticalSection(&g_csCommonLock);
}
static void DeleteAliveThreadInfo(DWORD dwThreadID)
{
    if (!g_bInitAlive)
    {
        InitAliveProccess();
    }
	EnterCriticalSection(&g_csCommonLock);
	std::map<DWORD, TAliveThreadInfo>::iterator iter = g_mapAliveThreadInfo.find(dwThreadID);
	if(iter != g_mapAliveThreadInfo.end())
	{
		g_mapAliveThreadInfo.erase(iter);
	}
	LeaveCriticalSection(&g_csCommonLock);
}
static bool FindAliveThreadInfo(DWORD dwThreadID, TAliveThreadInfo* ptAliveThreadInfo = NULL)
{
    if (!g_bInitAlive)
    {
        InitAliveProccess();
    }

	bool bRet = false;
	EnterCriticalSection(&g_csCommonLock);
	std::map<DWORD, TAliveThreadInfo>::iterator iter = g_mapAliveThreadInfo.find(dwThreadID);
	if(iter != g_mapAliveThreadInfo.end())
	{
		if(ptAliveThreadInfo) memcpy(ptAliveThreadInfo, &iter->second, sizeof(TAliveThreadInfo));
		bRet = true;
	}
	LeaveCriticalSection(&g_csCommonLock);
	return bRet;
}
static DWORD WINAPI AliveProcessThread(LPVOID lpParam)
{
	TAliveDeviceData* ptAliveDeviceData = (TAliveDeviceData*)lpParam;
	if(ptAliveDeviceData == NULL)
	{
		return 0;
	}

	int nCount = 0;
	while(ptAliveDeviceData->m_dwThreadID == 0)
	{
		nCount++;
		Sleep(1000);
		if(nCount > 10)
		{
			return 0;
		}
	}
	DWORD dwThreadID = ptAliveDeviceData->m_dwThreadID;
	TAliveThreadInfo tAliveThreadInfo;
	while(true)
	{
		Sleep(ptAliveDeviceData->m_nAliveTimer);
		if(FindAliveThreadInfo(dwThreadID, &tAliveThreadInfo) == false)
			break;
		if(ptAliveDeviceData->m_pAliveCB && ptAliveDeviceData->m_pAliveCB(ptAliveDeviceData->m_pAliveData) == false)
		{
			if(ptAliveDeviceData->m_pLoginCB) ptAliveDeviceData->m_pLoginCB(ptAliveDeviceData->m_pLoginData);
		}
	}
	delete ptAliveDeviceData;
	ptAliveDeviceData = NULL;
	return 0;
}

static DWORD CreateCheckAliveProc(DeviceAliveCheckCB pAliveCB, void* pAliveData, DeviceProcLoginCB pLoginCB, void* pLoginData, int nAliveTimer)
{
	TAliveDeviceData* ptAliveThreadData = new TAliveDeviceData;
	ptAliveThreadData->m_pAliveCB = pAliveCB;
	ptAliveThreadData->m_pAliveData = pAliveData;
	ptAliveThreadData->m_pLoginCB = pLoginCB;
	ptAliveThreadData->m_pLoginData = pLoginData;
	ptAliveThreadData->m_nAliveTimer = (nAliveTimer > 0 ? nAliveTimer : 30000);

	DWORD dwThread = 0;
	HANDLE hHandle = ::CreateThread(NULL, 0, AliveProcessThread, ptAliveThreadData, 0, &dwThread);
	CloseHandle(hHandle);

	ptAliveThreadData->m_dwThreadID = dwThread;

	TAliveThreadInfo tAliveThreadInfo;
	tAliveThreadInfo.m_dwThreadID = dwThread;
	InsertAliveThreadInfo(dwThread, tAliveThreadInfo);

	return dwThread;

}

static void DestroyCheckAliveProc(DWORD dwProcID)
{
	DeleteAliveThreadInfo(dwProcID);
}

#endif	// _WINDOWS_

#endif	// _INTERFACE_ALIVE_FUN_H_