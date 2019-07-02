// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDriveTimeGroup wrapper class

class CDriveTimeGroup : public COleDispatchDriver
{
public:
	CDriveTimeGroup(){} // Calls COleDispatchDriver default constructor
	CDriveTimeGroup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDriveTimeGroup(const CDriveTimeGroup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DriveTimeGroup methods
public:
	BOOL get_CheckModeGroup()
	{
		BOOL result;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_CheckModeGroup(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void SetCheckMode(long iWeekIndex, short TimeIndex, unsigned char lMode)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_UI1 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, iWeekIndex, TimeIndex, lMode);
	}
	unsigned char GetCheckMode(long iWeekIndex, short TimeIndex)
	{
		unsigned char result;
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_UI1, (void*)&result, parms, iWeekIndex, TimeIndex);
		return result;
	}
	void SetBeginTime(long iWeekIndex, short TimeIndex, DATE dTime)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_DATE ;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, iWeekIndex, TimeIndex, dTime);
	}
	void SetEndTime(long iWeekIndex, short TimeIndex, DATE dTime)
	{
		static BYTE parms[] = VTS_I4 VTS_I2 VTS_DATE ;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, iWeekIndex, TimeIndex, dTime);
	}
	void SetTimeGroupNum(short lNum)
	{
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lNum);
	}
	DATE GetBeginTime(long iWeekIndex, short TimeIndex)
	{
		DATE result;
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_DATE, (void*)&result, parms, iWeekIndex, TimeIndex);
		return result;
	}
	DATE GetEndTime(long iWeekIndex, short TimeIndex)
	{
		DATE result;
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_DATE, (void*)&result, parms, iWeekIndex, TimeIndex);
		return result;
	}
	short GetTimeGroupNum()
	{
		short result;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
		return result;
	}
	CString toString_FC8800(long iWeekIndex)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, iWeekIndex);
		return result;
	}
	CString toString_FC8800_All()
	{
		CString result;
		InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString toString_FC8000(LPCTSTR iIndex)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, iIndex);
		return result;
	}
	BOOL SetTimeHex_FC8800(LPCTSTR sHex)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sHex);
		return result;
	}
	BOOL SetTimeHex_FC8800_CheckMode(LPCTSTR sHex)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sHex);
		return result;
	}
	CString toString_FC8800_CheckMode()
	{
		CString result;
		InvokeHelper(0x60030010, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// _DriveTimeGroup properties
public:

};
