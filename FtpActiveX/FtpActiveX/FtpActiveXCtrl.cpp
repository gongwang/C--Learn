// FtpActiveXCtrl.cpp : Implementation of the CFtpActiveXCtrl ActiveX Control class.

#include "stdafx.h"
#include "FtpActiveX.h"
#include "FtpActiveXCtrl.h"
#include "FtpActiveXPropPage.h"
#include "afxdialogex.h"
#include "ftpmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CFtpActiveXCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CFtpActiveXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(MSG_UPLOAD_FILE_STATUS_NTY, &CFtpActiveXCtrl::ProcUploadFileStatusNty)
	ON_MESSAGE(MSG_UPLOAD_FILE_PROGRESS_NTY, &CFtpActiveXCtrl::ProcUploadFileProgressNty)
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CFtpActiveXCtrl, COleControl)
	DISP_FUNCTION_ID(CFtpActiveXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFtpActiveXCtrl, "FTP_StartUploadFile", dispidFTP_StartUploadFile, FTP_StartUploadFile, VT_UI4, VTS_BSTR VTS_UI4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CFtpActiveXCtrl, "FTP_GetVersion", dispidFTP_GetVersion, FTP_GetVersion, VT_BSTR, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CFtpActiveXCtrl, COleControl)
	EVENT_CUSTOM_ID("Notify_UploadFileStatus", eventidNotify_UploadFileStatus, Notify_UploadFileStatus, VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CFtpActiveXCtrl, 1)
	PROPPAGEID(CFtpActiveXPropPage::guid)
END_PROPPAGEIDS(CFtpActiveXCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CFtpActiveXCtrl, "FTPACTIVEX.FtpActiveXCtrl.1",
	0xf770acf3, 0x7bd1, 0x4d3a, 0xb5, 0x7d, 0x52, 0x37, 0x6d, 0x61, 0x89, 0xd3)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CFtpActiveXCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID IID_DFtpActiveX = { 0xCDEF82FB, 0x13BE, 0x4B83, { 0xB1, 0x3C, 0xC, 0xA2, 0x56, 0x2C, 0xF1, 0x6E } };
const IID IID_DFtpActiveXEvents = { 0xC13A54AB, 0x5E27, 0x47D4, { 0xB8, 0x99, 0x8E, 0x86, 0x5B, 0x20, 0xA5, 0x92 } };


// Control type information

static const DWORD _dwFtpActiveXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CFtpActiveXCtrl, IDS_FTPACTIVEX, _dwFtpActiveXOleMisc)



// CFtpActiveXCtrl::CFtpActiveXCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CFtpActiveXCtrl

BOOL CFtpActiveXCtrl::CFtpActiveXCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_FTPACTIVEX,
			IDB_FTPACTIVEX,
			afxRegApartmentThreading,
			_dwFtpActiveXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CFtpActiveXCtrl::CFtpActiveXCtrl - Constructor

CFtpActiveXCtrl::CFtpActiveXCtrl()
{
	InitializeIIDs(&IID_DFtpActiveX, &IID_DFtpActiveXEvents);
	// TODO: Initialize your control's instance data here.

	m_hCommonEvent = CreateEvent(NULL,FALSE,TRUE,NULL); //使用手动重置为无信号状态，初始化时有信号状态
	m_bIsUploadFile = FALSE;
	m_nErrorCode = em_Ftp_Err_Success;
	m_nUploadProgress = 0;
}



// CFtpActiveXCtrl::~CFtpActiveXCtrl - Destructor

CFtpActiveXCtrl::~CFtpActiveXCtrl()
{
	// TODO: Cleanup your control's instance data here.
	CloseHandle(m_hCommonEvent);
}



// CFtpActiveXCtrl::OnDraw - Drawing function

void CFtpActiveXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CFtpActiveXCtrl::DoPropExchange - Persistence support

void CFtpActiveXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CFtpActiveXCtrl::OnResetState - Reset control to default state

void CFtpActiveXCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}

