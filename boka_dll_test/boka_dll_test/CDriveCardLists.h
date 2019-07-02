// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDriveCardLists wrapper class

class CDriveCardLists : public COleDispatchDriver
{
public:
	CDriveCardLists(){} // Calls COleDispatchDriver default constructor
	CDriveCardLists(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDriveCardLists(const CDriveCardLists& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DriveCardLists methods
public:
	void SetEquptCardType(long lType)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lType);
	}
	long GetEquptCardType()
	{
		long result;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH Add(BSTR * sKey)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PBSTR ;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, sKey);
		return result;
	}
	VARIANT SetCardCount(long lCount)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030004, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, lCount);
		return result;
	}
	LPDISPATCH get_Item(long vntIndexKey)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, vntIndexKey);
		return result;
	}
	void putref_Item(long vntIndexKey, LPDISPATCH * newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x0, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, vntIndexKey, newValue);
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void Remove(long vntIndexKey, BOOL * bResList)
	{
		static BYTE parms[] = VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vntIndexKey, bResList);
	}
	void RefreshList()
	{
		InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Unload()
	{
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

	// _DriveCardLists properties
public:

};
