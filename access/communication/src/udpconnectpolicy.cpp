#include "udpconnectpolicy.h"
#include "dascommonfun.h"
#include "errorcode.h"
#include <algorithm>

using namespace std;

#define DEF_LOG_NAME	"UDPCON"	



#define UDP_OPER_ERR -1
#define UDP_SEL_ENG_MAX_FD_NUM 64 //最大值


//////////////////////////////////////////////////////////////////////////
s32 CUdpConnectPolicy::CBRecv(HUdpSession hUdpSession)
{
	//ILog(DEF_LOG_NAME) << "session:"<< std::hex << hUdpSession;

	s8 abyBuff[MAX_LEN_DATA_BUF] = {0};
	u32 dwSrcIP = 0;
	u16 wSrcPort = 0;

	s32 nRet = UdpSessionRecvFrom(hUdpSession, abyBuff, sizeof(abyBuff), &dwSrcIP, &wSrcPort);
	if (nRet < 0)
	{
		//ELog(DEF_LOG_NAME) <<" UdpSessionRecv Error Ret:" << nRet <<" Buff:"<<abyBuff;

		TUdpSessionStatus tStatus;
		UdpSessionGetStatus(hUdpSession, &tStatus);
		for(int i = 0;i< tStatus.m_nRmtAddrNum;i++)
		{
			//ELog(DEF_LOG_NAME)<<" UdpSessionRecv Error RemoteIp:"<<tStatus.m_adwRmtIp[i]
			//<<" RemotePort:"<<tStatus.m_awRmtPort[i];
		}

		return -1;
	}
	else
	{
		s32 nLen = nRet;
		if (nRet > sizeof(abyBuff))
		{
			//WLog(DEF_LOG_NAME)<< " The receive buff is out of the max array";
			nLen = sizeof(abyBuff);
		}

		dascommon::CspString strDesc;
		dascommon::IpU32ToStr(dwSrcIP, strDesc);
		strDesc.format("Receive udp buff from %s:%d", strDesc.c_str(), wSrcPort);
		dascommon::PrintfDataBuff(abyBuff, nLen, strDesc.c_str());

		CUdpConnectPolicy *pUdp = (CUdpConnectPolicy *)UdpSessionGetUserHandle(hUdpSession);
		if (pUdp != NULL)
		{
			return pUdp->OnRecv(dwSrcIP, wSrcPort, abyBuff, nRet);
		}
		else 
		{
			//WLog(DEF_LOG_NAME) << " user handle is null";
		}
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
//静态变量

BOOL32 CUdpConnectPolicy::s_bInitProtocol = FALSE;
BOOL32 CUdpConnectPolicy::s_bInitEng = FALSE;
HSelectEngine CUdpConnectPolicy::s_hSelEng = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUdpConnectPolicy::CUdpConnectPolicy()
{
	m_bConnect = FALSE;
	m_Connectcb = NULL;
	m_Translatecb = NULL;
	Clear();
}

CUdpConnectPolicy::~CUdpConnectPolicy()
{
	//printf("CUdpConnectPolicy::~CUdpConnectPolicy\n");
}

u32 CUdpConnectPolicy::SetLocalCfg(const void* ptLocalCfg, u32 dwLocalCfgSize)
{
	TICBAddress * ptICBAddress = (TICBAddress *)ptLocalCfg;
	m_dwLocalIp = ptICBAddress->m_dwIp;
	m_wLocalPort = ptICBAddress->m_wPort;
	if (sizeof(*ptICBAddress) == dwLocalCfgSize)
	{
		return EMICB_OK;
	}
	else
	{
		return EMICB_FAIL;
	}
}

u32 CUdpConnectPolicy::GetLocalCfg(void* ptLocalCfg)
{
	TICBAddress* ptICBAddress = (TICBAddress*)ptLocalCfg;
	ptICBAddress->m_dwIp = m_dwLocalIp;
	ptICBAddress->m_wPort = m_wLocalPort;
	return EMICB_OK;
}

u32 CUdpConnectPolicy::BindDeviceInfo(u32 dwObjMark,const TICBAddress* ptDevAddr)
{
	Mapret ret;
	//注意插入为结构体内容
	ret = m_mapAddrToObj.insert(pair<TICBAddress,u32>(*ptDevAddr,dwObjMark));
	if (ret.second)
	{
		return EMICB_OK;
	}
	else
	{
		//插入失败说明已存在该设备的连接
		return EMICB_FAIL;
	}
}

u32 CUdpConnectPolicy::UnbindDeviceInfo(u32 dwObjMark, const TICBAddress* ptDevAddr)
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

BOOL32 CUdpConnectPolicy::IsEqualDeviceInfo(const TICBAddress* ptDevAddr)
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

u32 CUdpConnectPolicy::StartConnect(void)
{

	if(!InitProtocol())
	{
		//printf("InitProtocol fail!\n");
		return EMICB_FAIL;
	}
	//对于TcpServer和UdpServer（Udp）来说，若连接已创建，又调用此函数，此处判断可以防止重复开启同一个端口。
	if (m_bConnect)
	{
		return EMICB_OK;
	}

	if (!x_open())
	{
		//printf("x_open fail!\n");
		StopConnect();
		return EMICB_FAIL;
	}

	m_bConnect = TRUE;
	return EMICB_OK;
}

u32 CUdpConnectPolicy::StopConnect()
{
	//做server时直到删除了该连接的所有设备，才关闭连接
	if (m_mapAddrToObj.empty())
	{
		if (!x_close())
		{
			//ELog(DEF_LOG_NAME) << "失败原因：" << GetLastErrStr();
			//printf("x_close fail!\n");
			return EMICB_FAIL;
		}
	    Clear();
		//printf("所有设备均已关闭\n");
		m_bConnect = FALSE;
		ReleaseProtocol();
		return EMICB_OK;
	}
	return EMICB_FORBIDDEN_CLOSE;
	
}
u32 CUdpConnectPolicy::PostData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut)
{
	if (m_eConnState != emUdpConnState_RUN)
	{
		SetLastErr(EV_EMI_ERROR_UDP_SEND_ERROR, "Udp connect is not establish");
		//ELog(DEF_LOG_NAME) << "send data fail, reason:"<<GetLastErrStr();

		return EMICB_FAIL;
	}

	s32 nSentMsgLen = -1;
	for (ItrObj itrObj = m_mapAddrToObj.begin();itrObj != m_mapAddrToObj.end();itrObj++)
	{
		if (itrObj->second == dwObjMark)
		{
			nSentMsgLen = UdpSessionSendTo(m_hUdpSession, const_cast<s8*>(reinterpret_cast<const s8*>(pszData)), 
				nLength,itrObj->first.m_dwIp,itrObj->first.m_wPort);   
		}
	}

	if (nSentMsgLen < 0)
	{
		SetLastErr(EV_EMI_ERROR_UDP_SEND_ERROR, "UdpSessionSend Fail");
		//ELog(DEF_LOG_NAME)<< "send data fail, reason:"<<GetLastErrStr() <<" return value: "<<nSentMsgLen;
		//printf("UdpSessionSend Fail\n");
		return EMICB_FAIL;
	}

	dascommon::PrintfDataBuff(pszData, nLength, "Udp Send Bag");

	return EMICB_OK;
}

u32 CUdpConnectPolicy::SendData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut)
{
	return EMICB_OK;
}

