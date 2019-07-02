// OcxTest.cpp : Implementation of COcxTestApp and DLL registration.

#include "stdafx.h"
#include "OcxTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


COcxTestApp theApp;

const GUID CDECL _tlid = { 0xE549B9D8, 0x8A72, 0x4D03, { 0x87, 0x47, 0x7C, 0x62, 0x8E, 0x8B, 0x93, 0x64 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// COcxTestApp::InitInstance - DLL initialization

BOOL COcxTestApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.	
//		MessageBox(NULL,_T("hello coco"),MB_OK,NULL);
	}

	return bInit;
}



// COcxTestApp::ExitInstance - DLL termination

int COcxTestApp::ExitInstance()
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
