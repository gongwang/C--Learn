#pragma once

// FtpActiveXCtrl.h : Declaration of the CFtpActiveXCtrl ActiveX Control class.
#include <objSafe.h> 

// CFtpActiveXCtrl : See FtpActiveXCtrl.cpp for implementation.

#define MSG_UPLOAD_FILE_STATUS_NTY		30000	// 文件上传状态通知
#define MSG_UPLOAD_FILE_PROGRESS_NTY	30001	// 文件上传进度通知

#define EVENT_UPLOAD_FILE_START			1		// 上传开始
#define EVENT_UPLOAD_FILE_PROGRESS		2		// 上传进度
#define EVENT_UPLOAD_FILE_END			3		// 上传结束

struct TUploadFileInfo
{
	char m_szServerIp[64];
	unsigned long m_dwServerPort;
	char m_szUsername[64];
	char m_szPassword[64];
	char m_szLocalFile[256];
	char m_szRemoteFile[256];
	TUploadFileInfo()
	{
		memset(this, 0, sizeof(TUploadFileInfo));
	}
};

class CFtpAutoLock
{
public:
	CFtpAutoLock(HANDLE hEvent)
	{
		m_hEvent = NULL;
		if(hEvent)
		{
			m_hEvent = hEvent;
			WaitForSingleObject(m_hEvent, INFINITE);
		}
	};
	~CFtpAutoLock()
	{
		if(m_hEvent)
		{
			SetEvent(m_hEvent);
		}
	};
protected:
	HANDLE m_hEvent;
};
class CFtpActiveXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CFtpActiveXCtrl)
	//////////////////////////////////////////////////////////////////////////
	// ActiveX控件安全初始化：实现ISafeObject接口
	//////////////////////////////////////////////////////////////////////////
	//ISafeObject  
	DECLARE_INTERFACE_MAP()  
	BEGIN_INTERFACE_PART(ObjSafe, IObjectSafety)  
		STDMETHOD_(HRESULT, GetInterfaceSafetyOptions) (   
		/* [in] */ REFIID riid,  
		/* [out] */ DWORD __RPC_FAR *pdwSupportedOptions,  
		/* [out] */ DWORD __RPC_FAR *pdwEnabledOptions  
		);  

		STDMETHOD_(HRESULT, SetInterfaceSafetyOptions) (   
			/* [in] */ REFIID riid,  
			/* [in] */ DWORD dwOptionSetMask,  
			/* [in] */ DWORD dwEnabledOptions  
			);  
	END_INTERFACE_PART(ObjSafe);  
	//ISafeObject 

// Constructor
public:
	CFtpActiveXCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	void SetUploadFileInfo(const TUploadFileInfo &tUploadFileInfo);
	void GetUploadFileInfo(TUploadFileInfo &tUploadFileInfo);
	void SetIsUploadFile(BOOL bUplLoad);
	BOOL GetIsUploadFile();
	void SetUploadFileErrCode(int nErrorCode);
	int GetUploadFileErrCode();
// Implementation
protected:
	~CFtpActiveXCtrl();

	DECLARE_OLECREATE_EX(CFtpActiveXCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CFtpActiveXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CFtpActiveXCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CFtpActiveXCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT ProcUploadFileStatusNty(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcUploadFileProgressNty(WPARAM wParam, LPARAM lParam);

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		eventidNotify_UploadFileStatus = 3L,
		dispidFTP_GetVersion = 2L,
		dispidFTP_StartUploadFile = 1L
	};
protected:
	ULONG FTP_StartUploadFile(LPCTSTR szServerIp, ULONG nServerPort, LPCTSTR szUsername, LPCTSTR szPassword, LPCTSTR szLocalFile, LPCTSTR szRemoteFile);
	BSTR FTP_GetVersion(void);

	HANDLE m_hCommonEvent;
	TUploadFileInfo m_tUploadFileInfo;
	BOOL m_bIsUploadFile;
	int m_nErrorCode;
	int m_nUploadProgress;

	void Notify_UploadFileStatus(LONG nEvent, LONG nErrorCode, LONG nProgress, LPCTSTR strLocalFile, LPCTSTR strRemoteFile)
	{
		FireEvent(eventidNotify_UploadFileStatus, EVENT_PARAM(VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR), nEvent, nErrorCode, nProgress, strLocalFile, strRemoteFile);
	}
public:
	static void UploadFileProgressCB(int nEvent, unsigned long nHasTransSzie, unsigned long nTotalSzie, const void* userdata);
};

enum emFtpErrorCode
{
	em_Ftp_Err_Success = 0,			// 成功
	em_Ftp_Err_Param_Invalid,		// 参数错误
	em_Ftp_Err_Login_Failed,		// 登录失败
	em_Ftp_Err_Upload_Failed,		// 上传失败
	em_Ftp_Err_Exist_Uploading,		// 有文件正在上传
};

