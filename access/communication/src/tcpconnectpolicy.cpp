#include "tcpconnectpolicy.h"
#include "errorcode.h"


#define DEF_LOG_NAME	"TCPCON"
#define TCP_OPER_ERR -1
#define TCP_SEL_ENG_MAX_FD_NUM 64 //最大值

//静态变量
BOOL32 CTcpConnectPolicy::s_bInitProtocol = FALSE;
BOOL32 CTcpConnectPolicy::s_bInitEng = FALSE;
HSelectEngine CTcpConnectPolicy::s_hSelEng = NULL;

CTcpConnectPolicy::CTcpConnectPolicy()
{
	m_bConnect = FALSE;
	m_dwHostIp = 0;
	m_wHostPort = 0;
	m_hTcpSession = NULL;
	m_eConnState = emTcpConnState_NULL;
	m_Connectcb = NULL;
	m_Translatecb = NULL;
}

CTcpConnectPolicy::~CTcpConnectPolicy()
{
	//printf("CTcpConnectPolicy::~CTcpConnectPolicy()\n");
}

u32 CTcpConnectPolicy::SetLocalCfg(const void* ptLocalCfg, u32 dwLocalCfgSize)
{
	//Tcp做客户端连接不需要设备ip和本地端口号
	return EMICB_OK;
}

u32 CTcpConnectPolicy::GetLocalCfg(void* ptLocalCfg)
{
	//该接口无效
	return EMICB_OK;
}

u32 CTcpConnectPolicy::BindDeviceInfo(u32 dwObjMark,const TICBAddress* ptDevAddr)
{
	m_dwHostIp = ptDevAddr->m_dwIp;
	m_wHostPort = ptDevAddr->m_wPort;
	m_dwObjMark = dwObjMark;
	//printf("m_dwHostIp = %d\n",ptDevAddr->m_dwIp);

	//tcp连接其实可以不用插入map，因为一个对象只会与创建一个与设备的连接，为了与其他连接方式统一而做绑定
	Mapret ret;
	//注意，插入的必须为结构体内容，否则相同内容的结构体，指针地址也会不同，也能插入成功
	ret = m_mapAddrToObj.insert(pair<TICBAddress,u32>(*ptDevAddr,dwObjMark));
	if (ret.second)
	{
		//printf("insert map success\n");
		return EMICB_OK;
	}
	else
	{
		//插入失败说明已存在该设备的连接
		return EMICB_FAIL;
	}
}

u32 CTcpConnectPolicy::UnbindDeviceInfo(u32 dwObjMark, const TICBAddress* ptDevAddr)
{
	{
		GSAutoLock csAuto(m_cs);
		ItrObj itrObj;
		for (itrObj = m_mapAddrToObj.begin();itrObj != m_mapAddrToObj.end();itrObj++)
		{
			if (itrObj->second == dwObjMark)
			{
				itrObj = m_mapAddrToObj.erase(itrObj);	
				break;
			}
		}
	}

	
	return EMICB_OK;
	
}

BOOL32 CTcpConnectPolicy::IsEqualDeviceInfo(const TICBAddress* ptDevAddr)
{
	ItrObj itrObj;
	for(itrObj = m_mapAddrToObj.begin();itrObj!= m_mapAddrToObj.end();itrObj++)
	{
		if ((itrObj->first.m_dwIp == ptDevAddr->m_dwIp)&&(itrObj->first.m_wPort == ptDevAddr->m_wPort))
		{
			return TRUE;
		}
	}

	return FALSE;
}

u32 CTcpConnectPolicy::StartConnect(void)
{
	 if(!InitProtocol())
	 {
		 //printf("InitProtocol fail\n");
		 return EMICB_FAIL;
	 }
	 //对于TcpServer和UdpServer（Udp）来说，若连接已创建，又调用此函数，此处判断可以防止重复开启同一个端口。
	 if (m_bConnect)
	 {
		 return EMICB_OK;
	 }

	 if (!x_open())
	 {
		 //printf("x_open fail\n");
		 StopConnect();
		 return EMICB_FAIL;
	 }
	 m_bConnect = TRUE;
	 return EMICB_OK;
}

