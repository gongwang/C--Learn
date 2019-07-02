#pragma once

// onlytestPropPage.h : Declaration of the ConlytestPropPage property page class.


// ConlytestPropPage : See onlytestPropPage.cpp for implementation.

class ConlytestPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(ConlytestPropPage)
	DECLARE_OLECREATE_EX(ConlytestPropPage)

// Constructor
public:
	ConlytestPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_ONLYTEST };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

