// testcocoocCtrl.cpp : Implementation of the CtestcocoocCtrl ActiveX Control class.

#include "stdafx.h"
#include "testcocooc.h"
#include "testcocoocCtrl.h"
#include "testcocoocPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CtestcocoocCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CtestcocoocCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CtestcocoocCtrl, COleControl)
	DISP_FUNCTION_ID(CtestcocoocCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CtestcocoocCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CtestcocoocCtrl, 1)
	PROPPAGEID(CtestcocoocPropPage::guid)
END_PROPPAGEIDS(CtestcocoocCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CtestcocoocCtrl, "TESTCOCOOC.testcocoocCtrl.1",
	0xb880d962, 0x8460, 0x4f20, 0x8a, 0x32, 0x91, 0xd4, 0x5c, 0x7d, 0x96, 0xc6)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CtestcocoocCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID IID_Dtestcocooc = { 0xAF8FD425, 0x6C2B, 0x400F, { 0xA7, 0x55, 0x3E, 0x75, 0xCE, 0x6D, 0x34, 0x45 } };
const IID IID_DtestcocoocEvents = { 0xA699D8C6, 0xE515, 0x4639, { 0xBE, 0xEA, 0x2, 0xE7, 0xF9, 0x91, 0x41, 0x77 } };


// Control type information

static const DWORD _dwtestcocoocOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CtestcocoocCtrl, IDS_TESTCOCOOC, _dwtestcocoocOleMisc)



// CtestcocoocCtrl::CtestcocoocCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CtestcocoocCtrl

BOOL CtestcocoocCtrl::CtestcocoocCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_TESTCOCOOC,
			IDB_TESTCOCOOC,
			afxRegApartmentThreading,
			_dwtestcocoocOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CtestcocoocCtrl::CtestcocoocCtrl - Constructor

CtestcocoocCtrl::CtestcocoocCtrl()
{
	InitializeIIDs(&IID_Dtestcocooc, &IID_DtestcocoocEvents);
	// TODO: Initialize your control's instance data here.
}



// CtestcocoocCtrl::~CtestcocoocCtrl - Destructor

CtestcocoocCtrl::~CtestcocoocCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CtestcocoocCtrl::OnDraw - Drawing function

void CtestcocoocCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CtestcocoocCtrl::DoPropExchange - Persistence support

void CtestcocoocCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CtestcocoocCtrl::OnResetState - Reset control to default state

void CtestcocoocCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CtestcocoocCtrl::AboutBox - Display an "About" box to the user

void CtestcocoocCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_TESTCOCOOC);
	dlgAbout.DoModal();
}



// CtestcocoocCtrl message handlers
