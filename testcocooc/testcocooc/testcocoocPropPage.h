#pragma once

// testcocoocPropPage.h : Declaration of the CtestcocoocPropPage property page class.


// CtestcocoocPropPage : See testcocoocPropPage.cpp for implementation.

class CtestcocoocPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CtestcocoocPropPage)
	DECLARE_OLECREATE_EX(CtestcocoocPropPage)

// Constructor
public:
	CtestcocoocPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_TESTCOCOOC };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

