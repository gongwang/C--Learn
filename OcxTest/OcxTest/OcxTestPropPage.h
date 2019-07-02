#pragma once

// OcxTestPropPage.h : Declaration of the COcxTestPropPage property page class.


// COcxTestPropPage : See OcxTestPropPage.cpp for implementation.

class COcxTestPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(COcxTestPropPage)
	DECLARE_OLECREATE_EX(COcxTestPropPage)

// Constructor
public:
	COcxTestPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_OCXTEST };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

