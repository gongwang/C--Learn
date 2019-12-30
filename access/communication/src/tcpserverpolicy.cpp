// tcpserverpolicy.cpp: implementation of the TcpServerPolicy class.
//
//////////////////////////////////////////////////////////////////////
#include "tcpserverpolicy.h"
#include "dascommonfun.h"
#include "errorcode.h"

using namespace dascommon;
#define DEF_LOG_NAME	"TCPSER"
#define TCP_OPER_ERR -1
#define TCP_SEL_ENG_MAX_FD_NUM 64 //最大值

//////////////////////////////////////////////////////////////////////////
//静态变量
BOOL32 CTcpServerPolicy::s_bInitProtocol = FALSE;
BOOL32 CTcpServerPolicy::s_bInitEng = FALSE;
HSelectEngine CTcpServerPolicy::s_hSelEng = NULL;

//////////////////////////////////////////////////////////////////////
s32 CTcpServerPolicy::CBNewProc(HTcpListen hTcpListen, SOCKHANDLE hNewSocket, void* pContext, u32 dwPeerIP, u16 wPeerPort)
{
	//ILog("") <<  "HTcpListen: " << hTcpListen << " PeerIp: " << dwPeerIP << " PeerPort: " << wPeerPort; 

	CTcpServerPolicy *pTcp = (CTcpServerPolicy *)pContext;
	if (pTcp != NULL)
	{
		return pTcp->OnNewProc(hTcpListen, hNewSocket, pContext, dwPeerIP, wPeerPort);
	}
	
	return 0;
}

s32 CTcpServerPolicy::CBRecv(HTcpSession hTcpSession)
{
	s8 abyBuff[MAX_LEN_SEND_BUF] = {0};
	
	s32 nRet = TcpSessionRecv(hTcpSession, (s8 *)abyBuff, sizeof(abyBuff));
	if (nRet < 0)
	{
		//ELog(DEF_LOG_NAME) << "TcpSessionRecv Error: " << nRet << " : " << abyBuff;
		//printf("TcpSessionRecv Error nRet = %d\n",nRet);
		return -1;
	}
	else
	{
		u32 dwPeerIp = 0;
		u16 wPeerPort = 0;
		s32 nLen = nRet;
		TcpSessionGetPeerAddr(hTcpSession, &dwPeerIp, &wPeerPort);
		if (nRet > sizeof(abyBuff))
		{
			//MLog(DEF_LOG_NAME) << "接受的TCP数据大于最大的数据缓存";
			//printf("接受的TCP数据大于最大的数据缓存\n");
			nLen = sizeof(abyBuff);
		}
		
		CspString strIp;
		dascommon::IpU32ToStr(dwPeerIp, strIp);
		CspString strDesc;
		strDesc.format("CTcpServerPolicy::CBRecv %s:%d", strIp.c_str(), wPeerPort);
		dascommon::PrintfDataBuff(abyBuff, nLen, strDesc.c_str());
		//printf("————————————————————————\n");
	    //printf("CBRecv: ip = %s\n",strIp.c_str());
		CTcpServerPolicy *pTcp = (CTcpServerPolicy *)TcpSessionGetUserHandle(hTcpSession);

        printf("————————————————————————\n");
        printf("CBRecv: pTcp: %d remote ip = %s remotePort: %d\n",pTcp, strIp.c_str(), wPeerPort);

		if (pTcp != NULL)
		{
			for(ItrObj itrObj = pTcp->m_mapAddrToObj.begin();itrObj != pTcp->m_mapAddrToObj.end();itrObj++)
			{
                printf("CBRecv itrObj->first.m_dwIp : %d dwPeerIp:%d",itrObj->first.m_dwIp, dwPeerIp);

				if (itrObj->first.m_dwIp == dwPeerIp)
				{
					return pTcp->OnRecv((const u8*)abyBuff, nRet,itrObj->second);
				}
			}
		}
	}

	return 0;
}

s32 CTcpServerPolicy::CBSend(HTcpSession hTcpSession)
{
	//ILog(DEF_LOG_NAME) << "CTcpServerPolicy::CBSend";
	
	return 0;
}

