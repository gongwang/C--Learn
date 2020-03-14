#pragma once
#include "iconnectbase.h"
#include "kdvtype.h"
#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#include <iostream>
#include <map>

#pragma comment(lib,"ws2_32.lib")
class CTestUdpObj
{
public:
	CTestUdpObj(){};
	virtual ~CTestUdpObj(){};

	//�ص�������������ͨ��Ա�������þ�̬�������߶���������
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
	//�������Ӷ���
	void CreateConnectPolicy()
	{
		//����һ���µ����Ӷ���
		m_policy = NewConnectObject(EMICB_PROTO_Udp);
	}

	void ProcConnectPolicy(const void * ptLoacolCfg,const TICBAddress* ptDevAddress)
	{
		m_policy->SetLocalCfg(ptLoacolCfg,sizeof(ptLoacolCfg));
		m_policy->BindDeviceInfo((u32)this,ptDevAddress);
		m_tDevAddress = *ptDevAddress;
		printf("this = %d\n",(u32)this);
		m_policy->SetConnectStateCB(CBConnect,NULL);
		m_policy->SetTranslateCB(CBData,NULL);
		m_policy->StartConnect();
	}

	void PostDataConnectPolicy(const u8 *pszData,s32 nLength,u32 dwTimeOut = 0)
	{
		m_policy->PostData((u32)this,pszData,nLength,dwTimeOut);	
	}

	void DestroyConnetPolicy()
	{
		//TCPserver�����ʱ������ڶ�������
		m_policy->UnbindDeviceInfo((u32)this,&m_tDevAddress);
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

private:
	TICBAddress m_tDevAddress;
};
