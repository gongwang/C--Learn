/********************************************************************
	created:	2012/09/27 10:17 
	file base:	comconnectpolicy.h
	author:		Milo
	
	purpose:	
*********************************************************************/
#pragma once
#include "gsautoLock.h"
#include "serialport/SerialEx.h"
#include "errorcode.h"
#include "dascommonfun.h"
#include "iconnectbase.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;
#define SERIAL_DATA_NOTY 10000

class CComConnectPolicy;
class CSerialEventHandle;
class CSerialEventHandle : public CSerialEx
{
public:
	CSerialEventHandle() : m_pPolicy(NULL){}
	virtual ~CSerialEventHandle(){m_pPolicy = NULL;}

	void SetPolicy(CComConnectPolicy* pPolicy){m_pPolicy = pPolicy;}
protected:
	// Event handler that is called when 
	virtual void OnEvent (EEvent eEvent, EError eError);

private:
	TDataBuff m_tDataBuff;
	CComConnectPolicy* m_pPolicy;
};

class CComConnectPolicy : public IConnectBase
{
	typedef map<s32,u32> AddrToObjMap;//com连接不需要IP和端口，存串口地址
	typedef AddrToObjMap::iterator ItrObj;
	typedef pair<ItrObj,bool> Mapret;
public:
	CComConnectPolicy();
	virtual ~CComConnectPolicy();


	enum {ConnetMode = EMICB_PROTO_Com};//连接模式
public:
	//初始化协议
	static BOOL32 InitProtocol();
	//释放协议
	static BOOL32 ReleaseProtocol();
	//是否初始化协议
	static BOOL32 IsProtocolInit(){return s_bInitProtocol;};

	virtual u32 SetLocalCfg(const void* ptLocalCfg= NULL, u32 dwLocalCfgSize=0);
	virtual u32 GetLocalCfg(void* ptLocalCfg = NULL);
	virtual u32 BindDeviceInfo(u32 dwObjMark,const TICBAddress* ptDevAddr = NULL);
	virtual u32 UnbindDeviceInfo(u32 dwObjMark, const TICBAddress* ptDevAddr=NULL );
	virtual BOOL32 IsEqualDeviceInfo(const TICBAddress* ptDevAddr);
	virtual u32 StartConnect(void);
	virtual u32 StopConnect();
	virtual u32 SendData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut = 0);
	virtual u32 PostData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut = 0);
	virtual void SetConnectStateCB(pfICBConnectFunc cb,void *pContext=NULL);
	virtual void SetTranslateCB(pfICBTranslateFunc cb,void *pContext=NULL);
	virtual const s8* GetLastErrStr() const
	{
		return m_strLastErr;
	}
	virtual u32	GetLastErrNo() const
	{
		return m_dwLastErrNo;
	}
	void SetLastErr(const u32 dwErrNo, string pErr)
	{
		if (!pErr.empty())
		{
			strcpy_s(m_strLastErr,pErr.c_str());
			m_dwLastErrNo = dwErrNo;
		}
	}

	void SetParam(u16 event=SERIAL_DATA_NOTY, DWORD dwInQueue = 0, DWORD dwOutQueue = 0);
	BOOL32 CheckPort(const u16 wPort);
public:
	s32 OnRecv(const u8* chData, s32 nSize);
	s32 OnState(EMICBDevConnState eState);

protected:
	BOOL32 x_open();
	BOOL32 x_close(void);
	void Clear();
private:
	typedef void (*Timer)(void * args,u32 dwTimeOut);
	void SetTimerCB(Timer timer,void * pContent,u32 dwTimeOut);
	static void OnTimer(void * args,u32 dwTimeOut);
protected:
	static BOOL32 s_bInitProtocol;

	friend class CSerialEventHandle;
	CSerialEventHandle m_serial;
	u16 m_event;
	u32 m_dwInQueue;
	u32 m_dwOutQueue;
	BOOL32 m_bConnect;
	EMICBDevConnState m_devConnectState;
	BOOL32 m_bPortOpen;
	BOOL32 m_bListenerStart;
	s8 m_strLastErr[128];
	u32 m_dwLastErrNo;
	s32 m_dwAdress;
	s32 m_wComPort;
	TICBCOMCfg::EMBaudrate m_dwBaudRate;
	TICBCOMCfg::EMStopBits m_dwStopBits;
	AddrToObjMap m_mapAddrToObj;//com可有可无
	pfICBConnectFunc m_Connectcb;
	void * m_pConContext;
	pfICBTranslateFunc m_Translatecb;
	void * m_pTransContext;
	u32 m_dwObjMark;
	GSMutex m_cs;
	BOOL32 m_bRecv;
	//u32 m_dwTimeOut;

};	