s32 CTcpServerPolicy::CBState(HTcpSession hTcpSession, s32 nState)
{
	//ILog(DEF_LOG_NAME) << "CTcpServerPolicy::CBState HTcpSession:" << hTcpSession << " state: " << nState;
	CTcpServerPolicy *pTcp = (CTcpServerPolicy *)TcpSessionGetUserHandle(hTcpSession);
	if (pTcp != NULL)
	{
		return pTcp->OnState(hTcpSession, nState);
	}
	
	return 0;
}


// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTcpServerPolicy::CTcpServerPolicy()
{
	m_bConnect = FALSE;
	m_dwLocalIp = 0;
	m_wLocalPort = 0;
	m_hTcpListen = NULL;
	m_eConnState = emTcpConnState_NULL;
	m_Connectcb = NULL;
	m_Translatecb = NULL;
}

CTcpServerPolicy::~CTcpServerPolicy()
{
	//printf("CTcpServerPolicy::~CTcpServerPolicy()\n");
}

u32 CTcpServerPolicy::SetLocalCfg(const void* ptLocalCfg, u32 dwLocalCfgSize)
{
	TICBAddress * ptICBAddress = (TICBAddress *)ptLocalCfg;
	m_dwLocalIp = INADDR_ANY;
	m_wLocalPort = ptICBAddress->m_wPort;
	//printf("m_localport = %d\n",m_wLocalPort);
	if (sizeof(*ptICBAddress) == dwLocalCfgSize)
	{
		return EMICB_OK;
	}
	else
	{
		return EMICB_FAIL;
	}
}

u32 CTcpServerPolicy::GetLocalCfg(void* ptLocalCfg)
{
	TICBAddress* ptICBAddress = (TICBAddress*)ptLocalCfg;
	ptICBAddress->m_dwIp = m_dwLocalIp;
	ptICBAddress->m_wPort = m_wLocalPort;
	return EMICB_OK;
}

