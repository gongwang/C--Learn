// svrmwplayerCtrl.cpp : Implementation of the CsvrmwplayerCtrl ActiveX Control class.

#include "stdafx.h"
#include "svrmwplayer.h"
#include "svrmwplayerCtrl.h"
#include "svrmwplayerPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CsvrmwplayerCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CsvrmwplayerCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CsvrmwplayerCtrl, COleControl)
	DISP_FUNCTION_ID(CsvrmwplayerCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CsvrmwplayerCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CsvrmwplayerCtrl, 1)
	PROPPAGEID(CsvrmwplayerPropPage::guid)
END_PROPPAGEIDS(CsvrmwplayerCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CsvrmwplayerCtrl, "SVRMWPLAYER.svrmwplayerCtrl.1",
	0xe804f18d, 0x1c55, 0x487e, 0xb7, 0xf5, 0x46, 0x69, 0x2, 0x6f, 0x8c, 0x61)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CsvrmwplayerCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID IID_Dsvrmwplayer = { 0xD3AB8013, 0x7AED, 0x46CC, { 0xB0, 0xEE, 0x2B, 0x94, 0x9B, 0xEB, 0x15, 0xEA } };
const IID IID_DsvrmwplayerEvents = { 0xA0D18D48, 0x22EB, 0x4A76, { 0xAA, 0xAA, 0x1A, 0xB6, 0x4E, 0x78, 0x93, 0x20 } };


// Control type information

static const DWORD _dwsvrmwplayerOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CsvrmwplayerCtrl, IDS_SVRMWPLAYER, _dwsvrmwplayerOleMisc)



// CsvrmwplayerCtrl::CsvrmwplayerCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CsvrmwplayerCtrl

BOOL CsvrmwplayerCtrl::CsvrmwplayerCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SVRMWPLAYER,
			IDB_SVRMWPLAYER,
			afxRegApartmentThreading,
			_dwsvrmwplayerOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CsvrmwplayerCtrl::CsvrmwplayerCtrl - Constructor

CsvrmwplayerCtrl::CsvrmwplayerCtrl()
{
	InitializeIIDs(&IID_Dsvrmwplayer, &IID_DsvrmwplayerEvents);
	// TODO: Initialize your control's instance data here.
}



// CsvrmwplayerCtrl::~CsvrmwplayerCtrl - Destructor

CsvrmwplayerCtrl::~CsvrmwplayerCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CsvrmwplayerCtrl::OnDraw - Drawing function

void CsvrmwplayerCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CsvrmwplayerCtrl::DoPropExchange - Persistence support

void CsvrmwplayerCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CsvrmwplayerCtrl::OnResetState - Reset control to default state

void CsvrmwplayerCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CsvrmwplayerCtrl::AboutBox - Display an "About" box to the user

void CsvrmwplayerCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_SVRMWPLAYER);
	dlgAbout.DoModal();
}



// CsvrmwplayerCtrl message handlers
