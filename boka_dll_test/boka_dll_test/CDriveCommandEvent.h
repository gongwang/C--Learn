// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDriveCommandEvent wrapper class

class CDriveCommandEvent : public COleDispatchDriver
{
public:
	CDriveCommandEvent(){} // Calls COleDispatchDriver default constructor
	CDriveCommandEvent(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDriveCommandEvent(const CDriveCommandEvent& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DriveCommandEvent methods
public:
	LPDISPATCH get_EventConnInfo()
	{
		LPDISPATCH result;
		InvokeHelper(0x40030000, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_EventConnInfo(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x40030000, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH get_EventCols()
	{
		LPDISPATCH result;
		InvokeHelper(0x40030001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void putref_EventCols(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(0x40030001, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EventType()
	{
		long result;
		InvokeHelper(0x40030002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EventType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40030002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EventiFunc()
	{
		long result;
		InvokeHelper(0x40030003, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EventiFunc(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40030003, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EventiWatchType()
	{
		long result;
		InvokeHelper(0x40030004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EventiWatchType(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40030004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_EventsValue()
	{
		CString result;
		InvokeHelper(0x40030005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_EventsValue(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x40030005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EventiStep()
	{
		long result;
		InvokeHelper(0x40030006, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EventiStep(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40030006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EventiStepCount()
	{
		long result;
		InvokeHelper(0x40030007, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EventiStepCount(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40030007, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EventlRecordCount()
	{
		long result;
		InvokeHelper(0x40030008, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EventlRecordCount(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40030008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_EventlDataLen()
	{
		long result;
		InvokeHelper(0x40030009, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_EventlDataLen(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40030009, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_EventsHex()
	{
		CString result;
		InvokeHelper(0x4003000a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_EventsHex(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x4003000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	LPDISPATCH GetRecords()
	{
		LPDISPATCH result;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetCards()
	{
		LPDISPATCH result;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetHolidays()
	{
		LPDISPATCH result;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetTimeGroups()
	{
		LPDISPATCH result;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}

	// _DriveCommandEvent properties
public:

};