u32 CTcpServerPolicy::BindDeviceInfo(u32 dwObjMark,const TICBAddress* ptDevAddr)
{
	printf("serverbind ip = %d port: %d\n",ptDevAddr->m_dwIp, ptDevAddr->m_wPort);
	Mapret ret;
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

u32 CTcpServerPolicy::UnbindDeviceInfo(u32 dwObjMark, const TICBAddress* ptDevAddr)
{
	//printf("serverUnbind ip = %d\n",ptDevAddr->m_dwIp);
	{
		GSAutoLock csAuto(m_cs);
		for(ItrObj itrObj = m_mapAddrToObj.begin();itrObj != m_mapAddrToObj.end(); ++itrObj)
		{
			if (itrObj->second != dwObjMark){continue;}
			itrObj = m_mapAddrToObj.erase(itrObj);
			break;
		}
	}

	if (ptDevAddr == NULL)
	{
		return EMICB_FAIL;
	}

	{
		GSAutoLock csAuto(m_cs);
		for (ItrHSession itrHSession = m_mapClientSession.begin();itrHSession != m_mapClientSession.end(); ++itrHSession)
		{
			if (itrHSession->first.m_dwIp == ptDevAddr->m_dwIp)
			{
				//printf("find client ip\n");
				if (itrHSession->second != NULL)
				{
					//printf("free hsession\n");
					TcpSessionFree(itrHSession->second);
				}
				m_mapClientSession.erase(itrHSession);
				break;
			}
		}

	}
	
	return EMICB_OK;
}

BOOL32 CTcpServerPolicy::IsEqualDeviceInfo(const TICBAddress* ptDevAddr)
{
	ItrObj itrObj;
	for(itrObj = m_mapAddrToObj.begin();itrObj!= m_mapAddrToObj.end();itrObj++)
	{
		if (itrObj->first.m_dwIp == ptDevAddr->m_dwIp)
		{
			return TRUE;
		}
	}
	return FALSE;
}

u32 CTcpServerPolicy::StartConnect(void)
{
	//printf("CTcpServerPolicy::StartConnect\n");
	if(!InitProtocol())
	{
		//printf("init fail");
		return EMICB_FAIL;
	}
	//对于TcpServer和UdpServer（Udp）来说，相同本地端口只创建一次连接对象
	//若连接已创建，又调用此函数，此处判断可以防止重复开启同一个端口。
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

u32 CTcpServerPolicy::StopConnect()
{
	//printf(" CTcpServerPolicy::StopConnect\n");
	//tcpserver直到删除了该连接的所有设备，才关闭连接
    if (m_mapAddrToObj.empty())
    {
		if (!x_close())
		{
			//printf("x_close fail");
			//ELog(DEF_LOG_NAME) << "失败原因：" << GetLastErrStr();
			return EMICB_FAIL;
		}
		//printf("断开连接\n");
		m_bConnect = FALSE;
		m_wLocalPort = 0;
		m_hTcpListen = NULL;
		m_eConnState = emTcpConnState_NULL;
		ReleaseProtocol();
		return EMICB_OK;
	}
	return EMICB_FORBIDDEN_CLOSE;
	
}
u32 CTcpServerPolicy::PostData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut)
{
	//printf("sendata\n");
	//printf("%s\n",pszData);
	if (m_eConnState != emTcpConnState_LISTENING)
	{
		SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "发送TCPServer数据失败，TCPServer未监听");
		//ELog(DEF_LOG_NAME) << GetLastErrStr();
		return EMICB_FAIL;
	}

	for(ItrObj itrObj = m_mapAddrToObj.begin();itrObj != m_mapAddrToObj.end();itrObj++)
	{
		if (itrObj->second == dwObjMark)
		{
			for (ItrHSession itr = m_mapClientSession.begin();itr != m_mapClientSession.end();itr++)
			{
				if (itr->first.m_dwIp == itrObj->first.m_dwIp)
				{
					if (itr->second != NULL)
					{
						s32 nSentMsgLen = TcpSessionSend(itr->second, const_cast<s8*>
							(reinterpret_cast<const s8*>(pszData)),nLength);        
						if (nSentMsgLen < 0)
						{
							SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "TcpSessionSend Fail");
							//ELog(DEF_LOG_NAME) << GetLastErrStr();
							//printf("TcpSessionSend Fail");
							return EMICB_FAIL;
						}
					}
					else
					{
						SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "发送TCPServer数据失败，对应的设备连接未建立");
						//ELog(DEF_LOG_NAME) << GetLastErrStr();
						//printf("发送TCPServer数据失败，对应的设备连接未建立\n");
						return EMICB_FAIL;
					}
					break;
				}
			}
			break;
		}
	}
	dascommon::PrintfDataBuff(pszData, nLength, "TCPServer发送的数据包");


	m_mapIsRecv[dwObjMark] = FALSE;
	
	if (dwTimeOut != 0)
	{
		SetTimerCB(OnTimer,this,dwObjMark,dwTimeOut);
	}
	
	return EMICB_OK;
}

u32 CTcpServerPolicy::SendData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut)
{
	{
		GSAutoLock csAuto(m_cs);
		
		if (m_eConnState != emTcpConnState_LISTENING)
		{
			SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "发送TCPServer数据失败，TCPServer未监听");
			//ELog(DEF_LOG_NAME) << GetLastErrStr();
			return EMICB_FAIL;
		}

		for(ItrObj itrObj = m_mapAddrToObj.begin();itrObj != m_mapAddrToObj.end();itrObj++)
		{
			if (itrObj->second == dwObjMark)
			{
				for (ItrHSession itr = m_mapClientSession.begin();itr != m_mapClientSession.end();itr++)
				{
					if (itr->first.m_dwIp == itrObj->first.m_dwIp)
					{
						if (itr->second != NULL)
						{
							s32 nSentMsgLen = TcpSessionSend(itr->second, const_cast<s8*>
								(reinterpret_cast<const s8*>(pszData)),nLength);        
							if (nSentMsgLen < 0)
							{
								SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "TcpSessionSend Fail");
								//ELog(DEF_LOG_NAME) << GetLastErrStr();
								//printf("TcpSessionSend Fail");
								return EMICB_FAIL;
							}
						}
						else
						{
							SetLastErr(EV_EMI_ERROR_TCP_SEND_ERROR, "发送TCPServer数据失败，对应的设备连接未建立");
							//ELog(DEF_LOG_NAME) << GetLastErrStr();
							//printf("发送TCPServer数据失败，对应的设备连接未建立\n");
							return EMICB_FAIL;
						}
						break;
					}
				}
				break;
			}
		}
		dascommon::PrintfDataBuff(pszData, nLength, "TCPServer发送的数据包");

		m_mapIsRecv[dwObjMark] = FALSE;

		printf("false = %d\n",m_mapIsRecv[dwObjMark]);
		u32 dwPasstime = 0;
		do 
		{ 
			Sleep(100);
			dwPasstime += 100;
			printf("nPst = %d\n",dwPasstime);
			//printf("send\n");
		} while (m_mapIsRecv[dwObjMark] == FALSE && dwPasstime < dwTimeOut*1000);

		if (m_mapIsRecv[dwObjMark] == FALSE)
		{
			printf("TimeOut\n");
			return EMICB_TIMEOUT;
		}

		return EMICB_OK;
	
	}
}

