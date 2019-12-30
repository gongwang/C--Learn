/********************************************************************
	created:	2012/11/15 9:45 
	file base:	platform_win32.h
	author:		Milo
	
	purpose:	
*********************************************************************/

#pragma once 

// Verify that we're built with the multithreaded 
// versions of the runtime libraries
#if defined(_MSC_VER) && !defined(_MT)
#error Must compile with /MD, /MDd, /MT or /MTd
#endif


// Check debug/release settings consistency
#if defined(NDEBUG) && defined(_DEBUG)
#error Inconsistent build settings (check for /MD[d])
#endif


#if defined(_WIN32)
#if !defined(_WIN32_WINNT)
#define _WIN32_WINNT 0x0501
#endif
#if !defined(WIN32_LEAN_AND_MEAN) && !defined(CSP_BLOATED_WIN32)
#define WIN32_LEAN_AND_MEAN
#endif
#endif


// Unicode Support
#if defined(UNICODE) && !defined(CSP_WIN32_UTF8)
#define CSP_WIN32_UTF8
#endif



// Turn off some annoying warnings
#if defined(_MSC_VER)
#pragma warning(disable:4018) // signed/unsigned comparison
#pragma warning(disable:4251) // ... needs to have dll-interface warning 
#pragma warning(disable:4355) // 'this' : used in base member initializer list
#pragma warning(disable:4996) // VC++ 8.0 deprecation warnings
#pragma warning(disable:4351) // new behavior: elements of array '...' will be default initialized
#pragma warning(disable:4675) // resolved overload was found by argument-dependent lookup
#pragma warning(disable:4275) // non dll-interface class 'std::exception' used as base for dll-interface class 'Poco::Exception'
#pragma warning(disable:4250) // VC++ 11.0: inheriting from std stream classes produces C4250 warning;
#pragma warning(disable:4006) // _OspLogShow already defined in osplib.lib(ospLog.obj); second definition ignored
#pragma warning(disable:4099) // PDB 'vc100.pdb' was not found with 'csputil.lib(mutex.obj)' or at 'F:\svn_roots\CSP_VOB\30-csp\pas\Debug\vc100.pdb'; linking object as if no debug info
// see <http://connect.microsoft.com/VisualStudio/feedback/details/733720/inheriting-from-std-fstream-produces-c4250-warning>
#endif


#if defined(__INTEL_COMPILER)
#pragma warning(disable:1738) // base class dllexport/dllimport specification differs from that of the derived class
#pragma warning(disable:1478) // function ... was declared "deprecated"
#pragma warning(disable:1744) // field of class type without a DLL interface used in a class with a DLL interface
#endif

#if defined(_MSC_VER)
#define CSP_SSCANF 		sscanf_s
#define CSP_SNPRINTF 	_snprintf_s
#define CSP_VSNPRINT 	_vsnprintf_s
#define CSP_STRNCPY	 	strncpy_s
#define STD_MOVE 		std::move
#else
#define CSP_SSCANF 		sscanf
#define CSP_SNPRINTF 	snprintf
#define CSP_VSNPRINT 	vsnprintf
#define CSP_STRNCPY	 	strncpy
#define STD_MOVE
#endif

#include <WTypes.h>
