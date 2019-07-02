// boka_dll_test.h : main header file for the boka_dll_test DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cboka_dll_testApp
// See boka_dll_test.cpp for the implementation of this class
//

class Cboka_dll_testApp : public CWinApp
{
public:
	Cboka_dll_testApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