void  CTcpServerPolicy::SetConnectStateCB(pfICBConnectFunc cb,void *pContext)
{
	//printf("CTcpServerPolicy::SetConnectStateCB\n");
	m_Connectcb = cb;
	m_pTransContext = pContext;
}
void CTcpServerPolicy::SetTranslateCB(pfICBTranslateFunc cb,void *pContext)
{
	//printf("CTcpServerPolicy::SetTranslateCB\n");
	m_Translatecb = cb;
	m_pTransContext = pContext;
}

//////////////////////////////////////////////////////////////////////////

BOOL32 CTcpServerPolicy::x_open()
{
	if (s_hSelEng == NULL)
	{
		SetLastErr(EV_EMI_ERROR_TCP_MODULE_NOT_INIT, "SelectEngine is Null");
		//ELog(DEF_LOG_NAME) << "打开Tcp连接失败，失败原因：" << GetLastErrStr();
		return FALSE;
	}

	m_hTcpListen = TcpListenNew(m_dwLocalIp, m_wLocalPort, 128, s_hSelEng, CBNewProc, (void*)this);
	if (NULL == m_hTcpListen)
	{
		SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpListenNew Fail");
		//ELog(DEF_LOG_NAME) <<  "打开TcpServer连接失败，失败原因："<< GetLastErrStr();
		return FALSE;
	}

	m_eConnState = emTcpConnState_LISTENING;

	return TRUE;
}

BOOL32 CTcpServerPolicy::x_close()
{
	s32 nRet = 0;
	if (m_hTcpListen != NULL)
	{	
		// Stop listen service and free object
		nRet = TcpListenFree(m_hTcpListen);
		if (nRet == TCP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_RELEASE_ERROR, "TcpListenFree Fail");
		}
		
	}
	
	m_eConnState = emTcpConnState_NULL;
	return nRet != SOCKET_ERROR;	
}

//////////////////////////////////////////////////////////////////////////
s32 CTcpServerPolicy::OnRecv(const u8* chData, s32 nSize,u32 dwObj)
{

	m_mapIsRecv[dwObj] = TRUE;

	printf("收到回调\n");
    if (m_Translatecb != NULL)
    {
		m_Translatecb(dwObj,chData,nSize,m_pTransContext);
    }
  
	return 0;
}

s32 CTcpServerPolicy::OnState(u32 dwObj,EMICBDevConnState eState)
{
	m_Connectcb(dwObj,eState,m_pConContext);
	return 0;
}

