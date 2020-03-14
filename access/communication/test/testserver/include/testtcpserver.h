#pragma once
#include "iconnectbase.h"
#include "kdvtype.h"
#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#include <iostream>
#include <map>
//使用<WinSock2.h>头文件依赖该库
#pragma comment(lib,"ws2_32.lib")

typedef std::map<u16,IConnectBase*> ConMap;
class CTestTcpServerObj
{
public:	

	CTestTcpServerObj(){m_bIsConnect = FALSE;m_policy = NULL;};
	virtual ~CTestTcpServerObj(){printf("~CTestTcpServerObj\n");};

	//回调方法不能是普通成员方法，用静态方法或者定义在类外
	static void CBConnect(u32 dwObjMark, EMICBDevConnState eDevState,void *pContext)
	{
		printf("server::CBConnect\n");
		printf("state = %d\n",eDevState);
	}

	static void CBData(u32 dwObjMark, const u8* pszData, s32 nLength,void *pContext)
	{
		printf("server::CBData\n");
		printf("data = %s,length = %d\n",pszData,nLength);
	}
	//创建连接对象
	void CreateConnectPolicy(u16 wLoalPort)
	{
		m_wLoalPort = wLoalPort;
		for(ConMap::iterator itr = s_ConnetMap.begin();itr != s_ConnetMap.end();itr++)
		{
			//已经创建过该端口的连接对象
			if (itr->first == m_wLoalPort)
			{
				printf("该端口已开启\n");
				m_policy = itr->second;//使用已经创建过的policy
				m_bIsConnect = TRUE;
				break;
			}
		}
		//创建一个新的连接对象
		if (!m_bIsConnect)
		{
			s_policy = NewConnectObject(EMICB_PROTO_TCPSERVER);
			s_ConnetMap.insert(std::pair<u16,IConnectBase*>(m_wLoalPort,s_policy));
			m_policy = s_policy;
		}
	}

	void ProcConnectPolicy(const void * ptLoalCfg,const TICBAddress* ptClientAddress)
	{
		m_policy->SetLocalCfg(ptLoalCfg,sizeof(ptLoalCfg));
		m_policy->BindDeviceInfo((u32)this,ptClientAddress);
		m_tClientAddress = *ptClientAddress;
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
		m_policy->UnbindDeviceInfo((u32)this,&m_tClientAddress);
		u32 dwRet = m_policy->StopConnect();
		//所有设备均已断开连接
		if (dwRet == EMICB_OK)
		{
			s_ConnetMap.erase(m_wLoalPort);
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
	static  ConMap s_ConnetMap;//保存本地端口号和该端口上创建的连接对象，若该端口已建立连接，则使用已创建的连接对象
	static IConnectBase *s_policy;//因为插入map的是指针地址，普通成员变量在本对象被销毁时指针释放，静态变量不依赖对象的生命周期
	IConnectBase * m_policy;//为防止静态指针的地址被随意更改，另外第一一个成员变量供对象使用

private:
	BOOL32 m_bIsConnect;
	u16 m_wLoalPort;
	TICBAddress m_tClientAddress;
};