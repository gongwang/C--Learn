// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

//#import "C:\\Program Files (x86)\\智能门禁控制器调试程序\\FCDrive8800.Dll" no_namespace
#import "../Release/FCDrive8800.Dll" no_namespace
// CDriveMain wrapper class

class CDriveMain : public COleDispatchDriver
{
public:
	CDriveMain(){} // Calls COleDispatchDriver default constructor
	CDriveMain(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CDriveMain(const CDriveMain& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _DriveMain methods
public:
	CString GetVer()
	{
		CString result;
		InvokeHelper(0x60030022, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	long get_CheckMaxChunnel()
	{
		long result;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_CheckMaxChunnel(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x68030000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL SetShowTxRxLog(BOOL bShow)
	{
		BOOL result;
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030167, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, bShow);
		return result;
	}
	long GetAlignmentCount()
	{
		long result;
		InvokeHelper(0x60030001, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetChunelAlignmentCount(LPDISPATCH * objConnInfo)
	{
		long result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x600300cf, DISPATCH_METHOD, VT_I4, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL CheckCommandIsAdd(LPDISPATCH * objConnInfo, long lFunction)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x60030163, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lFunction);
		return result;
	}
	void ChunnelForciblyState(LPDISPATCH * objConnInfo, BOOL bBool)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x600300f4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, bBool);
	}
	void SetShowEvent(BOOL bShowEvent)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bShowEvent);
	}
	LPDISPATCH GetEvent()
	{
		LPDISPATCH result;
		InvokeHelper(0x60030003, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void SetDrivePause()
	{
		InvokeHelper(0x600300cd, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetDriveRun()
	{
		InvokeHelper(0x600300ce, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL DriveIsRun()
	{
		BOOL result;
		InvokeHelper(0x60030166, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetEventSending()
	{
		LPDISPATCH result;
		InvokeHelper(0x60030008, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetTmpConnInfo()
	{
		LPDISPATCH result;
		InvokeHelper(0x60030009, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetTmpRecords()
	{
		LPDISPATCH result;
		InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetTmpCards()
	{
		LPDISPATCH result;
		InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetTmpHolidays()
	{
		LPDISPATCH result;
		InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetTmpTimeGroups()
	{
		LPDISPATCH result;
		InvokeHelper(0x6003000d, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH GetTmpPasswords()
	{
		LPDISPATCH result;
		InvokeHelper(0x6003000e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void RaiseWatchEvents(LPDISPATCH * objConnInfo, short iWatchType, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR ;
		InvokeHelper(0x60030010, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iWatchType, sValue);
	}
	void RaiseFrameAchieve(LPDISPATCH * objConnInfo, short iFunc, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR ;
		InvokeHelper(0x60030011, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, sValue);
	}
	void RaiseFrameAchieve_Obj(LPDISPATCH * objConnInfo, short iFunc, LPCTSTR sValue, LPDISPATCH * oCols)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR VTS_PDISPATCH ;
		InvokeHelper(0x60030012, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, sValue, oCols);
	}
	void RaiseSendProcess(LPDISPATCH * objConnInfo, long iFunc, long iStep, long iStepCount, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_I4 VTS_I4 VTS_BSTR ;
		InvokeHelper(0x60030013, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, iStep, iStepCount, sValue);
	}
	void RaiseContainOverflow(LPDISPATCH * objConnInfo, short iFunc, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR ;
		InvokeHelper(0x60030014, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, sValue);
	}
	void RaiseReadRecordAchieve(LPDISPATCH * objConnInfo, long lRecordCount, LPDISPATCH * objRecords)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x60030015, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, lRecordCount, objRecords);
	}
	void RaiseReadCardAchieve(LPDISPATCH * objConnInfo, LPDISPATCH * objCards)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030016, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, objCards);
	}
	void RaiseReadHolidayAchieve(LPDISPATCH * objConnInfo, LPDISPATCH * objCol)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030017, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, objCol);
	}
	void RaiseReadTimeGroupAchieve(LPDISPATCH * objConnInfo, LPDISPATCH * objCol)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030018, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, objCol);
	}
	void RaisePasswordErr(LPDISPATCH * objConnInfo, short iFunc, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR ;
		InvokeHelper(0x60030019, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, sValue);
	}
	void RaiseReadPasswordAchieve(LPDISPATCH * objConnInfo, LPDISPATCH * objCol)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x6003001a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, objCol);
	}
	void RaiseOpenPortErr(LPDISPATCH * objConnInfo, short iFunc, LPDISPATCH * objCards, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_PDISPATCH VTS_BSTR ;
		InvokeHelper(0x6003001b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, objCards, sValue);
	}
	void RaiseCommandTimeout(LPDISPATCH * objConnInfo, short iFunc, short iStep, LPDISPATCH * objCards, LPCTSTR sValue)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_I2 VTS_PDISPATCH VTS_BSTR ;
		InvokeHelper(0x6003001c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, iFunc, iStep, objCards, sValue);
	}
	void RaiseSendEvent(LPDISPATCH * objConnInfo, long lDataLen, LPCTSTR sHex)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_BSTR ;
		InvokeHelper(0x6003001d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, lDataLen, sHex);
	}
	void RaiseDataArrivalEvent(LPDISPATCH * objConnInfo, long lDataLen, LPCTSTR sHex)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_BSTR ;
		InvokeHelper(0x6003001e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, lDataLen, sHex);
	}
	void Unload()
	{
		InvokeHelper(0x60030021, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long GetWinSockState(long lIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60030023, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lIndex);
		return result;
	}
	void StopConnAll()
	{
		InvokeHelper(0x60030024, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void StopConn(LPDISPATCH * objConnInfo)
	{
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030025, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo);
	}
	void StopConnOnSN(LPDISPATCH * objConnInfo, LPCTSTR sSN)
	{
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR ;
		InvokeHelper(0x600300fd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, objConnInfo, sSN);
	}
	BOOL WriteSN(LPDISPATCH * objConnInfo, LPCTSTR sSN)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR ;
		InvokeHelper(0x6003002a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, sSN);
		return result;
	}
	BOOL WriteSN_Ps(LPDISPATCH * objConnInfo, LPCTSTR sPassword1, LPCTSTR sPassword2, LPCTSTR sSN)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x6003010a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, sPassword1, sPassword2, sSN);
		return result;
	}
	BOOL WriteSN_Bro(LPDISPATCH * objConnInfo, LPCTSTR sPassword1, LPCTSTR sPassword2, LPCTSTR sSN)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x60030145, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, sPassword1, sPassword2, sSN);
		return result;
	}
	BOOL ReadSN(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003002b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteConnPassword(LPDISPATCH * objConnInfo, LPCTSTR sPassword)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR ;
		InvokeHelper(0x6003002c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, sPassword);
		return result;
	}
	BOOL ReadConnPassword(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003002d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL DeleteConnPassword(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003002e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL GetTCPPar(LPDISPATCH * objConnInfo, BOOL mUDPBroadcast)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x6003002f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, mUDPBroadcast);
		return result;
	}
	BOOL WriteTCPPar(LPDISPATCH * objConnInfo, LPDISPATCH * oTCPPar, BOOL mUDPBroadcast)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x60030030, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oTCPPar, mUDPBroadcast);
		return result;
	}
	BOOL WriteTCPPar_MAC(LPDISPATCH * objConnInfo, LPDISPATCH * oTCPPar, BOOL mUDPBroadcast)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x60030160, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oTCPPar, mUDPBroadcast);
		return result;
	}
	BOOL ReadEquptDeadline(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030031, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteEquptDeadline(LPDISPATCH * objConnInfo, long lDeadline)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x60030032, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lDeadline);
		return result;
	}
	BOOL ReadEquptVer(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030033, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL ReadEquptStateInfo(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030034, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL ReadSysPar(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030035, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteRecordMode(LPDISPATCH * objConnInfo, short lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030036, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lMode);
		return result;
	}
	BOOL ReadRecordMode(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030037, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteKeyboard(LPDISPATCH * objConnInfo, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030038, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oPort);
		return result;
	}
	BOOL ReadKeyboard(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030039, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL IniKeyboardPassword(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003003a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL ReadKeyboardPassword(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003003b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteLockInteraction(LPDISPATCH * objConnInfo, BOOL bDoor1, BOOL bDoor2, BOOL bDoor3, BOOL bDoor4)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL VTS_BOOL VTS_BOOL VTS_BOOL ;
		InvokeHelper(0x6003003c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bDoor1, bDoor2, bDoor3, bDoor4);
		return result;
	}
	BOOL ReadLockInteraction(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003003d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteFireAlarm(LPDISPATCH * objConnInfo, short lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x6003003e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lMode);
		return result;
	}
	BOOL ReadFireAlarm(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003003f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteOpenAlarm(LPDISPATCH * objConnInfo, short lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030040, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lMode);
		return result;
	}
	BOOL ReadOpenAlarm(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030041, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SetReaderIntervalTime(LPDISPATCH * objConnInfo, long lInterval)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x60030042, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lInterval);
		return result;
	}
	BOOL GetReaderIntervalTime(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030043, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SetReaderIntervalTime_Elevator(LPDISPATCH * objConnInfo, BOOL bUse, long lInterval, short lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL VTS_I4 VTS_I2 ;
		InvokeHelper(0x60030044, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bUse, lInterval, lMode);
		return result;
	}
	BOOL SetSpeakOpen(LPDISPATCH * objConnInfo, LPCTSTR sOpen)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR ;
		InvokeHelper(0x60030045, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, sOpen);
		return result;
	}
	BOOL ReadSpeakOpen(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030046, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SetReaderCheckMode(LPDISPATCH * objConnInfo, BOOL bOpen)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x60030047, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bOpen);
		return result;
	}
	BOOL SetReaderCheckMode_Mode(LPDISPATCH * objConnInfo, unsigned char lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_UI1 ;
		InvokeHelper(0x600300f3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lMode);
		return result;
	}
	BOOL ReadReaderCheckMode(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030048, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SetEquptBuzzerMode(LPDISPATCH * objConnInfo, BOOL bOpen)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x60030049, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bOpen);
		return result;
	}
	BOOL ReadEquptBuzzerMode(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003004a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteAssistantAlertPar(LPDISPATCH * objConnInfo, unsigned char lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_UI1 ;
		InvokeHelper(0x6003004b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lMode);
		return result;
	}
	BOOL ReadAssistantAlertPar(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003004c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteCardMaxPar(LPDISPATCH * objConnInfo, long lCardMax, LPDISPATCH * oDoorCardMax, LPDISPATCH * oDoorInCardCount)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x6003004d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lCardMax, oDoorCardMax, oDoorInCardCount);
		return result;
	}
	BOOL ReadCardMaxPar(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003004e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteTheftAlarmPar(LPDISPATCH * objConnInfo, BOOL bOpen, unsigned char lInTime, unsigned char lOutTime, long lBeginPassword, long lClosePassword, long lAlarmTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL VTS_UI1 VTS_UI1 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x6003004f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bOpen, lInTime, lOutTime, lBeginPassword, lClosePassword, lAlarmTime);
		return result;
	}
	BOOL ReadTheftAlarmPar(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030050, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteCheckInOutPar(LPDISPATCH * objConnInfo, unsigned char lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_UI1 ;
		InvokeHelper(0x60030051, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lMode);
		return result;
	}
	BOOL ReadCheckInOutPar(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030052, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteCardPeriodSpeak(LPDISPATCH * objConnInfo, BOOL lOpen)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x60030053, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lOpen);
		return result;
	}
	BOOL ReadCardPeriodSpeak(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030054, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteReadCardSpeak(LPDISPATCH * objConnInfo, BOOL bOpen, unsigned char lMsgIndex, DATE dBeginDate, DATE dEndDate)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL VTS_UI1 VTS_DATE VTS_DATE ;
		InvokeHelper(0x60030055, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bOpen, lMsgIndex, dBeginDate, dEndDate);
		return result;
	}
	BOOL ReadReadCardSpeak(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030056, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL BeginWatch(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030057, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL BeginWatch_Bro(LPDISPATCH * objConnInfo, BOOL bUDP)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x600300db, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bUDP);
		return result;
	}
	BOOL CloseWatch(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030058, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL CloseWatch_Bro(LPDISPATCH * objConnInfo, BOOL bUDP)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x600300dc, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bUDP);
		return result;
	}
	BOOL ReadWatchState(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030059, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SendFireAlarm(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003005a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL CloseFireAlarm(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003005b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL ReadFireAlarmState(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003005c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SendAssistantAlarm(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003005d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL CloseAssistantAlarm(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003005e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL ReadAssistantAlarm(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003005f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL CloseAlarm(LPDISPATCH * objConnInfo, short iPort, long lAlarmPar)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_I4 ;
		InvokeHelper(0x60030060, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, lAlarmPar);
		return result;
	}
	BOOL CloseAlarm_Elevator(LPDISPATCH * objConnInfo, long lAlarmPar)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x60030061, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lAlarmPar);
		return result;
	}
	BOOL GetPortState(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030062, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL GetTheftAlarmState(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030063, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL IniEqupt(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030064, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SearchEquptOnNetNum(LPDISPATCH * objConnInfo, long lNetNum)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x60030065, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lNetNum);
		return result;
	}
	BOOL SetEquptNetNum(LPDISPATCH * objConnInfo, long lNetNum, BOOL mUDPBroadcast)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_BOOL ;
		InvokeHelper(0x60030066, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lNetNum, mUDPBroadcast);
		return result;
	}
	BOOL WriteMemory(LPDISPATCH * objConnInfo, LPCTSTR sHex)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR ;
		InvokeHelper(0x600300dd, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, sHex);
		return result;
	}
	BOOL ReadMemory(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x600300de, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SetManageCard(LPDISPATCH * objConnInfo, BOOL bUse)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x60030152, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bUse);
		return result;
	}
	BOOL GetManageCard(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030153, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL ReadTime(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030067, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteTime(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030068, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteTimeDefine(LPDISPATCH * objConnInfo, DATE oDate)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_DATE ;
		InvokeHelper(0x600300cb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oDate);
		return result;
	}
	BOOL WriteTime_Bro(LPDISPATCH * objConnInfo, BOOL bUDP)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x600300df, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bUDP);
		return result;
	}
	BOOL ReadTimePar(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030069, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteTimePar(LPDISPATCH * objConnInfo, short lParMode, short lSecond)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_I2 ;
		InvokeHelper(0x6003006a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lParMode, lSecond);
		return result;
	}
	BOOL GetReaderPar(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003006b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteReaderPar(LPDISPATCH * objConnInfo, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x6003006c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oPort);
		return result;
	}
	BOOL GetLockPar(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003006d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteLockPar(LPDISPATCH * objConnInfo, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x6003006e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oPort);
		return result;
	}
	BOOL OpenDoor(LPDISPATCH * objConnInfo, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x6003006f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oPort);
		return result;
	}
	BOOL OpenDoor_Check(LPDISPATCH * objConnInfo, unsigned char lNum, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_UI1 VTS_PDISPATCH ;
		InvokeHelper(0x60030070, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lNum, oPort);
		return result;
	}
	BOOL CloseDoor(LPDISPATCH * objConnInfo, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030071, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oPort);
		return result;
	}
	BOOL OpenDoor_always(LPDISPATCH * objConnInfo, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030072, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oPort);
		return result;
	}
	BOOL LockPort(LPDISPATCH * objConnInfo, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030073, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oPort);
		return result;
	}
	BOOL UnLockPort(LPDISPATCH * objConnInfo, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030074, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oPort);
		return result;
	}
	BOOL GetPortCheckMode(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030075, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL WritePortCheckMode(LPDISPATCH * objConnInfo, short iPort, LPDISPATCH * oTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_PDISPATCH ;
		InvokeHelper(0x60030076, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, oTime);
		return result;
	}
	BOOL GetPortCheckMode_Elevator(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030077, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WritePortCheckMode_Elevator(LPDISPATCH * objConnInfo, LPDISPATCH * oTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030078, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oTime);
		return result;
	}
	BOOL GetPortWorkMode(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030079, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL WritePortWorkMode(LPDISPATCH * objConnInfo, short iPort, BOOL bUse, short lOpenMode, short lLongOpenMode, LPDISPATCH * oTimeG)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL VTS_I2 VTS_I2 VTS_PDISPATCH ;
		InvokeHelper(0x6003007a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse, lOpenMode, lLongOpenMode, oTimeG);
		return result;
	}
	BOOL ReadLockReadTime(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x6003007b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL WriteLockReadTime(LPDISPATCH * objConnInfo, short iPort, BOOL bOpen, LPDISPATCH * oTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL VTS_PDISPATCH ;
		InvokeHelper(0x6003007c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bOpen, oTime);
		return result;
	}
	BOOL ReadLockOpenTime(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x6003007d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL WriteLockOpenTime(LPDISPATCH * objConnInfo, short iPort, long lLockOpenTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_I4 ;
		InvokeHelper(0x6003007e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, lLockOpenTime);
		return result;
	}
	BOOL ReadLockOpenTime_Elevator(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003007f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteLockOpenTime_Elevator(LPDISPATCH * objConnInfo, LPDISPATCH * oPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x60030080, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, oPort);
		return result;
	}
	BOOL GetReaderInterval(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030081, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL WriteReaderInterval(LPDISPATCH * objConnInfo, short iPort, BOOL bUse, short lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL VTS_I2 ;
		InvokeHelper(0x60030082, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse, lMode);
		return result;
	}
	BOOL SetReadErrCardAlarm(LPDISPATCH * objConnInfo, short iPort, BOOL bUse)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL ;
		InvokeHelper(0x60030083, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse);
		return result;
	}
	BOOL GetReadErrCardAlarm(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030084, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetReadErrCardAlarm_Elevator(LPDISPATCH * objConnInfo, BOOL bUse)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x60030085, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bUse);
		return result;
	}
	BOOL GetReadErrCardAlarm_Elevator(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030086, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SetAlarmPassword(LPDISPATCH * objConnInfo, short iPort, BOOL bUse, LPCTSTR sPasswrod, short lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL VTS_BSTR VTS_I2 ;
		InvokeHelper(0x60030087, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse, sPasswrod, lMode);
		return result;
	}
	BOOL GetAlarmPassword(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030088, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetAlarmPassword_Elevator(LPDISPATCH * objConnInfo, BOOL bUse, LPCTSTR sPasswrod, short lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL VTS_BSTR VTS_I2 ;
		InvokeHelper(0x60030089, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bUse, sPasswrod, lMode);
		return result;
	}
	BOOL GetAlarmPassword_Elevator(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003008a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL GetCheckCardInOutState(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x6003008b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetCheckCardInOutState(LPDISPATCH * objConnInfo, short iPort, BOOL bUse)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL ;
		InvokeHelper(0x6003008c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse);
		return result;
	}
	BOOL GetLongOpenAlarm(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x6003008d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetLongOpenAlarm(LPDISPATCH * objConnInfo, short iPort, BOOL bOpen, long lLongOpenTime, BOOL bAlarm)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL VTS_I4 VTS_BOOL ;
		InvokeHelper(0x6003008e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bOpen, lLongOpenTime, bAlarm);
		return result;
	}
	BOOL GetButtonPar(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x6003008f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetButtonPar(LPDISPATCH * objConnInfo, short iPort, BOOL bUse, BOOL bUseLongOpen, LPDISPATCH * oTimeG)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL VTS_BOOL VTS_PDISPATCH ;
		InvokeHelper(0x60030090, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse, bUseLongOpen, oTimeG);
		return result;
	}
	BOOL GetDoorStateAlarm(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030091, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetDoorStateAlarm(LPDISPATCH * objConnInfo, short iPort, BOOL bUse, LPDISPATCH * oTimeG)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL VTS_PDISPATCH ;
		InvokeHelper(0x60030092, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse, oTimeG);
		return result;
	}
	BOOL GetProhibitCheck(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030093, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetProhibitCheck(LPDISPATCH * objConnInfo, short iPort, BOOL bOpen, BOOL bSaveCard, long lTimeGroupIndex)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL VTS_BOOL VTS_I4 ;
		InvokeHelper(0x60030094, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bOpen, bSaveCard, lTimeGroupIndex);
		return result;
	}
	BOOL GetMuchCardPar(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030095, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetMuchCardPar(LPDISPATCH * objConnInfo, short iPort, unsigned char lInCardMax, unsigned char lOutCardMax)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_UI1 VTS_UI1 ;
		InvokeHelper(0x60030096, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, lInCardMax, lOutCardMax);
		return result;
	}
	BOOL GetMuchCardGroup(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x60030097, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetMuchCardGroup(LPDISPATCH * objConnInfo, short iPort, unsigned char lGroupIndex, long lGroupMode, LPDISPATCH * oCards)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_UI1 VTS_I4 VTS_PDISPATCH ;
		InvokeHelper(0x60030098, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, lGroupIndex, lGroupMode, oCards);
		return result;
	}
	BOOL SetPortSpeak(LPDISPATCH * objConnInfo, short iPort, BOOL bUse)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL ;
		InvokeHelper(0x600300e0, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse);
		return result;
	}
	BOOL GetPortSpeak(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x600300e1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetBothWayCheck(LPDISPATCH * objConnInfo, short iPort, BOOL bUse)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL ;
		InvokeHelper(0x600300e2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse);
		return result;
	}
	BOOL GetBothWayCheck(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x600300e3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetKeyboardManage(LPDISPATCH * objConnInfo, short iPort, BOOL bUse)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BOOL ;
		InvokeHelper(0x600300e4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, bUse);
		return result;
	}
	BOOL GetKeyboardManage(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x600300e5, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetKeyboardManagePassword(LPDISPATCH * objConnInfo, short iPort, LPCTSTR sPassword)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR ;
		InvokeHelper(0x600300e6, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, sPassword);
		return result;
	}
	BOOL SetKeyboardManagePassword_8832(LPDISPATCH * objConnInfo, BOOL bUse, LPCTSTR sPassword)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL VTS_BSTR ;
		InvokeHelper(0x60030161, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bUse, sPassword);
		return result;
	}
	BOOL GetKeyboardManagePassword(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x600300e7, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL GetKeyboardManagePassword_8832(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030162, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SetFirstCardCheck(LPDISPATCH * objConnInfo, short iPort, unsigned char lCardMode, unsigned char lPasswordMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_UI1 VTS_UI1 ;
		InvokeHelper(0x60030119, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, lCardMode, lPasswordMode);
		return result;
	}
	BOOL GetFirstCardCheck(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x6003011a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetMuchCardCheck(LPDISPATCH * objConnInfo, short iPort, unsigned char lCardMode, unsigned char lCheckInOut)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_UI1 VTS_UI1 ;
		InvokeHelper(0x6003011b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, lCardMode, lCheckInOut);
		return result;
	}
	BOOL GetMuchCardCheck(LPDISPATCH * objConnInfo, short iPort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 ;
		InvokeHelper(0x6003011c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort);
		return result;
	}
	BOOL SetAlarmOutputTime(LPDISPATCH * objConnInfo, unsigned char lTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_UI1 ;
		InvokeHelper(0x60030129, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lTime);
		return result;
	}
	BOOL GetAlarmOutputTime(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003012a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL ClearAllOpenData(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003012b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL GetHolidayInfo(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030099, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL HolidayInfoHexToNum(LPCTSTR sHex, double * lMaxCount, double * lReocrdCount)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x6003009a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sHex, lMaxCount, lReocrdCount);
		return result;
	}
	BOOL ClearHoliday(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003009b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL GetHoliday(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003009c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteHoliday(LPDISPATCH * objConnInfo, LPDISPATCH * objHols)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x6003009d, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, objHols);
		return result;
	}
	BOOL DeleteHoliday(LPDISPATCH * objConnInfo, LPDISPATCH * objHols)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x6003009e, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, objHols);
		return result;
	}
	BOOL GetPasswordInfo(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x6003009f, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL PasswordInfoHexToNum(LPCTSTR sHex, double * lMaxCount, double * lReocrdCount)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x600300a0, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sHex, lMaxCount, lReocrdCount);
		return result;
	}
	BOOL ClearPassword(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x600300a1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL GetPassword(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x600300a2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WritePassword(LPDISPATCH * objConnInfo, LPDISPATCH * objPw)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x600300a3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, objPw);
		return result;
	}
	BOOL DeletePassword(LPDISPATCH * objConnInfo, LPDISPATCH * objPw)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x600300a4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, objPw);
		return result;
	}
	BOOL TestPassword(LPDISPATCH * objConnInfo, short iPort, LPCTSTR sPassword)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR ;
		InvokeHelper(0x600300a5, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, sPassword);
		return result;
	}
	BOOL TestPassword_Ps(LPDISPATCH * objConnInfo, LPCTSTR sPassword1, LPCTSTR sPassword2, short iPort, LPCTSTR sPassword)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR VTS_BSTR VTS_I2 VTS_BSTR ;
		InvokeHelper(0x6003010b, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, sPassword1, sPassword2, iPort, sPassword);
		return result;
	}
	BOOL TestPassword_Elevator(LPDISPATCH * objConnInfo, short iPort, LPCTSTR sPassword, unsigned char lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR VTS_UI1 ;
		InvokeHelper(0x600300a6, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, sPassword, lMode);
		return result;
	}
	BOOL ClearTimeGroup(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x600300a7, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL GetTimeGroup(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x600300a8, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL WriteTimeGroup(LPDISPATCH * objConnInfo, LPDISPATCH * objTs)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x600300a9, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, objTs);
		return result;
	}
	BOOL GetCardCount(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x600300aa, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL CardCountInfoHexToNum(LPCTSTR sHex, double * lMaxCount, double * lCardCount, double * lSequenceCardMaxCount, double * lSequenceCardCount)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x600300ab, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sHex, lMaxCount, lCardCount, lSequenceCardMaxCount, lSequenceCardCount);
		return result;
	}
	BOOL ClearCard(LPDISPATCH * objConnInfo, long lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x600300ac, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lMode);
		return result;
	}
	BOOL GetCards(LPDISPATCH * objConnInfo, long lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x600300ad, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lMode);
		return result;
	}
	BOOL GetCardInfo(LPDISPATCH * objConnInfo, double dCard)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_R8 ;
		InvokeHelper(0x600300ae, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, dCard);
		return result;
	}
	BOOL WriteUserCard(LPDISPATCH * objConnInfo, LPDISPATCH * objCards)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x600300af, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, objCards);
		return result;
	}
	BOOL WriteUserCard_Mode(LPDISPATCH * objConnInfo, LPDISPATCH * objCards, long lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x60030138, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, objCards, lMode);
		return result;
	}
	BOOL DeleteUserCard(LPDISPATCH * objConnInfo, LPDISPATCH * objCards)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH ;
		InvokeHelper(0x600300b0, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, objCards);
		return result;
	}
	BOOL TestCard(LPDISPATCH * objConnInfo, short iPort, double dCard, LPCTSTR sPassword, long lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_R8 VTS_BSTR VTS_I4 ;
		InvokeHelper(0x600300b1, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, iPort, dCard, sPassword, lMode);
		return result;
	}
	BOOL TestCard_Ps(LPDISPATCH * objConnInfo, LPCTSTR sPassword1, LPCTSTR sPassword2, short iPort, double dCard, LPCTSTR sPassword, long lMode)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BSTR VTS_BSTR VTS_I2 VTS_R8 VTS_BSTR VTS_I4 ;
		InvokeHelper(0x6003010c, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, sPassword1, sPassword2, iPort, dCard, sPassword, lMode);
		return result;
	}
	BOOL WriteSequenceCard(LPDISPATCH * objConnInfo, LPDISPATCH * objCards, BOOL bSort)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x600300b2, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, objCards, bSort);
		return result;
	}
	BOOL GetRecordInfo(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x600300b3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL RecordInfoHexToNum(long lRecordCode, LPCTSTR sHex, long * lRecordMaxCount, double * lNewRecordCount, double * lRecordEndAddr, double * lUploadAddr, long * lReadAll)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_PI4 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PI4 ;
		InvokeHelper(0x600300b4, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, lRecordCode, sHex, lRecordMaxCount, lNewRecordCount, lRecordEndAddr, lUploadAddr, lReadAll);
		return result;
	}
	BOOL ClearAllRecord(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x600300b5, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL ClearRecordOnly(LPDISPATCH * objConnInfo, long lRecordType)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x600300b6, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lRecordType);
		return result;
	}
	BOOL UpdateReocrdFlag(LPDISPATCH * objConnInfo, long lRecordType, long lRecordFlay, BOOL bLoop)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x600300b7, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lRecordType, lRecordFlay, bLoop);
		return result;
	}
	BOOL UpdateReocrdEndFlag(LPDISPATCH * objConnInfo, long lRecordType, long lRecordEndFlay)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_I4 ;
		InvokeHelper(0x600300b8, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lRecordType, lRecordEndFlay);
		return result;
	}
	BOOL RepairRecord(LPDISPATCH * objConnInfo, long lRecordType)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 ;
		InvokeHelper(0x600300b9, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lRecordType);
		return result;
	}
	BOOL GetRecordOnFlay(LPDISPATCH * objConnInfo, long lRecordType, long lBeginNum, long lReadCount)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x600300ba, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lRecordType, lBeginNum, lReadCount);
		return result;
	}
	BOOL ReadRecord(LPDISPATCH * objConnInfo, long lRecordType, long lReadCount)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I4 VTS_I4 ;
		InvokeHelper(0x600300bb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lRecordType, lReadCount);
		return result;
	}
	BOOL SetICPar(LPDISPATCH * objConnInfo, short lSector, LPCTSTR sSectorPassword, unsigned char lCheckType, long lIDCode1, long lIDCode2, long lIDCode3, long lIDCode4)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR VTS_UI1 VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x60030174, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, lSector, sSectorPassword, lCheckType, lIDCode1, lIDCode2, lIDCode3, lIDCode4);
		return result;
	}
	BOOL ReadICPar(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030175, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL GetBlacklistAlarm(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030176, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	BOOL SetBlacklistAlarm(LPDISPATCH * objConnInfo, BOOL bOpen)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL ;
		InvokeHelper(0x60030177, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bOpen);
		return result;
	}
	BOOL SetKeyboardManagePassword_IC(LPDISPATCH * objConnInfo, BOOL bUse, LPCTSTR sPassword)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH VTS_BOOL VTS_BSTR ;
		InvokeHelper(0x60030178, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo, bUse, sPassword);
		return result;
	}
	BOOL GetKeyboardManagePassword_IC(LPDISPATCH * objConnInfo)
	{
		BOOL result;
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(0x60030179, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, objConnInfo);
		return result;
	}
	CString AscToStr(LPCTSTR strAsc)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x600300bc, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strAsc);
		return result;
	}
	CString StrToAsc(LPCTSTR strChr)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x600300bd, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, strChr);
		return result;
	}
	BOOL CallSubByName(LPCTSTR sSubName, long lEquptType, LPDISPATCH * objConnInfo, SAFEARRAY * * oParList)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_PDISPATCH VTS_UNKNOWN ;
		InvokeHelper(0x600300be, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, sSubName, lEquptType, objConnInfo, oParList);
		return result;
	}
	LPDISPATCH AchieveValuetoList(LPDISPATCH * objConnInfo, short iFunc, LPCTSTR sValue)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_PDISPATCH VTS_I2 VTS_BSTR ;
		InvokeHelper(0x600300bf, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, objConnInfo, iFunc, sValue);
		return result;
	}

	// _DriveMain properties
public:

};