s32 CTcpServerPolicy::OnState(HTcpSession hTcpSession, s32 nState)
{
	switch(nState)
	{
	case TCP_SESSION_STATE_CONNECT:
		{
			u32 dwPeerIp = 0;
			u16 wPeerPort = 0;
			TcpSessionGetPeerAddr(hTcpSession, &dwPeerIp, &wPeerPort);
			
			CspString strIp;
			dascommon::IpU32ToStr(dwPeerIp, strIp);
			
			//MLog(DEF_LOG_NAME) << "TcpServer Port[" << m_wLocalPort << "]:PeerTcp{IP:" << strIp << " "
			//	<< "Port:" << wPeerPort << "} connected";
			m_devConnectState = EMICB_DEV_CONNECT_DISCONNECTED;
			for(ItrObj itrObj = m_mapAddrToObj.begin();itrObj != m_mapAddrToObj.end();itrObj++)
			{
				if (itrObj->first.m_dwIp == dwPeerIp)
				{
					if (m_Connectcb != NULL)
					{
						m_Connectcb(itrObj->second,m_devConnectState,m_pConContext);
					}
					
				}
			}
		}
		break;	

	case TCP_SESSION_STATE_DISCONN:
		{	
			u32 dwPeerIp = 0;
			u16 wPeerPort = 0;
			TcpSessionGetPeerAddr(hTcpSession, &dwPeerIp, &wPeerPort);
			
			CspString strIp;
			dascommon::IpU32ToStr(dwPeerIp, strIp);
			
			//MLog(DEF_LOG_NAME) << "TcpServer Port[" << m_wLocalPort << "]:PeerTcp{IP:" 
			//	<< strIp << " " << "Port:" << wPeerPort << "} disconnected";

			/*for(ItrHSession itrHSession = m_mapClientSession.begin();itrHSession != m_mapClientSession.end();itrHSession++)
			{
				if (itrHSession->second == hTcpSession )
				{
					if (NULL != itrHSession->second)
					{
						//TcpSessionFree(hTcpSession);
						itrHSession->second = NULL;
					}
				}
			}
			*/
			m_devConnectState = EMICB_DEV_CONNECT_DISCONNECTED;
			for(ItrObj itrObj = m_mapAddrToObj.begin();itrObj != m_mapAddrToObj.end();itrObj++)
			{
				if (itrObj->first.m_dwIp == dwPeerIp)
				{
					if (m_Connectcb != NULL)
					{
						m_Connectcb(itrObj->second,m_devConnectState,m_pConContext);
					}
					
				}
			}
		}
		break;
	default:
		{
			u32 dwPeerIp = 0;
			u16 wPeerPort = 0;
			TcpSessionGetPeerAddr(hTcpSession, &dwPeerIp, &wPeerPort);
			
			CspString strIp;
			dascommon::IpU32ToStr(dwPeerIp, strIp);
			
			//MLog(DEF_LOG_NAME) << "TcpServer Port[" << m_wLocalPort << "]:PeerTcp{IP:" << strIp << " " 
			//	<< "Port:" << wPeerPort << "} state: " << nState;

		}
		break;
	}
	
    return 0;
}

