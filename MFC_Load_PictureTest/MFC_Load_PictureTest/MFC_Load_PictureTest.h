
// MFC_Load_PictureTest.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFC_Load_PictureTestApp:
// See MFC_Load_PictureTest.cpp for the implementation of this class
//

class CMFC_Load_PictureTestApp : public CWinApp
{
public:
	CMFC_Load_PictureTestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFC_Load_PictureTestApp theApp;