// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDriveValueList wrapper class

class CDriveValueList : public COleDispatchDriver
{
public:
	CDriveValueList(){} // Calls COleDispatchDriver default constructor
	CDriveValueList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDriveValueList(const CDriveValueList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DriveValueList methods
public:
	long GetCount()
	{
		long result;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetItemKey(long lItem)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lItem);
		return result;
	}
	void AddValue(LPCTSTR sKey, long lValue, LPCTSTR sValue, LPDISPATCH * oPortList, LPDISPATCH * oTimeGroup)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BSTR VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms, sKey, lValue, sValue, oPortList, oTimeGroup);
	}
	CString GetValue_String(LPCTSTR sKey)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, sKey);
		return result;
	}
	long GetValue_Num(LPCTSTR sKey)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030007, DISPATCH_METHOD, VT_I4, (void*)&result, parms, sKey);
		return result;
	}
	LPDISPATCH GetValue_PortList(LPCTSTR sKey)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, sKey);
		return result;
	}
	LPDISPATCH GetValue_TimeGroup(LPCTSTR sKey)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, sKey);
		return result;
	}

	// _DriveValueList properties
public:

};
