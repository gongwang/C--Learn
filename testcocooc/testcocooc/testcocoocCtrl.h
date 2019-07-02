#pragma once

// testcocoocCtrl.h : Declaration of the CtestcocoocCtrl ActiveX Control class.


// CtestcocoocCtrl : See testcocoocCtrl.cpp for implementation.

class CtestcocoocCtrl : public COleControl
{
	DECLARE_DYNCREATE(CtestcocoocCtrl)

// Constructor
public:
	CtestcocoocCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CtestcocoocCtrl();

	DECLARE_OLECREATE_EX(CtestcocoocCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CtestcocoocCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CtestcocoocCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CtestcocoocCtrl)		// Type name and misc status

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