void CFtpActiveXCtrl::SetUploadFileInfo(const TUploadFileInfo &tUploadFileInfo)
{
	CFtpAutoLock cLock(m_hCommonEvent);
	memcpy(&m_tUploadFileInfo, &tUploadFileInfo, sizeof(m_tUploadFileInfo));
}
void CFtpActiveXCtrl::GetUploadFileInfo(TUploadFileInfo &tUploadFileInfo)
{
	CFtpAutoLock cLock(m_hCommonEvent);
	memcpy(&tUploadFileInfo, &m_tUploadFileInfo, sizeof(tUploadFileInfo));
}
void CFtpActiveXCtrl::SetIsUploadFile(BOOL bUpload)
{
	CFtpAutoLock cLock(m_hCommonEvent);
	m_bIsUploadFile = bUpload;
}
BOOL CFtpActiveXCtrl::GetIsUploadFile()
{
	CFtpAutoLock cLock(m_hCommonEvent);
	return m_bIsUploadFile;
}
void CFtpActiveXCtrl::SetUploadFileErrCode(int nErrorCode)
{
	CFtpAutoLock cLock(m_hCommonEvent);
	m_nErrorCode = nErrorCode;
}
int CFtpActiveXCtrl::GetUploadFileErrCode()
{
	CFtpAutoLock cLock(m_hCommonEvent);
	return m_nErrorCode;
}
// CFtpActiveXCtrl::AboutBox - Display an "About" box to the user
LRESULT CFtpActiveXCtrl::ProcUploadFileStatusNty(WPARAM wParam, LPARAM lParam)
{
	long nProgress = (long)wParam;
	long nEvent = (long)lParam;

	TUploadFileInfo tUploadFileInfo;
	int nError = 0;

	GetUploadFileInfo(tUploadFileInfo);
	nError = GetUploadFileErrCode();

	char szInfo[256] = {0};
	sprintf(szInfo, "Notify_UploadFileStatus. nError:%d LocalFile:%s RemoteFile:%s", nError, tUploadFileInfo.m_szLocalFile, tUploadFileInfo.m_szRemoteFile);
	OutputDebugString(szInfo);
	Notify_UploadFileStatus(nEvent, nError, nProgress, tUploadFileInfo.m_szLocalFile, tUploadFileInfo.m_szRemoteFile);

	return 0;
}
LRESULT CFtpActiveXCtrl::ProcUploadFileProgressNty(WPARAM wParam, LPARAM lParam)
{
	long nProgress = (long)wParam;
	long nEvent = (long)lParam;
	TUploadFileInfo tUploadFileInfo;
	int nError = 0;
	GetUploadFileInfo(tUploadFileInfo);
	nError = GetUploadFileErrCode();

	char szInfo[256] = {0};
	sprintf(szInfo, "ProcUploadFileProgressNty. nProgress:%d", nProgress);
	OutputDebugString(szInfo);

	Notify_UploadFileStatus(nEvent, nError, nProgress, tUploadFileInfo.m_szLocalFile, tUploadFileInfo.m_szRemoteFile);
	return 0;
}
void CFtpActiveXCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_FTPACTIVEX);
	dlgAbout.DoModal();
}



// CFtpActiveXCtrl message handlers

