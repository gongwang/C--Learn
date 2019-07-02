#pragma once

#include "ocxEmbedUI.h"
// OcxTestCtrl.h : Declaration of the COcxTestCtrl ActiveX Control class.


// COcxTestCtrl : See OcxTestCtrl.cpp for implementation.

class COcxTestCtrl : public COleControl
{
	DECLARE_DYNCREATE(COcxTestCtrl)

// Constructor
public:
	COcxTestCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	ocxEmbedUI myUI;
// Implementation
protected:
	~COcxTestCtrl();

	DECLARE_OLECREATE_EX(COcxTestCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(COcxTestCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(COcxTestCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(COcxTestCtrl)		// Type name and misc status

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
		eventidOnSend = 1L,
		dispidStart = 2L,
		dispidTimeStyle = 1
	};
protected:
	void OnTimeStyleChanged(void);
	LONG m_TimeStyle;
	void Start(void);

	void ShowTime(void);

	void FileOnSend(LPCTSTR lpStrTime)
	{
		FireEvent(eventidOnSend, EVENT_PARAM(VTS_BSTR), lpStrTime);
	}
};

