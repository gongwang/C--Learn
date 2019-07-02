#pragma once

// mwplayerCtrl.h : Declaration of the CmwplayerCtrl ActiveX Control class.


// CmwplayerCtrl : See mwplayerCtrl.cpp for implementation.

class CmwplayerCtrl : public COleControl
{
	DECLARE_DYNCREATE(CmwplayerCtrl)

// Constructor
public:
	CmwplayerCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CmwplayerCtrl();

	DECLARE_OLECREATE_EX(CmwplayerCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CmwplayerCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CmwplayerCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CmwplayerCtrl)		// Type name and misc status

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

