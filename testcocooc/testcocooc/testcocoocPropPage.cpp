// testcocoocPropPage.cpp : Implementation of the CtestcocoocPropPage property page class.

#include "stdafx.h"
#include "testcocooc.h"
#include "testcocoocPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CtestcocoocPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CtestcocoocPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CtestcocoocPropPage, "TESTCOCOOC.testcocoocPropPage.1",
	0x62064309, 0x37fe, 0x475f, 0x80, 0x43, 0x66, 0x22, 0x36, 0xa5, 0x31, 0x6)



// CtestcocoocPropPage::CtestcocoocPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CtestcocoocPropPage

BOOL CtestcocoocPropPage::CtestcocoocPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_TESTCOCOOC_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CtestcocoocPropPage::CtestcocoocPropPage - Constructor

CtestcocoocPropPage::CtestcocoocPropPage() :
	COlePropertyPage(IDD, IDS_TESTCOCOOC_PPG_CAPTION)
{
}



// CtestcocoocPropPage::DoDataExchange - Moves data between page and properties

void CtestcocoocPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CtestcocoocPropPage message handlers
