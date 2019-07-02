// svrmwplayer.cpp : Implementation of CsvrmwplayerApp and DLL registration.

#include "stdafx.h"
#include "svrmwplayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CsvrmwplayerApp theApp;

const GUID CDECL _tlid = { 0xE0F6C8A5, 0x1369, 0x4A1E, { 0x86, 0x37, 0xD7, 0x58, 0x8B, 0xC0, 0x6D, 0xA2 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CsvrmwplayerApp::InitInstance - DLL initialization

BOOL CsvrmwplayerApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CsvrmwplayerApp::ExitInstance - DLL termination

int CsvrmwplayerApp::ExitInstance()
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
