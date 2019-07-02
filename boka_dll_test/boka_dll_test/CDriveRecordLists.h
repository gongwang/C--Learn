// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// CDriveRecordLists wrapper class

class CDriveRecordLists : public COleDispatchDriver
{
public:
    CDriveRecordLists(){} // Calls COleDispatchDriver default constructor
    CDriveRecordLists(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CDriveRecordLists(const CDriveRecordLists& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

    // Attributes
public:

    // Operations
public:


    // _DriveRecordLists methods
public:
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
    LPDISPATCH Add(BSTR * sKey)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_PBSTR ;
        InvokeHelper(0x60030004, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, sKey);
        return result;
    }
    VARIANT SetRecordCount(long lCount)
    {
        VARIANT result;
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x60030005, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, lCount);
        return result;
    }
    LPDISPATCH get_Item(long vntIndexKey)
    {
        LPDISPATCH result;
        static BYTE parms[] = VTS_I4 ;
        InvokeHelper(0x68030001, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, vntIndexKey);
        return result;
    }
    void putref_Item(long vntIndexKey, LPDISPATCH * newValue)
    {
        static BYTE parms[] = VTS_I4 VTS_PDISPATCH ;
        InvokeHelper(0x68030001, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, vntIndexKey, newValue);
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
        InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms, vntIndexKey, bResList);
    }
    void RefreshList()
    {
        InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }
    void Unload()
    {
        InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
    }

    // _DriveRecordLists properties
public:

};
