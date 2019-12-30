#pragma once

#include "socketbase/socketbase.h"
#include "iconnectbase.h"
#include <map>
#include <string>
#include "gsautoLock.h"
using namespace std;
class CUdpConnectPolicy : public IConnectBase
{

	typedef map<TICBAddress,u32> AddrToObjMap;
	typedef AddrToObjMap::iterator ItrObj;
	typedef pair<ItrObj,bool> Mapret;

	enum EUdpConnState
	{
		emUdpConnState_NULL             = 0,

		emUdpConnState_IDLE,
		emUdpConnState_RUN,
	};

	enum {ConnetMode = EMICB_PROTO_Udp};//连接模式

public:
	CUdpConnectPolicy();
	virtual ~CUdpConnectPolicy();

public:
	//初始化协议
	static BOOL32 InitProtocol();
	//释放协议
	static BOOL32 ReleaseProtocol();
	//是否初始化协议
	static BOOL32 IsInitProtocol(){return s_bInitProtocol;};

	virtual u32 SetLocalCfg(const void* ptLocalCfg= NULL, u32 dwLocalCfgSize=0);
	virtual u32 GetLocalCfg(void* ptLocalCfg = NULL);
	virtual u32 BindDeviceInfo(u32 dwObjMark,const TICBAddress* ptDevAddr = NULL);
	virtual u32 UnbindDeviceInfo(u32 dwObjMark, const TICBAddress* ptDevAddr=NULL );
	virtual BOOL32 IsEqualDeviceInfo(const TICBAddress* ptDevAddr);
	virtual u32 StartConnect(void);
	virtual u32 StopConnect();
	virtual u32 PostData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut = 0);
	virtual u32 SendData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut = 0);//无效
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

	
private:
	BOOL32 x_open();
	BOOL32 x_close();
	void Clear();

	s32 OnRecv(u32 dwHostIp, u16 wHostPort, const s8* chData, s32 nSize);

	static s32 CBRecv(HUdpSession hUdpSession);
private:
	static BOOL32 s_bInitProtocol;
	static BOOL32 s_bInitEng;
	static HSelectEngine s_hSelEng;

	AddrToObjMap m_mapAddrToObj;
	u32 m_dwLocalIp;
	u16	m_wLocalPort;
	s8 m_strLastErr[128];
	u32 m_dwLastErrNo;
	BOOL32 m_bConnect;
	HUdpSession m_hUdpSession;
	EUdpConnState m_eConnState;
	EMICBDevConnState m_devConnectState;
	pfICBConnectFunc m_Connectcb;
	void * m_pConContext;
	pfICBTranslateFunc m_Translatecb;
	void * m_pTransContext;
	GSMutex m_cs;
};


	

