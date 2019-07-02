// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
// C_DriveMain wrapper class

class C_DriveMain : public COleDispatchDriver
{
public:
	C_DriveMain(){} // Calls COleDispatchDriver default constructor
	C_DriveMain(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	C_DriveMain(const C_DriveMain& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// __DriveMain methods
public:
	void WatchEvents(LPDISPATCH * objConnInfo, short iWatchType, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iWatchType, sValue);
	}
	void FrameAchieve(LPDISPATCH * objConnInfo, short iFunc, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, sValue);
	}
	void PasswordErr(LPDISPATCH * objConnInfo, short iFunc)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc);
	}
	void SendProcess(LPDISPATCH * objConnInfo, short iFunc, long iStep, long iStepCount)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_I4 VTS_I4 ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, iStep, iStepCount);
	}
	void ContainOverflow(LPDISPATCH * objConnInfo, short iFunc)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc);
	}
	void CommandTimeout(LPDISPATCH * objConnInfo, short iFunc, short iStep)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_I2 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, iStep);
	}
	void ReadRecordAchieve(LPDISPATCH * objConnInfo, LPCTSTR lRecordCount, LPDISPATCH * objRecords)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR VTS_PDISPATCH ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, lRecordCount, objRecords);
	}
	void ReadCardAchieve(LPDISPATCH * objConnInfo, LPDISPATCH * objCol)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, objCol);
	}
	void ReadHolidayAchieve(LPDISPATCH * objConnInfo, LPDISPATCH * objCol)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, objCol);
	}
	void ReadTimeGroupAchieve(LPDISPATCH * objConnInfo, LPDISPATCH * objCol)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, objCol);
	}
	void ReadPasswordAchieve(LPDISPATCH * objConnInfo, LPDISPATCH * objCol)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, objCol);
	}
	void OpenPortErr(LPDISPATCH * objConnInfo, short iFunc)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc);
	}
	void SendEvent(LPDISPATCH * objConnInfo, long lDataLen, LPCTSTR sHex)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_BSTR ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, lDataLen, sHex);
	}
	void DataArrivalEvent(LPDISPATCH * objConnInfo, long lDataLen, LPCTSTR sHex)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_BSTR ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, lDataLen, sHex);
	}

	// __DriveMain properties
public:

};