void CUdpConnectPolicy::SetConnectStateCB(pfICBConnectFunc cb,void *pContext)
{
	m_Connectcb = cb;
	m_pConContext = pContext;

}

void CUdpConnectPolicy::SetTranslateCB(pfICBTranslateFunc cb,void *pContext)
{
	m_Translatecb = cb;
	m_pTransContext = pContext;
}

//////////////////////////////////////////////////////////////////////////
BOOL32 CUdpConnectPolicy::x_open()
{
	if (s_hSelEng == NULL)
	{
		SetLastErr(EV_EMI_ERROR_UDP_MODULE_NOT_INIT, "SelectEngine is Null");
		//ELog(DEF_LOG_NAME) << "open fail, reason:"<<GetLastErrStr();
		//printf("s_hSelEng == NULL\n");
		return FALSE;
	}

	s32 nRet = 0;
	s32 nWaitSeconds = 0;

	if (emUdpConnState_NULL == m_eConnState)
	{
		nRet = UdpSessionNew(&m_hUdpSession);
		if (nRet == UDP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_UDP_MODULE_OPEN_ERROR, "UdpSessionNew Fail");
			//printf("UdpSessionNew Fail\n");
		}
		

		m_eConnState = emUdpConnState_IDLE;
	}

	if (emUdpConnState_IDLE == m_eConnState)
	{
		//这里IP设置为0，多个IP地址的PC上可能收不到数据
		nRet = UdpSessionCreate(m_hUdpSession, 0, m_wLocalPort, FALSE);
		if (nRet == UDP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_UDP_MODULE_INIT_ERROR, "UdpSessionCreate Fail");
			//printf("UdpSessionCreate Fail");
		}

		nRet = UdpSessionSetEngine(m_hUdpSession, s_hSelEng);
		if (nRet == UDP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_UDP_MODULE_INIT_ERROR, "UdpSessionSetEngine Fail");
			//printf("UdpSessionSetEngine Fail\n");
		}

		nRet = UdpSessionSetRecvCallback(m_hUdpSession, CUdpConnectPolicy::CBRecv);
		if (nRet == UDP_OPER_ERR)
		{
			SetLastErr( EV_EMI_ERROR_UDP_MODULE_INIT_ERROR, "UdpSessionSetRecvCallback Fail");
		}

		nRet = UdpSessionSetUserHandle(m_hUdpSession, (void*)this);
		if (nRet == UDP_OPER_ERR)
		{
			SetLastErr(EV_EMI_ERROR_UDP_MODULE_INIT_ERROR, "UdpSessionSetUserHandle Fail");
		}
		m_eConnState = emUdpConnState_RUN;
	}

	m_devConnectState = EMICB_DEV_CONNECT_DISCONNECTED;

	return TRUE;
}

