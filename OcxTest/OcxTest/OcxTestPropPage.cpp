// OcxTestPropPage.cpp : Implementation of the COcxTestPropPage property page class.

#include "stdafx.h"
#include "OcxTest.h"
#include "OcxTestPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(COcxTestPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(COcxTestPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(COcxTestPropPage, "OCXTEST.OcxTestPropPage.1",
	0x64d2aebc, 0x1476, 0x4fb8, 0xb1, 0x11, 0xa6, 0xfd, 0x5d, 0xb, 0x78, 0x3d)



// COcxTestPropPage::COcxTestPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for COcxTestPropPage

BOOL COcxTestPropPage::COcxTestPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_OCXTEST_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// COcxTestPropPage::COcxTestPropPage - Constructor

COcxTestPropPage::COcxTestPropPage() :
	COlePropertyPage(IDD, IDS_OCXTEST_PPG_CAPTION)
{
}



// COcxTestPropPage::DoDataExchange - Moves data between page and properties

void COcxTestPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// COcxTestPropPage message handlers
