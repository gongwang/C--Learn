#include "comconnectpolicy.h"
#include "dasstring.h"
#define DEF_LOG_NAME	"COMCON"


BOOL32 CComConnectPolicy::s_bInitProtocol = TRUE;

//////////////////////////////////////////////////////////////////////////
CComConnectPolicy::CComConnectPolicy()
	: m_event(SERIAL_DATA_NOTY)
	, m_dwInQueue(0)
	, m_dwOutQueue(0)
	, m_bPortOpen(FALSE)
	,m_bConnect(FALSE)
	,m_bRecv(FALSE)
{
	m_Connectcb = NULL;
	m_Translatecb = NULL;
	m_serial.SetPolicy(this);
}

CComConnectPolicy::~CComConnectPolicy()
{
	//printf("CComConnectPolicy::~CComConnectPolicy\n");
}

u32 CComConnectPolicy::SetLocalCfg(const void* ptLocalCfg, u32 dwLocalCfgSize)
{
	TICBCOMCfg * ptICBComCfg = (TICBCOMCfg*)ptLocalCfg;
	m_wComPort = ptICBComCfg->m_dwComport;
	m_dwBaudRate = ptICBComCfg->m_emBaudrate;
	m_dwStopBits = ptICBComCfg->m_emStopbits;
	m_dwAdress = ptICBComCfg->m_dwAddress;

	if (sizeof(*ptICBComCfg) == dwLocalCfgSize)
	{
		return EMICB_OK;
	}
	else
	{
		return EMICB_FAIL;
	}
}

u32 CComConnectPolicy::GetLocalCfg(void* ptLocalCfg)
{
	TICBCOMCfg* ptICBComCfg = (TICBCOMCfg*)ptLocalCfg;
	ptICBComCfg->m_dwAddress = m_dwAdress;
	ptICBComCfg->m_dwComport = m_wComPort;
	ptICBComCfg->m_emBaudrate = m_dwBaudRate;
	ptICBComCfg->m_emStopbits = m_dwStopBits;
	return EMICB_OK;
}

u32 CComConnectPolicy::BindDeviceInfo(u32 dwObjMark,const TICBAddress* ptDevAddr)
{
	m_dwObjMark = dwObjMark;
	//com连接其实可以不用插入map，因为一个对象只会与创建一个与设备的连接，为了与其他连接方式统一而做绑定
	Mapret ret;
	//注意，插入的必须为结构体内容，否则相同内容的结构体，指针地址也会不同，也能插入成功
	ret = m_mapAddrToObj.insert(pair<s32,u32>(m_dwAdress,dwObjMark));
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

u32 CComConnectPolicy::UnbindDeviceInfo(u32 dwObjMark, const TICBAddress* ptDevAddr)
{
	{
		GSAutoLock csAuto(m_cs);
		ItrObj itrObj;
		for (itrObj = m_mapAddrToObj.begin();itrObj != m_mapAddrToObj.end();itrObj++)
		{
			if (itrObj->second == dwObjMark)
			{
				itrObj = m_mapAddrToObj.erase(itrObj);//注意，迭代器指向erase返回值	
				break;
			}

		}
	}

	return EMICB_OK;
}	

BOOL32 CComConnectPolicy::IsEqualDeviceInfo(const TICBAddress* ptDevAddr)
{
	//com该接口无效
	return TRUE;
}

u32 CComConnectPolicy::StartConnect(void)
{
	//printf("StartConnect\n");
	if(!InitProtocol())
	{
		//printf("InitProtocol fail\n");
		return EMICB_FAIL;
	}
	
	if (m_bConnect)
	{
		//printf("设备已连接\n");
		return EMICB_OK;
	}

	if (!x_open())
	{
	    //printf("x_open fail\n");
		SetLastErr(EV_EMI_ERROR_COM_MODULE_OPEN_ERROR,"com connect open fail");
		StopConnect();
		return EMICB_FAIL;
	}
	m_devConnectState = EMICB_DEV_CONNECT_CONNECTED;
	OnState(m_devConnectState);
	m_bConnect = TRUE;
	m_event = SERIAL_DATA_NOTY;
	return EMICB_OK;
}

u32 CComConnectPolicy::StopConnect()
{
	//printf("StopConnect\n");
	if (m_mapAddrToObj.empty())
	{
		if (!x_close())
		{
			//printf("x_close fail\n");
			//ELog(DEF_LOG_NAME)<<"com connect close fail, reason:"<<GetLastErrStr();
			return FALSE;
		}
		//printf("所有设备均已断开连接\n");
		Clear();
		m_bConnect = FALSE;
		ReleaseProtocol();
		return EMICB_OK;
	}
	return EMICB_FORBIDDEN_CLOSE;
}
u32 CComConnectPolicy::PostData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut)
{
	//printf("send data = %s\n",pszData);
	if (!m_bConnect)
	{
		SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR, "com connect is not establish");
		//ELog(DEF_LOG_NAME)<<"send data fail, reason:"<<GetLastErrStr();
		return FALSE;
	}

	LONG lLastError = m_serial.Write(reinterpret_cast<const void*>(pszData), nLength);
	if (ERROR_SUCCESS != lLastError)
	{
		SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR, "Write data Fail");
		//ELog(DEF_LOG_NAME)<<"send data fail, reason:"<<GetLastErrStr()
		//<<" error code: "<<lLastError;
		//printf("Send data fail\n");
		return EMICB_FAIL;
	}

	dascommon::PrintfDataBuff(pszData, nLength, "Com Send Bag");
	m_bRecv = FALSE;
	if (dwTimeOut != 0)
	{
		//m_dwTimeOut = dwTimeOut;
		SetTimerCB(OnTimer,this,dwTimeOut);
	}
	return EMICB_OK;
}