u32 CTcpConnectPolicy::StopConnect()
{
	
	//若为tcpserver,直到删除了该连接的所有设备，才关闭连接
	if (m_mapAddrToObj.empty())
	{
		if (!x_close())
		{
			//ELog(DEF_LOG_NAME) <<"TCP connect close fail, reason:"<<GetLastErrStr();
			return EMICB_FAIL;
		}
		m_bConnect = FALSE;
		//printf("设备连接已断开\n");
		ReleaseProtocol();
		return EMICB_OK;
	}
	
	return EMICB_FORBIDDEN_CLOSE;
}
u32 CTcpConnectPolicy::PostData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut)
{
	//printf("send data = %s\n",pszData);
	//tcp连接不需要根据对象找相应连接发送数据，如果是tcpserver,则将对象指针和hTcpSession保存进map中
	if (m_eConnState != emTcpConnState_CONNECTED)
	{
		SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "tcp connect is not establish");
		//ELog(DEF_LOG_NAME)<<"send data fail, reason:"<<GetLastErrStr();

		return EMICB_FAIL;
	}
	m_bRecv = FALSE;
	s32 nSentMsgLen = TcpSessionSend(m_hTcpSession, const_cast<s8*>(reinterpret_cast<const s8*>(pszData)), nLength);         
	if (nSentMsgLen < 0)
	{
		SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "TcpSessionSend Fail");
		//ELog(DEF_LOG_NAME)<<"send data fail, reason:"<<GetLastErrStr()
		//	<<" return value: "<<nSentMsgLen;
		//printf("Send data fail\n");
		return EMICB_FAIL;
	}
	if (dwTimeOut != 0)
	{
		SetTimerCB(OnTimer,this,dwTimeOut);
	}

	return EMICB_OK;
}

u32 CTcpConnectPolicy::SendData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut)
{
	if (m_eConnState != emTcpConnState_CONNECTED)
	{
		SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "tcp connect is not establish");
		//ELog(DEF_LOG_NAME)<<"send data fail, reason:"<<GetLastErrStr();

		return EMICB_FAIL;
	}

	s32 nSentMsgLen = TcpSessionSend(m_hTcpSession, const_cast<s8*>(reinterpret_cast<const s8*>(pszData)), nLength);        
	if (nSentMsgLen < 0)
	{
		SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "TcpSessionSend Fail");
		//ELog(DEF_LOG_NAME)<<"send data fail, reason:"<<GetLastErrStr()
		//	<<" return value: "<<nSentMsgLen;
		//printf("Send data fail\n");
		return EMICB_FAIL;
	}

	m_bRecv = FALSE;
	u32 dwPasstime = 0;
	do 
	{ 
		Sleep(100);
		dwPasstime += 100;
		//printf("nPst = %d\n",dwPasstime);
		//printf("send\n");
	} while (m_bRecv == FALSE && dwPasstime < dwTimeOut*1000);

	if (m_bRecv == FALSE)
	{
		//printf("TimeOut\n");
		return EMICB_TIMEOUT;
	}

	return EMICB_OK;
}

void CTcpConnectPolicy::SetConnectStateCB(pfICBConnectFunc cb,void *pContext)
{
	m_Connectcb = cb;
    m_pConContext = pContext;
}

void CTcpConnectPolicy::SetTranslateCB(pfICBTranslateFunc cb,void *pContext)
{
	m_Translatecb = cb;
	m_pTransContext = pContext;
}

void CTcpConnectPolicy::SetTimerCB(Timer timer,void * pContent,u32 dwTimeOut)
{
	timer(pContent,dwTimeOut);
}

//////////////////////////////////////////////////////////////////////////
s32 CTcpConnectPolicy::CBRecv(HTcpSession hTcpSession)
{
	//ILog(DEF_LOG_NAME)<< "session:"<<std::hex<<hTcpSession;

	s8 abyBuff[MAX_LEN_DATA_BUF] = {0};
	s32 nRet = TcpSessionRecv(hTcpSession, (s8 *)abyBuff, sizeof(abyBuff));
	if (nRet < 0)
	{
		//ELog(DEF_LOG_NAME)<<"[TcpConnectPolicy::CBRecv]"
		//	<<" TcpSessionRecv Error Ret:"<<nRet
		//	<<" Buff:"<<abyBuff;
		//printf("TcpSessionRecv Error Ret = %d\n",nRet);
		return -1;
	}
	else
	{
		s32 nLen = nRet;
		if (nRet > sizeof(abyBuff))
		{
			//WLog(DEF_LOG_NAME)<<"[TcpConnectPolicy::CBRecv]"
			//	<<" The receive buff is out of the max array";
			//printf("The receive buff is out of the max array\n");
			nLen = sizeof(abyBuff);
		}

		u32 dwPeerIp = 0;
		u16 wPeerPort = 0;
		TcpSessionGetPeerAddr(hTcpSession, &dwPeerIp, &wPeerPort);
	   
		//printf("CTcpConnectPolicy::CBRecv ip= %d\n",dwPeerIp);

		CTcpConnectPolicy *pTcp = (CTcpConnectPolicy *)TcpSessionGetUserHandle(hTcpSession);

		if (pTcp != NULL) 
		{
			return pTcp->OnRecv((const u8*)abyBuff, nRet,pTcp->m_dwObjMark);
		}
		else 
		{
			//WLog(DEF_LOG_NAME)<<"[TcpConnectPolicy::CBRecv]"
			//	<<" session"
			//	<<" user handle is null";
			//printf("CBRecv:user handle is null\n");
		}
	}

	return 0;
}

