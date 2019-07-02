// FtpActiveXPropPage.cpp : Implementation of the CFtpActiveXPropPage property page class.

#include "stdafx.h"
#include "FtpActiveX.h"
#include "FtpActiveXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFtpActiveXPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CFtpActiveXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CFtpActiveXPropPage, "FTPACTIVEX.FtpActiveXPropPage.1",
	0xe0ececec, 0xf94, 0x4f67, 0xa7, 0x20, 0xe6, 0x14, 0xa, 0x68, 0xd0, 0x96)



// CFtpActiveXPropPage::CFtpActiveXPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CFtpActiveXPropPage

BOOL CFtpActiveXPropPage::CFtpActiveXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_FTPACTIVEX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CFtpActiveXPropPage::CFtpActiveXPropPage - Constructor

CFtpActiveXPropPage::CFtpActiveXPropPage() :
	COlePropertyPage(IDD, IDS_FTPACTIVEX_PPG_CAPTION)
{
}



// CFtpActiveXPropPage::DoDataExchange - Moves data between page and properties

void CFtpActiveXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CFtpActiveXPropPage message handlers
