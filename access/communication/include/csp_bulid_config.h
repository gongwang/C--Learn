#ifndef CSP_BULID_CONFIG_INCLUDE
#define CSP_BULID_CONFIG_INCLUDE

#include "platform.h"

#if defined(CSP_OS_FAMILY_WINDOWS)
#include "platform_win32.h"
#elif defined(CSP_OS_FAMILY_UNIX)
#include "platform_unix.h"
#endif

#endif //CSP_BULID_CONFIG_INCLUDE

