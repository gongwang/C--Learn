// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDrivePortList wrapper class

class CDrivePortList : public COleDispatchDriver
{
public:
	CDrivePortList(){} // Calls COleDispatchDriver default constructor
	CDrivePortList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDrivePortList(const CDrivePortList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DrivePortList methods
public:
	void SetEquptType(long lEquptType)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lEquptType);
	}
	long GetEquptType()
	{
		long result;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetPortCount()
	{
		long result;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SetPortValue(long iPort, long lValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms, iPort, lValue);
	}
	long GetPortValue(long iPort)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_I4, (void*)&result, parms, iPort);
		return result;
	}
	CString ToString()
	{
		CString result;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString ToString_PortValue1B()
	{
		CString result;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString ToString_PortValue2B()
	{
		CString result;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString ToString_PortValue4B()
	{
		CString result;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// _DrivePortList properties
public:

};