void CFtpActiveXCtrl::UploadFileProgressCB(int nEvent, unsigned long nHasTransSzie, unsigned long nTotalSzie, const void* userdata)
{
	CFtpActiveXCtrl* pFtpCtrl = (CFtpActiveXCtrl*)userdata;
	if(pFtpCtrl == NULL)
	{
		return ;
	}
	int nProgress = 0;
	// nEvent: 1-开始上传  2-上传进度  3-上传结束
	if(nEvent == 1)
	{
		nEvent = EVENT_UPLOAD_FILE_START;
	}
	if(nEvent == 2)
	{
		nEvent = EVENT_UPLOAD_FILE_PROGRESS;
		nProgress = (float)nHasTransSzie / nTotalSzie * 100;
	}
	else if(nEvent == 3)
	{
		nEvent = EVENT_UPLOAD_FILE_END;
		nProgress = 100;
	}
	else
	{
		return ;
	}

	if(pFtpCtrl->GetSafeHwnd())
	{
		if(nEvent == EVENT_UPLOAD_FILE_END || pFtpCtrl->m_nUploadProgress != nProgress)
		{
			pFtpCtrl->m_nUploadProgress = nProgress;
			::PostMessage(pFtpCtrl->GetSafeHwnd(), MSG_UPLOAD_FILE_PROGRESS_NTY, nProgress, nEvent);
		}
	}
}
DWORD WINAPI UploadFileThread(LPVOID lpParam)
{
	CFtpActiveXCtrl* pFtpCtrl = (CFtpActiveXCtrl*)lpParam;
	if(pFtpCtrl == NULL)
	{
		return 0;
	}

	do
	{
		TUploadFileInfo tUploadFileInfo;
		pFtpCtrl->GetUploadFileInfo(tUploadFileInfo);

		CFTPManager cFTPManager;
		FTP_API nRet = 0;

		char achServerAddr[256] = {0};
		_snprintf(achServerAddr, sizeof(achServerAddr) - 1, "%s:%d", tUploadFileInfo.m_szServerIp, tUploadFileInfo.m_dwServerPort);
		// 登录
		cFTPManager.login2Server(achServerAddr, tUploadFileInfo.m_szUsername, tUploadFileInfo.m_szPassword);
		if(nRet < 0)
		{
			pFtpCtrl->SetUploadFileErrCode(em_Ftp_Err_Exist_Uploading);
			break;
		}

		// 删除已存在文件
		nRet = cFTPManager.DeleteRemoteFile(tUploadFileInfo.m_szRemoteFile);

		// 上传文件
		nRet = cFTPManager.Put(tUploadFileInfo.m_szRemoteFile, tUploadFileInfo.m_szLocalFile, CFtpActiveXCtrl::UploadFileProgressCB, pFtpCtrl);
		if(nRet < 0)
		{
			pFtpCtrl->SetUploadFileErrCode(em_Ftp_Err_Upload_Failed);
			break;
		}

		// 断开连接
		cFTPManager.quitServer();

		pFtpCtrl->SetUploadFileErrCode(em_Ftp_Err_Success);

	}while(FALSE);

	pFtpCtrl->SetIsUploadFile(FALSE);
	if(pFtpCtrl->GetSafeHwnd() && pFtpCtrl->GetUploadFileErrCode() != em_Ftp_Err_Success)
	{
		PostMessage(pFtpCtrl->GetSafeHwnd(), MSG_UPLOAD_FILE_STATUS_NTY, 100, EVENT_UPLOAD_FILE_END);
	}

	return 0;
}

ULONG CFtpActiveXCtrl::FTP_StartUploadFile(LPCTSTR szServerIp, ULONG nServerPort, LPCTSTR szUsername, LPCTSTR szPassword, LPCTSTR szLocalFile, LPCTSTR szRemoteFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: Add your dispatch handler code here
	if(GetIsUploadFile())
	{
		return em_Ftp_Err_Exist_Uploading;
	}
	SetIsUploadFile(TRUE);

	TUploadFileInfo tUploadFileInfo;
	strncpy(tUploadFileInfo.m_szServerIp, szServerIp, sizeof(tUploadFileInfo.m_szServerIp) - 1);
	tUploadFileInfo.m_dwServerPort = nServerPort;
	strncpy(tUploadFileInfo.m_szUsername, szUsername, sizeof(tUploadFileInfo.m_szUsername) - 1);
	strncpy(tUploadFileInfo.m_szPassword, szPassword, sizeof(tUploadFileInfo.m_szPassword) - 1);
	strncpy(tUploadFileInfo.m_szLocalFile, szLocalFile, sizeof(tUploadFileInfo.m_szLocalFile) - 1);
	strncpy(tUploadFileInfo.m_szRemoteFile, szRemoteFile, sizeof(tUploadFileInfo.m_szRemoteFile) - 1);
	SetUploadFileInfo(tUploadFileInfo);

	DWORD dwThreadId = 0;
	HANDLE hHandle = CreateThread(NULL, 0, UploadFileThread, this, 0, &dwThreadId);
	CloseHandle(hHandle);

	return em_Ftp_Err_Success;
}


BSTR CFtpActiveXCtrl::FTP_GetVersion(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: Add your dispatch handler code here
	strResult = "Ver.Ftp 190227";

	return strResult.AllocSysString();
}


//////////////////////////////////////////////////////////////////////////
// ActiveX控件安全初始化：实现ISafeObject接口
//////////////////////////////////////////////////////////////////////////
// Interface map for IObjectSafety  
BEGIN_INTERFACE_MAP( CFtpActiveXCtrl, COleControl )  
	INTERFACE_PART(CFtpActiveXCtrl, IID_IObjectSafety, ObjSafe)  
