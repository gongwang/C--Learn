// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDriveTimeGroupLists wrapper class

class CDriveTimeGroupLists : public COleDispatchDriver
{
public:
	CDriveTimeGroupLists(){} // Calls COleDispatchDriver default constructor
	CDriveTimeGroupLists(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDriveTimeGroupLists(const CDriveTimeGroupLists& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DriveTimeGroupLists methods
public:
	LPDISPATCH Add(BSTR * sKey)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, sKey);
		return result;
	}
	LPDISPATCH get_Item(VARIANT * vntIndexKey)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, vntIndexKey);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Remove(VARIANT * vntIndexKey)
	{
		static BYTE parms[] = VTS_PVARIANT ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vntIndexKey);
	}
	LPUNKNOWN get_NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}

	// _DriveTimeGroupLists properties
public:

};
