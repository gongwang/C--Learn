/********************************************************************
	created:	2012/11/15 9:38 
	file base:	platform.h
	author:		Milo
	
	purpose:	平台依赖定义
*********************************************************************/

#pragma once

//
// Platform Identification
//
#define CSP_OS_LINUX         0x0005
#define CSP_OS_WINDOWS_NT    0x1001

#if defined(_WIN32) || defined(_WIN64)
#define CSP_OS_FAMILY_WINDOWS 1
#define CSP_OS CSP_OS_WINDOWS_NT
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__TOS_LINUX__)
#define CSP_OS_FAMILY_UNIX 1
#define CSP_OS CSP_OS_LINUX
#endif

// Compiler detection.
#if defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSVC 1
#else
#error Please add support for your compiler in platform.h
#endif

// Use tcmalloc
/*
#if (defined(OS_WIN) || defined(OS_LINUX)) && !defined(NO_TCMALLOC)
#define USE_TCMALLOC 1
#endif
*/

