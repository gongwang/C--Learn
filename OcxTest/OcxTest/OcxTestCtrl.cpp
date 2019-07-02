// OcxTestCtrl.cpp : Implementation of the COcxTestCtrl ActiveX Control class.

#include "stdafx.h"
#include "OcxTest.h"
#include "OcxTestCtrl.h"
#include "OcxTestPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(COcxTestCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(COcxTestCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(COcxTestCtrl, COleControl)
	DISP_FUNCTION_ID(COcxTestCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_NOTIFY_ID(COcxTestCtrl, "TimeStyle", dispidTimeStyle, m_TimeStyle, OnTimeStyleChanged, VT_I4)
	DISP_FUNCTION_ID(COcxTestCtrl, "Start", dispidStart, Start, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(COcxTestCtrl, COleControl)
	EVENT_CUSTOM_ID("OnSend", eventidOnSend, FileOnSend, VTS_BSTR)
	EVENT_STOCK_CLICK()
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(COcxTestCtrl, 1)
	PROPPAGEID(COcxTestPropPage::guid)
END_PROPPAGEIDS(COcxTestCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(COcxTestCtrl, "OCXTEST.OcxTestCtrl.1",
	0x92f94649, 0x535a, 0x4c1e, 0xbe, 0xb8, 0xa4, 0xf7, 0xc2, 0xef, 0xdd, 0x26)



// Type library ID and version

IMPLEMENT_OLETYPELIB(COcxTestCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID IID_DOcxTest = { 0x905E7170, 0x806B, 0x477C, { 0x93, 0xE9, 0xA6, 0x5A, 0x1F, 0xC4, 0x7E, 0xAF } };
const IID IID_DOcxTestEvents = { 0x56AFC954, 0x4EEB, 0x43B4, { 0x92, 0xCF, 0x24, 0x41, 0xBE, 0x36, 0xFA, 0x6B } };


// Control type information

static const DWORD _dwOcxTestOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(COcxTestCtrl, IDS_OCXTEST, _dwOcxTestOleMisc)



// COcxTestCtrl::COcxTestCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for COcxTestCtrl

BOOL COcxTestCtrl::COcxTestCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_OCXTEST,
			IDB_OCXTEST,
			afxRegApartmentThreading,
			_dwOcxTestOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// COcxTestCtrl::COcxTestCtrl - Constructor

COcxTestCtrl::COcxTestCtrl()
{
	InitializeIIDs(&IID_DOcxTest, &IID_DOcxTestEvents);
	// TODO: Initialize your control's instance data here.
	m_TimeStyle = 2;
}



// COcxTestCtrl::~COcxTestCtrl - Destructor

COcxTestCtrl::~COcxTestCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// COcxTestCtrl::OnDraw - Drawing function

void COcxTestCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// COcxTestCtrl::DoPropExchange - Persistence support

void COcxTestCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// COcxTestCtrl::OnResetState - Reset control to default state

void COcxTestCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// COcxTestCtrl::AboutBox - Display an "About" box to the user

void COcxTestCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_OCXTEST);
	dlgAbout.DoModal();
}



// COcxTestCtrl message handlers


void COcxTestCtrl::OnTimeStyleChanged(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your property handler code here

	SetModifiedFlag();
}


void COcxTestCtrl::Start(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	ShowTime();

    //ActiveX 嵌入MFC界面
	int iRet = myUI.DoModal();

	FileOnSend(_T("123"));
}

void COcxTestCtrl::ShowTime()
{
	SYSTEMTIME sysTime;
	CString CsTime;

	//m_TimeStyle = 2;

	GetLocalTime(&sysTime);
	if (m_TimeStyle == 0)
	{
		CsTime.Format(_T("%4d年%d月%d日%d时%d分%d秒"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wMilliseconds);
	}else if (m_TimeStyle == 1)
	{
		CsTime.Format(_T("%4d-%d-%d-%d-%d-%d"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wMilliseconds);
	}else if (m_TimeStyle == 2)
	{
		CsTime.Format(_T("%4d/%d/%d %d:%d:%d"),sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wMilliseconds);
	}
	MessageBox(CsTime);
}