// mwplayerPropPage.cpp : Implementation of the CmwplayerPropPage property page class.

#include "stdafx.h"
#include "mwplayer.h"
#include "mwplayerPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CmwplayerPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CmwplayerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CmwplayerPropPage, "MWPLAYER.mwplayerPropPage.1",
	0x4607aa1a, 0x1807, 0x463e, 0xb7, 0xc3, 0x22, 0xa9, 0xe5, 0x16, 0x81, 0xe5)



// CmwplayerPropPage::CmwplayerPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CmwplayerPropPage

BOOL CmwplayerPropPage::CmwplayerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_MWPLAYER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CmwplayerPropPage::CmwplayerPropPage - Constructor

CmwplayerPropPage::CmwplayerPropPage() :
	COlePropertyPage(IDD, IDS_MWPLAYER_PPG_CAPTION)
{
}



// CmwplayerPropPage::DoDataExchange - Moves data between page and properties

void CmwplayerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CmwplayerPropPage message handlers
