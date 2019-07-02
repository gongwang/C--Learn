// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDrivePassword wrapper class

class CDrivePassword : public COleDispatchDriver
{
public:
	CDrivePassword(){} // Calls COleDispatchDriver default constructor
	CDrivePassword(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDrivePassword(const CDrivePassword& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DrivePassword methods
public:
	BOOL get_ClickButton()
	{
		BOOL result;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ClickButton(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void SetPassword(LPCTSTR vNew)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	CString GetPassword()
	{
		CString result;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long GetPwr(short vDoorNum)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_I4, (void*)&result, parms, vDoorNum);
		return result;
	}
	void SetPwr(short vDoorNum, long iPwr)
	{
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vDoorNum, iPwr);
	}
	CString GetPasswordPwrStr()
	{
		CString result;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString ToString()
	{
		CString result;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString toString_FC8800()
	{
		CString result;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetPasswordHex_FC8800(LPCTSTR sHex)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sHex);
	}
	CString toString_FC8832()
	{
		CString result;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetPasswordHex_FC8832(LPCTSTR sHex)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sHex);
	}

	// _DrivePassword properties
public:

};
