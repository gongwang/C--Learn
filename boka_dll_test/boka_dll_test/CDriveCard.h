// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDriveCard wrapper class

class CDriveCard : public COleDispatchDriver
{
public:
	CDriveCard(){} // Calls COleDispatchDriver default constructor
	CDriveCard(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDriveCard(const CDriveCard& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DriveCard methods
public:
	BOOL get_ClickButton()
	{
		BOOL result;
		InvokeHelper(0x68030001, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ClickButton(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x68030001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned char get_MuchReader()
	{
		unsigned char result;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_UI1, (void*)&result, NULL);
		return result;
	}
	void put_MuchReader(unsigned char newValue)
	{
		static BYTE parms[] = VTS_UI1 ;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void SetCardData(double vNew)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	double GetCardData()
	{
		double result;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	void SetCardNum(LPCTSTR vNew)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	CString GetCardNum()
	{
		CString result;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetPassword(LPCTSTR vNew)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	CString GetPassword()
	{
		CString result;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetUsePassword(BOOL vNew)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	BOOL GetUsePassword()
	{
		BOOL result;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetName(LPCTSTR vNew)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	CString GetName()
	{
		CString result;
		InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetUseHoliday(BOOL vNew)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	BOOL GetUseHoliday()
	{
		BOOL result;
		InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetDoorInOutState(short vDoorNum, long vNew)
	{
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0x6003000f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vDoorNum, vNew);
	}
	long GetDoorInOutState(short vDoorNum)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030010, DISPATCH_METHOD, VT_I4, (void*)&result, parms, vDoorNum);
		return result;
	}
	long GetDoorPwr(short vDoorNum)
	{
		long result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030011, DISPATCH_METHOD, VT_I4, (void*)&result, parms, vDoorNum);
		return result;
	}
	void SetDoorPwr(short vDoorNum, long iPwr)
	{
		static BYTE parms[] = VTS_I2 VTS_I4 ;
		InvokeHelper(0x60030012, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vDoorNum, iPwr);
	}
	CString GetDoorPwrStr()
	{
		CString result;
		InvokeHelper(0x60030013, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetDoorPwrStr(LPCTSTR sPwrStr)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030014, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sPwrStr);
	}
	void SetTimeLimit(DATE vNew)
	{
		static BYTE parms[] = VTS_DATE ;
		InvokeHelper(0x60030015, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	DATE GetTimeLimit()
	{
		DATE result;
		InvokeHelper(0x60030016, DISPATCH_METHOD, VT_DATE, (void*)&result, NULL);
		return result;
	}
	short GetTimeGroup(short iPort)
	{
		short result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030017, DISPATCH_METHOD, VT_I2, (void*)&result, parms, iPort);
		return result;
	}
	void SetTimeGroup(short iPort, short vNew)
	{
		static BYTE parms[] = VTS_I2 VTS_I2 ;
		InvokeHelper(0x60030018, DISPATCH_METHOD, VT_EMPTY, NULL, parms, iPort, vNew);
	}
	long GetReadCount()
	{
		long result;
		InvokeHelper(0x60030019, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetReadCount(long vNew)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6003001a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	BOOL GetFirstCard()
	{
		BOOL result;
		InvokeHelper(0x6003001b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetFirstCard(BOOL vNew)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6003001c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	BOOL GetLongOpen()
	{
		BOOL result;
		InvokeHelper(0x6003001d, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetLongOpen(BOOL vNew)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6003001e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	BOOL GetPatrol()
	{
		BOOL result;
		InvokeHelper(0x6003001f, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetPatrol(BOOL vNew)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030020, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	BOOL GetTheftSeting()
	{
		BOOL result;
		InvokeHelper(0x60030021, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetTheftSeting(BOOL vNew)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030022, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	BOOL GetManageCard()
	{
		BOOL result;
		InvokeHelper(0x60030033, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void SetManageCard(BOOL vNew)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030034, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vNew);
	}
	long GetState()
	{
		long result;
		InvokeHelper(0x60030023, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetState(long lState)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030024, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lState);
	}
	CString toString_Len17()
	{
		CString result;
		InvokeHelper(0x60030025, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString ToString(long lEquptType)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030026, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lEquptType);
		return result;
	}
	BOOL GetHolidayPwr(short vHolidayNum)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2 ;
		InvokeHelper(0x60030027, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, vHolidayNum);
		return result;
	}
	void SetHolidayPwr(short vHolidayNum, BOOL bUse)
	{
		static BYTE parms[] = VTS_I2 VTS_BOOL ;
		InvokeHelper(0x60030028, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vHolidayNum, bUse);
	}
	CString GetHolidayPwrStr()
	{
		CString result;
		InvokeHelper(0x60030029, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetHolidayPwrStr(BSTR * sPwrStr)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003002a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sPwrStr);
	}
	CString toString_FC8800()
	{
		CString result;
		InvokeHelper(0x6003002b, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetCardHex_FC8800(BSTR * sHex)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003002e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sHex);
	}
	CString toString_FC8832()
	{
		CString result;
		InvokeHelper(0x60030030, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetCardHex_FC8832(BSTR * sHex)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030031, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sHex);
	}

	// _DriveCard properties
public:

};
