#include "telnetServer.h"

std::vector<void*> g_vecDevObject;
CRITICAL_SECTION g_csDevObjectLock;
BOOL g_bDevObjectLockInit = FALSE;