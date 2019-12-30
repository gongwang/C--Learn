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

	//�ص�������������ͨ��Ա�������þ�̬�������߶���������
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
	//�������Ӷ���
	void CreateConnectPolicy()
	{
		//����һ���µ����Ӷ���
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
			printf("Ӧ��ʱ\n");
		}
	
	}

	void PostDataConnectPolicy(const u8 *pszData,s32 nLength,u32 dwTimeOut = 0)
	{
		 m_policy->PostData((u32)this,pszData,nLength,dwTimeOut);	
	}

	void DestroyConnetPolicy()
	{
		//TCPserver�����ʱ������ڶ�������
		m_policy->UnbindDeviceInfo((u32)this);
		u32 dwRet = m_policy->StopConnect();
		//�����豸���ѶϿ�����
		if (dwRet == EMICB_OK)
		{
			DelConnectObject(m_policy);
			m_policy = NULL;
		}
		//�����豸����
		else if (dwRet == EMICB_FORBIDDEN_CLOSE)
		{
			printf("�ö˿�����δ�Ͽ��������豸�����ɹر�����\n");
		}
	}


public:
	IConnectBase * m_policy;
};
