#pragma once
#include "iconnectbase.h"
#include "kdvtype.h"
#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#include <iostream>
#include <map>

#pragma comment(lib,"ws2_32.lib")
class CTestComObj
{
public:
	CTestComObj(){};
	virtual ~CTestComObj(){};

	//回调方法不能是普通成员方法，用静态方法或者定义在类外
	static void CBConnect(u32 dwObjMark, EMICBDevConnState eDevState,void *pContext)
	{
		printf("CBConnect\n");
		printf("state = %d\n",eDevState);
	}

	static void CBData(u32 dwObjMark, const u8* pszData, s32 nLength,void *pContext)
	{
		printf("CBData\n");
		printf("data = %s,length = %d\n",pszData,nLength);
	}
	//创建连接对象
	void CreateConnectPolicy()
	{
		//创建一个新的连接对象
		m_policy = NewConnectObject(EMICB_PROTO_Com);
	}

	void ProcConnectPolicy(const void * ptLoacolCfg)
	{
		m_policy->SetLocalCfg(ptLoacolCfg,sizeof(ptLoacolCfg));
		m_policy->BindDeviceInfo((u32)this);
		printf("this = %d\n",(u32)this);
		m_policy->SetConnectStateCB(CBConnect,NULL);
		m_policy->SetTranslateCB(CBData,NULL);
		m_policy->StartConnect();
	}

	void SendDataConnetcPolicy(const u8 *pszData,s32 nLength,u32 dwTimeOut = 0)
	{
		printf("send data+++++++++++++++++++++\n");
		if (m_policy->SendData((u32)this,pszData,nLength,dwTimeOut) == EMICB_TIMEOUT)
		{
			printf("应答超时\n");
		}
	
	}

	void PostDataConnectPolicy(const u8 *pszData,s32 nLength,u32 dwTimeOut = 0)
	{
		 m_policy->PostData((u32)this,pszData,nLength,dwTimeOut);	
	}

	void DestroyConnetPolicy()
	{
		//TCPserver解除绑定时必须填第二个参数
		m_policy->UnbindDeviceInfo((u32)this);
		u32 dwRet = m_policy->StopConnect();
		//所有设备均已断开连接
		if (dwRet == EMICB_OK)
		{
			DelConnectObject(m_policy);
			m_policy = NULL;
		}
		//尚有设备连接
		else if (dwRet == EMICB_FORBIDDEN_CLOSE)
		{
			printf("该端口尚有未断开的连接设备，不可关闭连接\n");
		}
	}


public:
	IConnectBase * m_policy;
};
