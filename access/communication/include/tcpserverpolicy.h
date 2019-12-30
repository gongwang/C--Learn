#pragma once
#include "socketbase/socketbase.h"
#include "databuff.h"
#include <map>
#include <set>
#include "iconnectbase.h"
#include <algorithm>
#include <string>
#include "gsautoLock.h"
using namespace std;

#define MAX_LEN_SEND_BUF MAX_LEN_DATA_BUF

class CTcpServerPolicy  : public IConnectBase
{
	typedef map<TICBAddress,u32> AddrToObjMap;
	typedef AddrToObjMap::iterator ItrObj;
	typedef pair<ItrObj,bool> Mapret;

	typedef map<TICBAddress,HTcpSession> AddrToHSessionMap;
	typedef AddrToHSessionMap::iterator ItrHSession;

	typedef map<u32,BOOL32> ObjIsRecv;//该Obj是否收到消息

	enum ETcpConnState
	{
		emTcpConnState_NULL             = 0,
			
			emTcpConnState_IDLE,
			emTcpConnState_CONNECTING,
			emTcpConnState_CONNECTED,
			emTcpConnState_PEERDROP,
			emTcpConnState_WAITCONNECT,

			emTcpConnState_LISTENING, //Tcp Server 已经建立监听
	};

	enum {ConnetMode = EMICB_PROTO_TCPSERVER};//连接模式	

public:
	CTcpServerPolicy();
	virtual ~CTcpServerPolicy();


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


private:
	BOOL32 x_open();
	BOOL32 x_close();

	s32 OnRecv(const u8* chData, s32 nSize,u32 dwObj);
	s32 OnState(HTcpSession hTcpSession, s32 nState);
	s32 OnState(u32 dwObj,EMICBDevConnState eState);
	s32 OnNewProc(HTcpListen hTcpListen, SOCKHANDLE hNewSocket, void* pContext, u32 dwPeerIP, u16 wPeerPort);
	//void OnConnectState(DEV_CONNECT_STATE devState, u32 dwAddress = -1);

private:
	static s32 CBRecv(HTcpSession hTcpSession);
	static s32 CBState(HTcpSession hTcpSession, s32 nState);
	static s32 CBSend(HTcpSession hTcpSession);
	static s32 CBNewProc(HTcpListen hTcpListen, SOCKHANDLE hNewSocket, void* pContext, u32 dwPeerIP, u16 wPeerPort);
private:
	typedef void (*Timer)(void * args,u32 dwObj,u32 dwTimeOut);
	void SetTimerCB(Timer timer,void * pContent,u32 dwObj,u32 dwTimeOut);
	static void OnTimer(void * args,u32 dwObj,u32 dwTimeOut);	
private:
	static BOOL32 s_bInitProtocol;
	static HSelectEngine s_hSelEng;
	static BOOL32 s_bInitEng;
	AddrToObjMap m_mapAddrToObj;
	ObjIsRecv m_mapIsRecv;
	u32 m_dwLocalIp;
	u16 m_wLocalPort;
	AddrToHSessionMap m_mapClientSession;
	EMICBDevConnState m_devConnectState;
	BOOL32 m_bConnect;
	s8 m_strLastErr[128];
	u32 m_dwLastErrNo;
	HTcpListen m_hTcpListen;
	ETcpConnState m_eConnState;//TCP连接状态

	pfICBConnectFunc m_Connectcb;
	void * m_pConContext;
	pfICBTranslateFunc m_Translatecb;
	void * m_pTransContext;
	GSMutex m_cs;
};

