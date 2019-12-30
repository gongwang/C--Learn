#include "interface_alive_fun.h"

#include "windows.h"
#include <map>
typedef struct tagAliveThreadInfo{
    DWORD m_dwThreadID;
    tagAliveThreadInfo()
    {
        memset(this, 0, sizeof(tagAliveThreadInfo));
    }
}TAliveThreadInfo;
bool g_bInitAlive = false;
CRITICAL_SECTION g_csCommonLock;
std::map<DWORD, TAliveThreadInfo> g_mapAliveThreadInfo;
