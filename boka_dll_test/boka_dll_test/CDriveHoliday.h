// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDriveHoliday wrapper class

class CDriveHoliday : public COleDispatchDriver
{
public:
	CDriveHoliday(){} // Calls COleDispatchDriver default constructor
	CDriveHoliday(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDriveHoliday(const CDriveHoliday& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DriveHoliday methods
public:
	void SetYearly(BOOL bData)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bData);
	}
	BOOL GetYearly()
	{
		BOOL result;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetDate(DATE nDate)
	{
		static BYTE parms[] = VTS_DATE ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, nDate);
	}
	DATE GetDate()
	{
		DATE result;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_DATE, (void*)&result, NULL);
		return result;
	}
	void SetHolLen(long lh)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lh);
	}
	long GetHolLen()
	{
		long result;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetHolNum(short lh)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lh);
	}
	short GetHolNum()
	{
		short result;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	CString ToString()
	{
		CString result;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString toString_Hol()
	{
		CString result;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString toString_FC8800()
	{
		CString result;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetHolHex_FC8800(LPCTSTR sHex)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sHex);
	}

	// _DriveHoliday properties
public:

};
