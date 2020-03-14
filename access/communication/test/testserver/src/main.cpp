#include "testtcpserver.h"

//��̬������������������
ConMap CTestTcpServerObj::s_ConnetMap;
IConnectBase *CTestTcpServerObj::s_policy;

int main()
{
	printf("SERVER\n");

	CTestTcpServerObj *Obj = new CTestTcpServerObj();

	TICBAddress tICBAdress;
	tICBAdress.m_wPort = 2503;//ȷ���ö˿�û�б�ռ��
	//�������Ӷ���
	Obj->CreateConnectPolicy(tICBAdress.m_wPort);

	printf("obj = %d\n",(u32)Obj);
	TICBAddress tClient1;
	//����Ǳ�������д127.0.0.1,��Ҫдʵ��IP������ص������ղ���
	tClient1.m_dwIp = inet_addr("127.0.0.1");
	//�������ӹ���
	Obj->ProcConnectPolicy(&tICBAdress,&tClient1);

	for(int i = 0;i < 3;i++)
	{
		Sleep(5000);
		//��������ʱ��ü���'\0'����ֹ����
		u8 data[]= {'a','f','j','\0'};
		//��������
		//Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8),10);
		Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8),5);
	}
	

	if (getchar() == 'e')
	{
        //�������Ӷ���
		Obj->DestroyConnetPolicy();
	}
	
	delete Obj;
	Obj = NULL;
	while(getchar() != 'q');




	

}


#if 0

#include "testcomobj.h"
int main()
{

	CTestComObj* Obj = new CTestComObj();
	Obj->CreateConnectPolicy();
	TICBCOMCfg tComCfg;//������������ڵ�ַ�����ظ�
	tComCfg.m_dwAddress = 1000;
	tComCfg.m_dwComport = 3;
	tComCfg.m_emBaudrate = tagICBCOMCfg::EBaud9600;
	tComCfg.m_emStopbits = tagICBCOMCfg::EStop1;

	Obj->ProcConnectPolicy(&tComCfg);

	for (u8 i = 0;i < 5;i++)
	{
		Sleep(5000);
		u8 data[]= {'j','k','l','m','n','f','a','c','d','\0'};
		//��������
		Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8));
	}


	if (getchar() == 'e')
	{
		//�������Ӷ���
		Obj->DestroyConnetPolicy();
	}

	delete Obj;
	Obj = NULL;
	while(getchar() != 'q');

}
#endif

#if 0
//UDP����
#include "testudpobj.h"
int main()
{
	CTestUdpObj *Obj = new CTestUdpObj();

	TICBAddress tLocalCfg;
	tLocalCfg.m_dwIp =inet_addr("127.0.0.1");
	tLocalCfg.m_wPort = 2501;//ȷ���ö˿�û�б�ռ��
	//�������Ӷ���
	Obj->CreateConnectPolicy();

	printf("obj = %d\n",(u32)Obj);
	TICBAddress tCIBAddress;
	//����Ǳ�������д127.0.0.1,��Ҫдʵ��IP������ص������ղ���
	tCIBAddress.m_dwIp = inet_addr("127.0.0.1");
	tCIBAddress.m_wPort = 2502;
	//�������ӹ���
	Obj->ProcConnectPolicy(&tLocalCfg,&tCIBAddress);

	for (u8 i = 0;i < 5;i++)
	{ 
		//���5�뷢��5������
		Sleep(5000);
		//��������ʱ��ü���'\0'����ֹ����
		u8 data[]= {'n','c','q','a','z','w','e','r','y','b','q','\0'};
		//��������
		Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8));
	}

	if (getchar() == 'e')
	{
		//�������Ӷ���
		Obj->DestroyConnetPolicy();
	}

	delete Obj;
	Obj = NULL;
	while(getchar() != 'q');
}
#endif