u32 CComConnectPolicy::SendData(u32 dwObjMark, const u8* pszData, s32 nLength, u32 dwTimeOut)
{
	//printf("send data = %s\n",pszData);
	if (!m_bConnect)
	{
		SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR, "com connect is not establish");
		//ELog(DEF_LOG_NAME)<<"send data fail, reason:"<<GetLastErrStr();
		return FALSE;
	}
	m_bRecv = FALSE;
	u32 dwPasstime = 0;
	LONG lLastError = m_serial.Write(reinterpret_cast<const void*>(pszData), nLength);
	if (ERROR_SUCCESS != lLastError)
	{
		SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR, "Write data Fail");
		//ELog(DEF_LOG_NAME)<<"send data fail, reason:"<<GetLastErrStr()
		//<<" error code: "<<lLastError;
		//printf("Send data fail\n");
		return EMICB_FAIL;
	}
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

	dascommon::PrintfDataBuff(pszData, nLength, "Com Send Bag");

	return EMICB_OK;
}

void CComConnectPolicy::SetConnectStateCB(pfICBConnectFunc cb,void *pContext)
{
	m_Connectcb = cb;
	m_pConContext = pContext;
}

void CComConnectPolicy::SetTranslateCB(pfICBTranslateFunc cb,void *pContext)
{
	m_Translatecb = cb;
	m_pTransContext = pContext;
}


void CComConnectPolicy::SetTimerCB(Timer timer,void * pContent,u32 dwTimeOut)
{
	timer(pContent,dwTimeOut);
}

BOOL32 CComConnectPolicy::x_close(void)
{
	LONG lLastError = ERROR_SUCCESS;
	BOOL32 bRet(FALSE);
	m_devConnectState = EMICB_DEV_CONNECT_DISCONNECT_FAIL;
	do 
	{
		if (m_bListenerStart)
		{
			lLastError = m_serial.StopListener();
			if (ERROR_SUCCESS != lLastError)
			{
				SetLastErr(EV_EMI_ERROR_DEV_CLOSE_FAIL, "stoplistener fail");
				//ELog(DEF_LOG_NAME)<<"stop listener com"<<m_wComPort
					//<<" fail, reason:"<<GetLastErrStr();
				//printf("Stop listener fail\n");
			}

			m_bListenerStart = FALSE;
		}

		if (m_bPortOpen)
		{
			lLastError = m_serial.Close();
			if (ERROR_SUCCESS != lLastError)
			{
				SetLastErr(EV_EMI_ERROR_DEV_CLOSE_FAIL,"close com fail");
				//ELog(DEF_LOG_NAME)<<"close com"<<m_wComPort
				//	<<" fail, reason:"<<GetLastErrStr();
				//printf("Close com fail\n");
			}
			m_bPortOpen = FALSE;
		}

		bRet = TRUE;
		m_devConnectState = EMICB_DEV_CONNECT_DISCONNECTED;
	} while (0);

	return bRet;
}

