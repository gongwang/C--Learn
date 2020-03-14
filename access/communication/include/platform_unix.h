/********************************************************************
	created:	2012/11/15 9:55 
	file base:	platform_unix.h
	author:		Milo
	
	purpose:	
*********************************************************************/

#pragma once

//
// PA-RISC based HP-UX platforms have some issues...
//
#if defined(hpux) || defined(_hpux)
#if defined(__hppa) || defined(__hppa__)
#define POCO_NO_SYS_SELECT_H 1
#if defined(__HP_aCC)
#define POCO_NO_TEMPLATE_ICOMPARE 1
#endif
#endif
#endif

#include <stdio.h>

#define CSP_SSCANF 		sscanf
#define CSP_SNPRINTF 	snprintf
#define CSP_VSNPRINT 	vsnprintf
#define CSP_STRNCPY	 	strncpy

#define STD_MOVE

#define HAVE_DLFCN_H
