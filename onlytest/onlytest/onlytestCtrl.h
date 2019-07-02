#pragma once

// onlytestCtrl.h : Declaration of the ConlytestCtrl ActiveX Control class.


// ConlytestCtrl : See onlytestCtrl.cpp for implementation.

class ConlytestCtrl : public COleControl
{
	DECLARE_DYNCREATE(ConlytestCtrl)

// Constructor
public:
	ConlytestCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~ConlytestCtrl();

	DECLARE_OLECREATE_EX(ConlytestCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(ConlytestCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(ConlytestCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(ConlytestCtrl)		// Type name and misc status

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