END_INTERFACE_MAP()
// IObjectSafety member functions  
// Delegate AddRef, Release, QueryInterface  
ULONG FAR EXPORT CFtpActiveXCtrl::XObjSafe::AddRef()  
{  
	METHOD_PROLOGUE(CFtpActiveXCtrl, ObjSafe)  
		return pThis->ExternalAddRef();  
}  
ULONG FAR EXPORT CFtpActiveXCtrl::XObjSafe::Release()  
{  
	METHOD_PROLOGUE(CFtpActiveXCtrl, ObjSafe)  
		return pThis->ExternalRelease();  
}  
HRESULT FAR EXPORT CFtpActiveXCtrl::XObjSafe::QueryInterface(  
	REFIID iid, void FAR* FAR* ppvObj)  
{  
	METHOD_PROLOGUE(CFtpActiveXCtrl, ObjSafe)  
		return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);  
}  
const DWORD dwSupportedBits =   
	INTERFACESAFE_FOR_UNTRUSTED_CALLER |  
	INTERFACESAFE_FOR_UNTRUSTED_DATA;  
const DWORD dwNotSupportedBits = ~ dwSupportedBits;  
//.............................................................................  
// CStopLiteCtrl::XObjSafe::GetInterfaceSafetyOptions  
// Allows container to query what interfaces are safe for what. We're  
// optimizing significantly by ignoring which interface the caller is  
// asking for.  
HRESULT STDMETHODCALLTYPE   
	CFtpActiveXCtrl::XObjSafe::GetInterfaceSafetyOptions(   
	/* [in] */ REFIID riid,  
	/* [out] */ DWORD __RPC_FAR *pdwSupportedOptions,  
	/* [out] */ DWORD __RPC_FAR *pdwEnabledOptions)  
{  
	METHOD_PROLOGUE(CFtpActiveXCtrl, ObjSafe)  
		HRESULT retval = ResultFromScode(S_OK);  
	// does interface exist?  
	IUnknown FAR* punkInterface;  
	retval = pThis->ExternalQueryInterface(&riid,   
		(void * *)&punkInterface);  
	if (retval != E_NOINTERFACE) { // interface exists  
		punkInterface->Release(); // release it--just checking!  
	}  

	// we support both kinds of safety and have always both set,  
	// regardless of interface  
	*pdwSupportedOptions = *pdwEnabledOptions = dwSupportedBits;  
	return retval; // E_NOINTERFACE if QI failed  
}  
/////////////////////////////////////////////////////////////////////////////  
// CStopLiteCtrl::XObjSafe::SetInterfaceSafetyOptions  
// Since we're always safe, this is a no-brainer--but we do check to make  
// sure the interface requested exists and that the options we're asked to  
// set exist and are set on (we don't support unsafe mode).  
HRESULT STDMETHODCALLTYPE   
	CFtpActiveXCtrl::XObjSafe::SetInterfaceSafetyOptions(   
	/* [in] */ REFIID riid,  
	/* [in] */ DWORD dwOptionSetMask,  
	/* [in] */ DWORD dwEnabledOptions)  
{  
	METHOD_PROLOGUE(CFtpActiveXCtrl, ObjSafe)  

		// does interface exist?  
		IUnknown FAR* punkInterface;  
	pThis->ExternalQueryInterface(&riid, (void * *)&punkInterface);  
	if (punkInterface) { // interface exists  
		punkInterface->Release(); // release it--just checking!  
	}  
	else { // interface doesn't exist  
		return ResultFromScode(E_NOINTERFACE);  
	}  
	// can't set bits we don't support  
	if (dwOptionSetMask & dwNotSupportedBits) {   
		return ResultFromScode(E_FAIL);  
	}  

	// can't set bits we do support to zero  
	dwEnabledOptions &= dwSupportedBits;  
	// (we already know there are no extra bits in mask )  
	if ((dwOptionSetMask & dwEnabledOptions) !=  
		dwOptionSetMask) {  
			return ResultFromScode(E_FAIL);  
	}          

	// don't need to change anything since we're always safe  
	return ResultFromScode(S_OK);  
}  
//////////////////////////////////////////////////////////////////////////