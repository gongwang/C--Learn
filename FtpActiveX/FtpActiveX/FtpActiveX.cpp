// FtpActiveX.cpp : Implementation of CFtpActiveXApp and DLL registration.

#include "stdafx.h"
#include "FtpActiveX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFtpActiveXApp theApp;

const GUID CDECL _tlid = { 0x36407BDF, 0xD73C, 0x4A0E, { 0xA1, 0xD9, 0xC4, 0x57, 0x4F, 0x84, 0xB8, 0x9A } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CFtpActiveXApp::InitInstance - DLL initialization

BOOL CFtpActiveXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
		WSADATA wsaData;
		WSAStartup( MAKEWORD(2, 2), &wsaData);
	}

	return bInit;
}



// CFtpActiveXApp::ExitInstance - DLL termination

int CFtpActiveXApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
