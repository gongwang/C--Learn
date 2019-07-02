// mwplayerCtrl.cpp : Implementation of the CmwplayerCtrl ActiveX Control class.

#include "stdafx.h"
#include "mwplayer.h"
#include "mwplayerCtrl.h"
#include "mwplayerPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CmwplayerCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CmwplayerCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CmwplayerCtrl, COleControl)
	DISP_FUNCTION_ID(CmwplayerCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CmwplayerCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CmwplayerCtrl, 1)
	PROPPAGEID(CmwplayerPropPage::guid)
END_PROPPAGEIDS(CmwplayerCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CmwplayerCtrl, "MWPLAYER.mwplayerCtrl.1",
	0xa9e2c003, 0x1be3, 0x40aa, 0xba, 0x1b, 0x77, 0xb7, 0x87, 0xe3, 0x81, 0xcc)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CmwplayerCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID IID_Dmwplayer = { 0x3681B6D3, 0x533D, 0x4D68, { 0x8E, 0xC7, 0x6A, 0x2A, 0x2, 0x4, 0x48, 0xD4 } };
const IID IID_DmwplayerEvents = { 0xFA246079, 0x4248, 0x48E4, { 0xAA, 0x26, 0xAA, 0x15, 0x34, 0x37, 0x24, 0x37 } };


// Control type information

static const DWORD _dwmwplayerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CmwplayerCtrl, IDS_MWPLAYER, _dwmwplayerOleMisc)



// CmwplayerCtrl::CmwplayerCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CmwplayerCtrl

BOOL CmwplayerCtrl::CmwplayerCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_MWPLAYER,
			IDB_MWPLAYER,
			afxRegApartmentThreading,
			_dwmwplayerOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CmwplayerCtrl::CmwplayerCtrl - Constructor

CmwplayerCtrl::CmwplayerCtrl()
{
	InitializeIIDs(&IID_Dmwplayer, &IID_DmwplayerEvents);
	// TODO: Initialize your control's instance data here.
}



// CmwplayerCtrl::~CmwplayerCtrl - Destructor

CmwplayerCtrl::~CmwplayerCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CmwplayerCtrl::OnDraw - Drawing function

void CmwplayerCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CmwplayerCtrl::DoPropExchange - Persistence support

void CmwplayerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CmwplayerCtrl::OnResetState - Reset control to default state

void CmwplayerCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CmwplayerCtrl::AboutBox - Display an "About" box to the user

void CmwplayerCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_MWPLAYER);
	dlgAbout.DoModal();
}



// CmwplayerCtrl message handlers
