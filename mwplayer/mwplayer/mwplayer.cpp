// mwplayer.cpp : Implementation of CmwplayerApp and DLL registration.

#include "stdafx.h"
#include "mwplayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CmwplayerApp theApp;

const GUID CDECL _tlid = { 0x2BF9D73A, 0x294B, 0x488A, { 0xB1, 0xD3, 0x49, 0x13, 0x8, 0xF1, 0xC8, 0x6 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CmwplayerApp::InitInstance - DLL initialization

BOOL CmwplayerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CmwplayerApp::ExitInstance - DLL termination

int CmwplayerApp::ExitInstance()
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
