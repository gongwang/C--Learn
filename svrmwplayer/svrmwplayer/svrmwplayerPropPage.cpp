// svrmwplayerPropPage.cpp : Implementation of the CsvrmwplayerPropPage property page class.

#include "stdafx.h"
#include "svrmwplayer.h"
#include "svrmwplayerPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CsvrmwplayerPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CsvrmwplayerPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CsvrmwplayerPropPage, "SVRMWPLAYER.svrmwplayerPropPage.1",
	0x8d180548, 0xab0b, 0x419d, 0xb6, 0x14, 0xfb, 0x35, 0xda, 0x45, 0x96, 0xb1)



// CsvrmwplayerPropPage::CsvrmwplayerPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CsvrmwplayerPropPage

BOOL CsvrmwplayerPropPage::CsvrmwplayerPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SVRMWPLAYER_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CsvrmwplayerPropPage::CsvrmwplayerPropPage - Constructor

CsvrmwplayerPropPage::CsvrmwplayerPropPage() :
	COlePropertyPage(IDD, IDS_SVRMWPLAYER_PPG_CAPTION)
{
}



// CsvrmwplayerPropPage::DoDataExchange - Moves data between page and properties

void CsvrmwplayerPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CsvrmwplayerPropPage message handlers
