// onlytest.cpp : Implementation of ConlytestApp and DLL registration.

#include "stdafx.h"
#include "onlytest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConlytestApp theApp;

const GUID CDECL _tlid = { 0xFAE33989, 0x6CA1, 0x47B7, { 0xBB, 0xBD, 0x60, 0x89, 0xA, 0x9B, 0x34, 0xE8 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// ConlytestApp::InitInstance - DLL initialization

BOOL ConlytestApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// ConlytestApp::ExitInstance - DLL termination

int ConlytestApp::ExitInstance()
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
