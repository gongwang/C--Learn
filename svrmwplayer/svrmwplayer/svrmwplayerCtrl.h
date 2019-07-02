#pragma once

// svrmwplayerCtrl.h : Declaration of the CsvrmwplayerCtrl ActiveX Control class.


// CsvrmwplayerCtrl : See svrmwplayerCtrl.cpp for implementation.

class CsvrmwplayerCtrl : public COleControl
{
	DECLARE_DYNCREATE(CsvrmwplayerCtrl)

// Constructor
public:
	CsvrmwplayerCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CsvrmwplayerCtrl();

	DECLARE_OLECREATE_EX(CsvrmwplayerCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CsvrmwplayerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CsvrmwplayerCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CsvrmwplayerCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	};
};

