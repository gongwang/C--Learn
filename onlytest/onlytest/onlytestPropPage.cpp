// onlytestPropPage.cpp : Implementation of the ConlytestPropPage property page class.

#include "stdafx.h"
#include "onlytest.h"
#include "onlytestPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(ConlytestPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(ConlytestPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(ConlytestPropPage, "ONLYTEST.onlytestPropPage.1",
	0xba9079f8, 0x86c0, 0x4014, 0xa0, 0xa0, 0x35, 0x88, 0xf9, 0x9e, 0x3d, 0x65)



// ConlytestPropPage::ConlytestPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for ConlytestPropPage

BOOL ConlytestPropPage::ConlytestPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ONLYTEST_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// ConlytestPropPage::ConlytestPropPage - Constructor

ConlytestPropPage::ConlytestPropPage() :
	COlePropertyPage(IDD, IDS_ONLYTEST_PPG_CAPTION)
{
}



// ConlytestPropPage::DoDataExchange - Moves data between page and properties

void ConlytestPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// ConlytestPropPage message handlers
