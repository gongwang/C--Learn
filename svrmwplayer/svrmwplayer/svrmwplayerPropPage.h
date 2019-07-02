#pragma once

// svrmwplayerPropPage.h : Declaration of the CsvrmwplayerPropPage property page class.


// CsvrmwplayerPropPage : See svrmwplayerPropPage.cpp for implementation.

class CsvrmwplayerPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CsvrmwplayerPropPage)
	DECLARE_OLECREATE_EX(CsvrmwplayerPropPage)

// Constructor
public:
	CsvrmwplayerPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_SVRMWPLAYER };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

