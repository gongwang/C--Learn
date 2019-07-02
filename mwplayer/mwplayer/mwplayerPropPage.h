#pragma once

// mwplayerPropPage.h : Declaration of the CmwplayerPropPage property page class.


// CmwplayerPropPage : See mwplayerPropPage.cpp for implementation.

class CmwplayerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CmwplayerPropPage)
	DECLARE_OLECREATE_EX(CmwplayerPropPage)

// Constructor
public:
	CmwplayerPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_MWPLAYER };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

