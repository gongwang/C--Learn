
#include "testtcpobj.h"
//TCP����
int main()
{
	printf("CLIENT\n");

	CTestTcpObj *Obj = new CTestTcpObj();

	//�������Ӷ���
	Obj->CreateConnectPolicy();

	printf("obj = %d\n",(u32)Obj);
	TICBAddress tDevAddress;
	tDevAddress.m_dwIp = inet_addr("127.0.0.1");
	tDevAddress.m_wPort = 2503;
	//�������ӹ���
	Obj->ProcConnectPolicy(&tDevAddress);

	//for (int i = 0;i < 5;i++)
	//{
		//Sleep(5000);
		u8 data[]= {'a','b','c','d','\0'};
		//��������
		Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8),5);
	//}
	

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
//COM����
#include "testcomobj.h"
int main()
{
	CTestComObj* Obj = new CTestComObj();
	Obj->CreateConnectPolicy();
	TICBCOMCfg tComCfg;//������������ڵ�ַ�����ظ�
	tComCfg.m_dwAddress = 1111;
	tComCfg.m_dwComport = 2;
	tComCfg.m_emBaudrate = tagICBCOMCfg::EBaud9600;
	tComCfg.m_emStopbits = tagICBCOMCfg::EStop1;

	Obj->ProcConnectPolicy(&tComCfg);

	//for (u8 i = 0;i < 5;i++)
	//{
		Sleep(5000);
		//��������ʱ��ü�'\0'����ֹ����
		u8 data[]= {'g','h','i','j','k','l','m','n','\0'};
		//��������
		//Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8),5);
		Obj->PostDataConnectPolicy(data,sizeof(data)/sizeof(u8),5);
	//}
	

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

#include "testudpobj.h"
//UDP����
int main()
{
	CTestUdpObj *Obj = new CTestUdpObj();

	TICBAddress tLocalCfg;
	tLocalCfg.m_dwIp =inet_addr("127.0.0.1");
	tLocalCfg.m_wPort = 2502;//ȷ���ö˿�û�б�ռ��
	//�������Ӷ���
	Obj->CreateConnectPolicy();

	printf("obj = %d\n",(u32)Obj);
	TICBAddress tCIBAddress;
	//����Ǳ�������д127.0.0.1,��Ҫдʵ��IP������ص������ղ���
	tCIBAddress.m_dwIp = inet_addr("127.0.0.1");
	tCIBAddress.m_wPort = 2501;
	//�������ӹ���
	Obj->ProcConnectPolicy(&tLocalCfg,&tCIBAddress);

	Sleep(10000);
	//��������ʱһ��Ҫ����'\0'����ֹ����
	u8 data[]= {'a','f','j','\0'};
	//��������
	Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8));

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