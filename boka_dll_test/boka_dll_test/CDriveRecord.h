// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDriveRecord wrapper class

class CDriveRecord : public COleDispatchDriver
{
public:
	CDriveRecord(){} // Calls COleDispatchDriver default constructor
	CDriveRecord(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDriveRecord(const CDriveRecord& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DriveRecord methods
public:
	void SetNum(long lNum)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lNum);
	}
	long GetNum()
	{
		long result;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetCode(long lCode)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lCode);
	}
	long GetCode()
	{
		long result;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetType()
	{
		long result;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	DATE GetDateTime()
	{
		DATE result;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_DATE, (void*)&result, NULL);
		return result;
	}
	double GetCard()
	{
		double result;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	CString GetPassword()
	{
		CString result;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long GetDoorPort()
	{
		long result;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetRecordStr()
	{
		CString result;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void SetRecordStr(BSTR * sNew)
	{
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sNew);
	}
	void SetRecordFC8800Str(long lCode, BSTR * sNew)
	{
		static BYTE parms[] = VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lCode, sNew);
	}
	void SetRecordFC9800ICStr(long lCode, BSTR * sNew)
	{
		static BYTE parms[] = VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x60030016, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lCode, sNew);
	}

	// _DriveRecord properties
public:

};
