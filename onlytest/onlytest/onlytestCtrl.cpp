// onlytestCtrl.cpp : Implementation of the ConlytestCtrl ActiveX Control class.

#include "stdafx.h"
#include "onlytest.h"
#include "onlytestCtrl.h"
#include "onlytestPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(ConlytestCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(ConlytestCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(ConlytestCtrl, COleControl)
	DISP_FUNCTION_ID(ConlytestCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(ConlytestCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(ConlytestCtrl, 1)
	PROPPAGEID(ConlytestPropPage::guid)
END_PROPPAGEIDS(ConlytestCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(ConlytestCtrl, "ONLYTEST.onlytestCtrl.1",
	0x15f63ded, 0xb7c5, 0x42ba, 0xad, 0x72, 0xf5, 0xd8, 0x36, 0x4b, 0x2b, 0x47)



// Type library ID and version

IMPLEMENT_OLETYPELIB(ConlytestCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID IID_Donlytest = { 0x22DCBE03, 0x7163, 0x43C2, { 0x99, 0x16, 0x98, 0x47, 0xB7, 0x35, 0x56, 0xAE } };
const IID IID_DonlytestEvents = { 0x14BB9456, 0xFF57, 0x4462, { 0x9B, 0xF3, 0x5C, 0x67, 0x34, 0x7B, 0xA6, 0xED } };


// Control type information

static const DWORD _dwonlytestOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(ConlytestCtrl, IDS_ONLYTEST, _dwonlytestOleMisc)



// ConlytestCtrl::ConlytestCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for ConlytestCtrl

BOOL ConlytestCtrl::ConlytestCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_ONLYTEST,
			IDB_ONLYTEST,
			afxRegApartmentThreading,
			_dwonlytestOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// ConlytestCtrl::ConlytestCtrl - Constructor

ConlytestCtrl::ConlytestCtrl()
{
	InitializeIIDs(&IID_Donlytest, &IID_DonlytestEvents);
	// TODO: Initialize your control's instance data here.
}



// ConlytestCtrl::~ConlytestCtrl - Destructor

ConlytestCtrl::~ConlytestCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// ConlytestCtrl::OnDraw - Drawing function

void ConlytestCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// ConlytestCtrl::DoPropExchange - Persistence support

void ConlytestCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// ConlytestCtrl::OnResetState - Reset control to default state

void ConlytestCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// ConlytestCtrl::AboutBox - Display an "About" box to the user

void ConlytestCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_ONLYTEST);
	dlgAbout.DoModal();
}



// ConlytestCtrl message handlers