s32 CTcpConnectPolicy::CBSend(HTcpSession hTcpSession)
{
	//ILog(DEF_LOG_NAME) << "session:"<<std::hex<<hTcpSession;

	return 0;
}

s32 CTcpConnectPolicy::CBState(HTcpSession hTcpSession, s32 nState)
{
	//ILog(DEF_LOG_NAME)<<"session:"<<std::hex<<hTcpSession <<" state: "<<nState;

	CTcpConnectPolicy *pTcp = (CTcpConnectPolicy *)TcpSessionGetUserHandle(hTcpSession);

	if (pTcp != NULL)
	{	
		//tcp不用查map,只有一个设备对象
		return pTcp->OnState(nState,pTcp->m_dwObjMark);
	}
	else 
	{
		//WLog(DEF_LOG_NAME) << " user handle is null";
		//printf("CBState:user handle is null\n");
	}


	return 0;
}


//////////////////////////////////////////////////////////////////////////

BOOL32 CTcpConnectPolicy::x_open()
{
	static const s8* FuncName = "[CTcpConnectPolicy::x_open]: ";
	if (s_hSelEng == NULL)
	{
		SetLastErr(EV_EMI_ERROR_TCP_MODULE_NOT_INIT, "SelectEngine is Null");
		//ELog(DEF_LOG_NAME)<<"open fail, reason:"<<GetLastErrStr();
		//printf("SelectEngine is Null\n");
		return FALSE;
	}

	s32 nRet = 0;
	s32 nWaitSeconds = 0;
	if (emTcpConnState_NULL == m_eConnState)
	{
		nRet = TcpSessionNew(&m_hTcpSession, MAX_LEN_SEND_BUF);
		if (TCP_OPER_ERR == nRet)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionNew Fail");
			//printf("TcpSessionNew Fail\n");
		}
		m_eConnState = emTcpConnState_IDLE;
	}

	if (emTcpConnState_IDLE == m_eConnState)
	{
		nRet = TcpSessionCreate(m_hTcpSession,INADDR_ANY, 0);
		if (TCP_OPER_ERR == nRet)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_INIT_ERROR, "TcpSessionCreate Fail");
			//printf("TcpSessionCreate Fail\n");
		}
		
		nRet = TcpSessionSetEngine(m_hTcpSession, s_hSelEng);
		if (TCP_OPER_ERR == nRet)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_INIT_ERROR, "TcpSessionSetEngine Fail");
			//printf("TcpSessionSetEngine Fail");
		}
		
		nRet = TcpSessionSetStateCallback(m_hTcpSession, CTcpConnectPolicy::CBState);
		if (TCP_OPER_ERR == nRet)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_INIT_ERROR, "TcpSessionSetStateCallback Fail");
		}	

		nRet = TcpSessionSetRecvCallback(m_hTcpSession, CTcpConnectPolicy::CBRecv);
		if (TCP_OPER_ERR == nRet)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_INIT_ERROR, "TcpSessionSetRecvCallback Fail");
			//printf("TcpSessionSetRecvCallback Fail\n");
		}
		
		nRet = TcpSessionSetSendCallback(m_hTcpSession, CTcpConnectPolicy::CBSend);
		if (TCP_OPER_ERR == nRet)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_INIT_ERROR, "TcpSessionSetSendCallback Fail");
		}

		nRet = TcpSessionSetUserHandle(m_hTcpSession, (void*)this);
	    if (TCP_OPER_ERR == nRet)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_INIT_ERROR, "TcpSessionSetUserHandle Fail");
		}
	}

	if (nWaitSeconds > 0)
	{
		nRet = TcpSessionConnectSync(m_hTcpSession, m_dwHostIp, m_wHostPort, nWaitSeconds);
	}
	else
	{
		nRet = TcpSessionConnect(m_hTcpSession, m_dwHostIp, m_wHostPort);
	}

	//OspDelay(100);
	Sleep(100);

	BOOL32 bRet(TRUE);

	if (0 == nRet)
	{
		m_eConnState = emTcpConnState_CONNECTED;
		m_devConnectState = EMICB_DEV_CONNECT_CONNECTED;
		//printf("client connect success\n");
	}
	else if (-2 == nRet)
	{
		if (m_eConnState != emTcpConnState_CONNECTED)
		{
			m_eConnState = emTcpConnState_CONNECTING;

			m_devConnectState = EMICB_DEV_CONNECT_CONNECTING;
		}
	}
	else
	{
		SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "open tcp connect fail");
		//printf("open tcp connect fail\n");
		m_devConnectState = EMICB_DEV_CONNECT_CONNECT_FAIL;
		
		bRet = FALSE;
	}

	return bRet;
}

