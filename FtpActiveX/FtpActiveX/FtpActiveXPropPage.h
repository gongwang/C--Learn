#pragma once

// FtpActiveXPropPage.h : Declaration of the CFtpActiveXPropPage property page class.


// CFtpActiveXPropPage : See FtpActiveXPropPage.cpp for implementation.

class CFtpActiveXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CFtpActiveXPropPage)
	DECLARE_OLECREATE_EX(CFtpActiveXPropPage)

// Constructor
public:
	CFtpActiveXPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_FTPACTIVEX };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