BOOL32 CComConnectPolicy::x_open()
{
	m_devConnectState = EMICB_DEV_CONNECT_IDLE;
	if (!CheckPort(m_wComPort))
	{
		//ELog(DEF_LOG_NAME)<<"Check com"<<m_wComPort
		//	<<" fail, reason:"<<GetLastErrStr();
		printf("Check com%d fail\n",m_wComPort);
		SetLastErr(EV_EMI_ERROR_COM_NOT_PRESENT,"Check com fail");
		return FALSE;
	}
	
    dascommon::CspString strPort;
	dascommon::ComIntToStr(m_wComPort, strPort);
	long lLastError = m_serial.Open(strPort.c_str(), m_dwInQueue, m_dwOutQueue);
	if (lLastError != ERROR_SUCCESS)
	{
		SetLastErr(EV_EMI_ERROR_DEV_CFG_ERROR, "Open com fail");
		//ELog(DEF_LOG_NAME)<<"Open com"<<m_wComPort
			//<<" fail, reason:"<<GetLastErrStr();
		printf("Open com%d fail\n",m_wComPort);
		return FALSE;
	}

	m_bPortOpen = TRUE;

	// Start the listener thread
	lLastError = m_serial.StartListener();
	if (lLastError != ERROR_SUCCESS)
	{
		SetLastErr(EV_EMI_ERROR_DEV_CFG_ERROR," m_serial.StartListener fail");
		//ELog(DEF_LOG_NAME)<<"Start Listener com"<<m_wComPort
		//	<<" fail, reason:"<<GetLastErrStr();
		printf("start listener com%d fail\n",m_wComPort);
		return FALSE;
	}

	m_bListenerStart = TRUE;

	lLastError = m_serial.Setup((CSerial::EBaudrate)m_dwBaudRate, 
		CSerial::EData8, CSerial::EParNone, (CSerial::EStopBits)m_dwStopBits);
	//ILog(DEF_LOG_NAME) << "bandrate: " << (CSerial::EBaudrate)m_dwBaudRate << " stop: " << m_dwStopBits;
	if (lLastError != ERROR_SUCCESS)
	{
		m_serial.Close();
		SetLastErr(EV_EMI_ERROR_COM_CONFIG_ERROR, "串口参数配置错误");
		//ELog(DEF_LOG_NAME)<<"setup params com"<<m_wComPort
		//	<<" fail, reason:"<<GetLastErrStr();
		printf("Setup params com%d fail-%d-%d-%d-%d\n",m_wComPort, m_dwBaudRate, CSerial::EData8, CSerial::EParNone, m_dwStopBits);
		return FALSE;
	}

	m_devConnectState = EMICB_DEV_CONNECT_CONNECTING;

	return TRUE;
}

void CComConnectPolicy::SetParam( u16 event/*=-1*/, 
	DWORD dwInQueue /*= 0*/, 
	DWORD dwOutQueue /*= 0*/ )
{
	m_event = event;

	m_dwInQueue = dwInQueue;
	m_dwOutQueue = dwOutQueue;
}

BOOL32 CComConnectPolicy::InitProtocol()
{
	return TRUE;
}

BOOL32 CComConnectPolicy::ReleaseProtocol()
{
	return TRUE;
}

s32 CComConnectPolicy::OnRecv(const u8* chData, s32 nSize)
{
    m_bRecv = TRUE;	
	if (m_Translatecb != NULL)
	{
		m_Translatecb(m_dwObjMark,chData,nSize,m_pTransContext);	
	}
	return 0;
}
s32 CComConnectPolicy::OnState(EMICBDevConnState eState)
{
	if (m_Connectcb != NULL)
	{
		m_Connectcb(m_dwObjMark,eState,m_pConContext);
	}
	return 0;
}
void CComConnectPolicy::Clear()
{
	m_wComPort = 0;
	m_dwBaudRate = TICBCOMCfg::EBaud9600;
	m_event = SERIAL_DATA_NOTY;
}