BOOL32 CTcpConnectPolicy::x_close()
{
	static const s8* FuncName = "[CTcpConnectPolicy::x_close]: ";
	s32 nRet = 0;
	if (m_hTcpSession != NULL)
	{	
		if (emTcpConnState_CONNECTED == m_eConnState 
			|| emTcpConnState_CONNECTING == m_eConnState )
		{
			nRet = TcpSessionClose(m_hTcpSession);
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_RELEASE_ERROR, "TcpSessionClose Fail");

			m_eConnState = emTcpConnState_IDLE;
		}

		if (emTcpConnState_IDLE == m_eConnState)
		{
			nRet = TcpSessionFree(m_hTcpSession);
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_RELEASE_ERROR, "UdpSessionFree Fail");

			m_hTcpSession = NULL;
			m_eConnState = emTcpConnState_NULL;
		}
	}

	m_eConnState = emTcpConnState_NULL;
	m_devConnectState = EMICB_DEV_CONNECT_DISCONNECTED;
	return nRet != SOCKET_ERROR;	
}

s32 CTcpConnectPolicy::OnState(u32 dwObj,EMICBDevConnState eState)
{
	m_Connectcb(dwObj,eState,m_pConContext);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
s32 CTcpConnectPolicy::OnRecv(const u8* chData, s32 nSize,u32 dwObj)
{
	m_bRecv = TRUE;	
	if (m_Translatecb != NULL)
	{
	    m_Translatecb(dwObj,chData,nSize,m_pTransContext);
	}
	
	return 0;
}

s32 CTcpConnectPolicy::OnState(s32 nState,u32 dwObj)
{
	switch(nState)
	{
	case TCP_SESSION_STATE_CONNECT:
		{
			m_eConnState = emTcpConnState_CONNECTED;

			m_devConnectState = EMICB_DEV_CONNECT_CONNECTED;	
		}
		break;	
	case TCP_SESSION_STATE_DISCONN:
	case TCP_SESSION_STATE_DISCONN_SEND_ERROR:
	default:
		{
			m_eConnState = emTcpConnState_IDLE;

			m_devConnectState = EMICB_DEV_CONNECT_DISCONNECTED;
			
		
		}
		break;
	}
	if (m_Connectcb != NULL)
	{
		m_Connectcb(dwObj,m_devConnectState,m_pConContext);
	}
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////
BOOL32 CTcpConnectPolicy::InitProtocol()
{
	//printf("InitProtocol\n");
	if (s_bInitProtocol)
	{
		//ILog(DEF_LOG_NAME) << "Tcp Connect Policy has Init";
		//printf("Tcp Connect Policy has Init\n");
		return TRUE;
	}

	//Release();
	s_hSelEng = SelectEngInit(TCP_SEL_ENG_MAX_FD_NUM, "CTcpConnectPolicy");
	if (s_hSelEng == NULL)
	{
		//printf("SelectEngInit Fail\n");
		//ELog(DEF_LOG_NAME) <<  "SelectEngInit Fail";
		return FALSE;
	}
	if (!SKBSockInit())
	{
		return FALSE;
	}
	s_bInitEng = TRUE;
	s_bInitProtocol = TRUE;
	return TRUE;
}

BOOL32 CTcpConnectPolicy::ReleaseProtocol()
{
	//printf("ReleaseProtocol\n");
	if (!s_bInitProtocol)
	{
		//ELog(DEF_LOG_NAME) << "Tcp Connect Policy do not Init";
		//printf("Tcp Connect Policy do not Init\n");
		return TRUE;
	}	


	s32 nRet = 0;
	if (s_hSelEng != NULL)
	{
		if (s_bInitEng)
		{
			nRet = SelectEngUnInit(s_hSelEng);
			if (nRet == TCP_OPER_ERR)
			{
				//ELog(DEF_LOG_NAME) << "SelectEngUnInit Fail";
				//printf("SelectEngUnInit Fail\n");
				return FALSE;
			}
			s_bInitEng = FALSE;
		}

		s_hSelEng = NULL;
	}
	if (!SKBSockCleanup())
	{
		return FALSE;
	}
	s_bInitProtocol = FALSE;
	return TRUE;
}

void CTcpConnectPolicy::OnTimer(void * args,u32 dwTimeOut)
{
	CTcpConnectPolicy * policy = (CTcpConnectPolicy*)args;
	printf("开始计时\n");
	Sleep(dwTimeOut*1000);
	if (policy->m_bRecv == FALSE)
	{
		printf("状态回调\n");
		policy->OnState(policy->m_dwObjMark,EMICB_DEV_CONNECT_DISCONNECTED);
	}
}