s32 CTcpServerPolicy::OnNewProc( HTcpListen hTcpListen, SOCKHANDLE hNewSocket, void* pContext, u32 dwPeerIP, u16 wPeerPort )
{
	CspString strIp;
	dascommon::IpU32ToStr(dwPeerIP, strIp);
	
	//MLog(DEF_LOG_NAME) << "TcpServer Port[" << m_wLocalPort << "]:PeerTcp{IP:" 
		//<< strIp << " " << "Port:" << wPeerPort << "} connected";
	//printf("CTcpServerPolicy::OnNewProc :ip = %s connected\n",strIp.c_str());

	TICBAddress tICBAddress;
	tICBAddress.m_dwIp = dwPeerIP;
	tICBAddress.m_wPort = wPeerPort;
	ItrHSession itrHSession = m_mapClientSession.find(tICBAddress);
	if (itrHSession == m_mapClientSession.end())//该设备不存在
	{
		s32 nRet = 0;
		HTcpSession hTcpSession = NULL;
		nRet = TcpSessionNew(&hTcpSession, MAX_LEN_SEND_BUF);
		if (nRet == TCP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionNew Fail");
		}
		
		nRet = TcpSessionCreateBySocket(hTcpSession, hNewSocket);
		if (nRet == TCP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionCreateBySocket Fail");
		}
	
		nRet = TcpSessionSetEngine(hTcpSession, s_hSelEng);
		if (nRet == TCP_OPER_ERR)
		{
			   SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetEngine Fail");
		}
	 
		nRet = TcpSessionSetStateCallback(hTcpSession, CTcpServerPolicy::CBState);
		if (nRet == TCP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetStateCallback Fail");
		}
		
		nRet = TcpSessionSetRecvCallback(hTcpSession, CTcpServerPolicy::CBRecv);
		if (nRet == TCP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetRecvCallback Fail");
		}
		
		nRet = TcpSessionSetSendCallback(hTcpSession, CTcpServerPolicy::CBSend);
		if (nRet == TCP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetSendCallback Fail");
		}
		
		
		nRet = TcpSessionSetUserHandle(hTcpSession, (void*)this);
		if (nRet == TCP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetUserHandle Fail");
		}		
		m_mapClientSession[tICBAddress] = hTcpSession;
	}
	else
	{
		HTcpSession hTcpSession = itrHSession->second;
	
		if (hTcpSession != NULL)
		{
			TcpSessionFree(hTcpSession);
			hTcpSession = NULL;
		}

		if (hTcpSession == NULL)
		{
			s32 nRet = 0;
			HTcpSession hTcpSession = NULL;
			nRet = TcpSessionNew(&hTcpSession, MAX_LEN_SEND_BUF);
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionNew Fail");
			
			nRet = TcpSessionCreateBySocket(hTcpSession, hNewSocket);
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionCreateBySocket Fail");
			
			nRet = TcpSessionSetEngine(hTcpSession, s_hSelEng);
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetEngine Fail");
			
			nRet = TcpSessionSetStateCallback(hTcpSession, CTcpServerPolicy::CBState);
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetStateCallback Fail");
			
			nRet = TcpSessionSetRecvCallback(hTcpSession, CTcpServerPolicy::CBRecv);
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetRecvCallback Fail");
			
			nRet = TcpSessionSetSendCallback(hTcpSession, CTcpServerPolicy::CBSend);
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetSendCallback Fail");
			
			nRet = TcpSessionSetUserHandle(hTcpSession, (void*)this);
			SetLastErr(EV_EMI_ERROR_TCP_MODULE_OPEN_ERROR, "TcpSessionSetUserHandle Fail");

			itrHSession->second = hTcpSession;
		}
	}

	return 0;	
}

//////////////////////////////////////////////////////////////////////////
BOOL32 CTcpServerPolicy::InitProtocol()
{
	//printf("InitProtocol\n");
	if (s_bInitProtocol)
	{
		//printf("Protocol已经初始化\n");
		//ILog(DEF_LOG_NAME) << "Protocol已经初始化";
		return TRUE;
	}
	
	//Release();
	s_hSelEng = SelectEngInit(TCP_SEL_ENG_MAX_FD_NUM, "CTcpServerPolicy");
	if (s_hSelEng == NULL)
	{
		//printf("失败原因：SelectEngInit Fail!\n");
		//ELog(DEF_LOG_NAME) << "失败原因：SelectEngInit Fail!";
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

BOOL32 CTcpServerPolicy::ReleaseProtocol()
{
	if (!s_bInitProtocol)
	{
		//ILog(DEF_LOG_NAME) << "Tcp Protocol未初始化";
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
				//SetLastErr(EV_EMI_ERROR_TCP_MODULE_RELEASE_ERROR, "SelectEngUnInit Fail");
				//ELog(DEF_LOG_NAME) << "失败原因：SelectEngUnInit Fail!";
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

void CTcpServerPolicy::SetTimerCB(Timer timer,void * pContent,u32 dwObj,u32 dwTimeOut)
{
	timer(pContent,dwObj,dwTimeOut);
}

void CTcpServerPolicy::OnTimer(void * args,u32 dwObj,u32 dwTimeOut)
{
	CTcpServerPolicy * policy = (CTcpServerPolicy*)args;
	printf("开始计时\n");
	Sleep(dwTimeOut*1000);

	if (policy->m_mapIsRecv[dwObj] == FALSE)
	{
		printf("状态回调\n");
		policy->OnState(dwObj,EMICB_DEV_CONNECT_DISCONNECTED);
	}
}