BOOL32 CUdpConnectPolicy::x_close()
{
	s32 nRet = 0;
	if (m_hUdpSession != NULL)
	{
		if (emUdpConnState_RUN == m_eConnState)
		{
			nRet = UdpSessionClose(m_hUdpSession);
			if (nRet == UDP_OPER_ERR)
			{
				SetLastErr( EV_EMI_ERROR_UDP_MODULE_RELEASE_ERROR, "UdpSessionClose Fail");
			}
			m_eConnState = emUdpConnState_IDLE;
		}

		if (emUdpConnState_IDLE == m_eConnState)
		{
			nRet = UdpSessionFree(m_hUdpSession);
			if (nRet == UDP_OPER_ERR)
			{
				SetLastErr( EV_EMI_ERROR_UDP_MODULE_RELEASE_ERROR, "UdpSessionFree Fail");
			}
			m_eConnState = emUdpConnState_NULL;
		}
	}

	m_eConnState = emUdpConnState_NULL;
	m_devConnectState = EMICB_DEV_CONNECT_DISCONNECTED;

	return nRet != SOCKET_ERROR;	
}

void CUdpConnectPolicy::Clear()
{	
	m_wLocalPort = 0;
	m_dwLocalIp = 0;

	m_hUdpSession = NULL;
	m_eConnState = emUdpConnState_NULL;
}

//////////////////////////////////////////////////////////////////////////
s32 CUdpConnectPolicy::OnRecv(u32 dwHostIp, u16 wHostPort, const s8* chData, s32 nSize)
{
	
	for (ItrObj itrObj = m_mapAddrToObj.begin();itrObj != m_mapAddrToObj.end();itrObj++)
	{
		if (itrObj->first.m_dwIp == dwHostIp && itrObj->first.m_wPort == wHostPort)
		{
			if (m_Translatecb != NULL)
			{
				m_Translatecb(itrObj->second,(const u8 *)chData,nSize,m_pTransContext);
			}
			
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
BOOL32 CUdpConnectPolicy::InitProtocol()
{
	printf("CUdpConnectPolicy::InitProtocol\n");
	if (s_bInitProtocol)
	{
		//ILog(DEF_LOG_NAME) << "Udp Connect Policy has Init";
		return TRUE;
	}

	//Release();
	s_hSelEng = SelectEngInit(UDP_SEL_ENG_MAX_FD_NUM, "UdpConnectPolicy");
	if (s_hSelEng == NULL)
	{
		//ELog(DEF_LOG_NAME) << "SelectEngInit Fail";
		return FALSE;
	}
	if (!SKBSockInit())
	{
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

BOOL32 CUdpConnectPolicy::ReleaseProtocol()
{
	//printf("CUdpConnectPolicy::ReleaseProtocol\n");
	if (!s_bInitProtocol)
	{
		//ELog(DEF_LOG_NAME) << "Udp Connect Policy do not Init";
		return TRUE;
	}	

	s32 nRet = 0;
	if (s_hSelEng != NULL)
	{
		if (s_bInitEng)
		{
			nRet = SelectEngUnInit(s_hSelEng);
			if (nRet == UDP_OPER_ERR)
			{
				//ELog(DEF_LOG_NAME) << "SelectEngUnInit Fail";
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