BOOL32 CComConnectPolicy::CheckPort(const u16 wPort)
{
	dascommon::CspString strPort;
	dascommon::ComIntToStr(wPort, strPort);

	CSerial::EPort ePort = CSerial::CheckPort(strPort.c_str());
	if (CSerial::EPortAvailable != ePort)
	{
		switch(ePort)
		{
		case CSerial::EPortNotAvailable:
			SetLastErr(EV_EMI_ERROR_COM_NOT_PRESENT, "port not present");
			break;
		case CSerial::EPortInUse:
			SetLastErr(EV_EMI_ERROR_COM_ISUSE, "port is use");
			break;
		default:
			SetLastErr(EV_EMI_ERROR_DEV_CONNECT_FAIL, "check port failed, unknown error");
			break;
		}

		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void CSerialEventHandle::OnEvent( EEvent eEvent, EError eError )
{
	// Post message to the client window
	//::PostMessage(m_hwndDest,m_nComMsg,MAKEWPARAM(eEvent,eError),LPARAM(m_lParam));
	if (eError)
	{
		//ELog(DEF_LOG_NAME)<<"An internal error occurred.";
		//printf("An internal error occurred.\n");
		//SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR,"An internal error occurred.");
	}
	
	if (eEvent & CSerial::EEventError)
	{
		//ELog(DEF_LOG_NAME)<<"A line-status error occurred.";
		//printf("A line-status error occurred.\n");
		//SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR,"A line-status error occurred.");
	}
		
	if (eEvent & CSerial::EEventBreak)
	{
		//ILog(DEF_LOG_NAME)<<"Break detected on input.";
		//printf("Break detected on input.\n");
		//SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR,"Break detected on input.");
	}
		
	if (eEvent & CSerial::EEventRcvEv)
	{
		//ILog(DEF_LOG_NAME)<<"Event character has been received.";
		//printf("Event character has been received.\n");
		//SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR,"Event character has been received.");
	}
		
	if (eEvent & CSerial::EEventRing)
	{
		//ILog(DEF_LOG_NAME)<<"Ring detected.";
		//printf("Ring detected.\n");
		//SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR,"Ring detected.");
	}
		
	if (eEvent & CSerial::EEventSend)
	{
		//ILog(DEF_LOG_NAME)<<"All data is send.";
		//printf("All data is send.\n");
		//SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR,"All data is send.");
	}
		
	if (eEvent & CSerial::EEventCTS)
	{
		//ILog(DEF_LOG_NAME)<<"CTS signal change. CTS="<<(GetCTS()?"on":"off");
		//printf("CTS signal change.\n");
		//SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR,"CTS signal change.");
	}
		
	if (eEvent & CSerial::EEventDSR)
	{
		//ILog(DEF_LOG_NAME)<<"DSR signal change. DSR="<<(GetDSR()?"on":"off");
		//printf("DSR signal change.\n");
		//SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR,"DSR signal change.");
	}
		
	if (eEvent & CSerial::EEventRLSD)
	{
		//ILog(DEF_LOG_NAME)<<"RLSD signal change. RLSD="<<(GetRLSD()?"on":"off");
		//printf("RLSD signal change.\n");
		//SetLastErr(EV_EMI_ERROR_COM_SEND_ERROR,"RLSD signal change.");
	}
		
	if (eEvent & CSerial::EEventRecv)
	{
		// Create a clean buffer
		DWORD dwRead = 0;
		char szData[MAX_LEN_DATA_BUF+1] = {0};
		const int nBuflen = MAX_LEN_DATA_BUF;

		// 			dwRead = 0;
		// 			memset(szData, 0, sizeof(szData));
		m_tDataBuff.Clear();

		// Obtain the data from the serial port
		do
		{
			//printf("read\n");
			Read(szData, nBuflen, &dwRead);
			szData[dwRead] = '\0';
			//printf("szData = %s\n,nBuflen = %d\n,dwReafd = %d\n",szData,nBuflen,dwRead);
			//为了保证信息读取完毕，稍作等待
			Sleep(50);
			if (dwRead != 0)
			{
				m_tDataBuff.CatBuff(szData, (u16)dwRead);
			}
			else
			{
				break;
			}
		} while (dwRead != nBuflen);
	
		if (0 == m_tDataBuff.GetBuffLen())
		{
			return;
		}

		if (NULL != m_pPolicy)
		{
			m_pPolicy->OnRecv(m_tDataBuff.GetBuff(), m_tDataBuff.GetBuffLen());
		}
		else
		{
			//WLog(DEF_LOG_NAME) << "Policy is null";
		}
	}
}	

void CComConnectPolicy::OnTimer(void * args,u32 dwTimeOut)
{
	CComConnectPolicy * policy = (CComConnectPolicy*)args;
	printf("开始计时\n");
	Sleep(dwTimeOut*1000);
	if (policy->m_bRecv == FALSE)
	{
		printf("状态回调\n");
		policy->OnState(EMICB_DEV_CONNECT_DISCONNECTED);
	}
}