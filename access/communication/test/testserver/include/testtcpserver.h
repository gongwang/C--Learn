#pragma once
#include "iconnectbase.h"
#include "kdvtype.h"
#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#include <iostream>
#include <map>
//ʹ��<WinSock2.h>ͷ�ļ������ÿ�
#pragma comment(lib,"ws2_32.lib")

typedef std::map<u16,IConnectBase*> ConMap;
class CTestTcpServerObj
{
public:	

	CTestTcpServerObj(){m_bIsConnect = FALSE;m_policy = NULL;};
	virtual ~CTestTcpServerObj(){printf("~CTestTcpServerObj\n");};

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
	void CreateConnectPolicy(u16 wLoalPort)
	{
		m_wLoalPort = wLoalPort;
		for(ConMap::iterator itr = s_ConnetMap.begin();itr != s_ConnetMap.end();itr++)
		{
			//�Ѿ��������ö˿ڵ����Ӷ���
			if (itr->first == m_wLoalPort)
			{
				printf("�ö˿��ѿ���\n");
				m_policy = itr->second;//ʹ���Ѿ���������policy
				m_bIsConnect = TRUE;
				break;
			}
		}
		//����һ���µ����Ӷ���
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
		m_policy->UnbindDeviceInfo((u32)this,&m_tClientAddress);
		u32 dwRet = m_policy->StopConnect();
		//�����豸���ѶϿ�����
		if (dwRet == EMICB_OK)
		{
			s_ConnetMap.erase(m_wLoalPort);
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
	static  ConMap s_ConnetMap;//���汾�ض˿ںź͸ö˿��ϴ��������Ӷ������ö˿��ѽ������ӣ���ʹ���Ѵ��������Ӷ���
	static IConnectBase *s_policy;//��Ϊ����map����ָ���ַ����ͨ��Ա�����ڱ���������ʱָ���ͷţ���̬�����������������������
	IConnectBase * m_policy;//Ϊ��ֹ��ָ̬��ĵ�ַ��������ģ������һһ����Ա����������ʹ��

private:
	BOOL32 m_bIsConnect;
	u16 m_wLoalPort;
	TICBAddress m_tClientAddress;
};