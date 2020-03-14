
#include "testtcpobj.h"
//TCP连接
int main()
{
	printf("CLIENT\n");

	CTestTcpObj *Obj = new CTestTcpObj();

	//创建连接对象
	Obj->CreateConnectPolicy();

	printf("obj = %d\n",(u32)Obj);
	TICBAddress tDevAddress;
	tDevAddress.m_dwIp = inet_addr("127.0.0.1");
	tDevAddress.m_wPort = 2503;
	//处理连接过程
	Obj->ProcConnectPolicy(&tDevAddress);

	//for (int i = 0;i < 5;i++)
	//{
		//Sleep(5000);
		u8 data[]= {'a','b','c','d','\0'};
		//发送数据
		Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8),5);
	//}
	

	if (getchar() == 'e')
	{
		//销毁连接对象
		Obj->DestroyConnetPolicy();
	}

	delete Obj;
	Obj = NULL;
	while(getchar() != 'q');

}



#if 0
//COM连接
#include "testcomobj.h"
int main()
{
	CTestComObj* Obj = new CTestComObj();
	Obj->CreateConnectPolicy();
	TICBCOMCfg tComCfg;//必填这四项，串口地址不得重复
	tComCfg.m_dwAddress = 1111;
	tComCfg.m_dwComport = 2;
	tComCfg.m_emBaudrate = tagICBCOMCfg::EBaud9600;
	tComCfg.m_emStopbits = tagICBCOMCfg::EStop1;

	Obj->ProcConnectPolicy(&tComCfg);

	//for (u8 i = 0;i < 5;i++)
	//{
		Sleep(5000);
		//发送数据时最好加'\0'，防止出错
		u8 data[]= {'g','h','i','j','k','l','m','n','\0'};
		//发送数据
		//Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8),5);
		Obj->PostDataConnectPolicy(data,sizeof(data)/sizeof(u8),5);
	//}
	

	if (getchar() == 'e')
	{
		//销毁连接对象
		Obj->DestroyConnetPolicy();
	}

	delete Obj;
	Obj = NULL;
	while(getchar() != 'q');


}
#endif

#if 0

#include "testudpobj.h"
//UDP连接
int main()
{
	CTestUdpObj *Obj = new CTestUdpObj();

	TICBAddress tLocalCfg;
	tLocalCfg.m_dwIp =inet_addr("127.0.0.1");
	tLocalCfg.m_wPort = 2502;//确保该端口没有被占用
	//创建连接对象
	Obj->CreateConnectPolicy();

	printf("obj = %d\n",(u32)Obj);
	TICBAddress tCIBAddress;
	//如果是本机，就写127.0.0.1,不要写实际IP，否则回调数据收不到
	tCIBAddress.m_dwIp = inet_addr("127.0.0.1");
	tCIBAddress.m_wPort = 2501;
	//处理连接过程
	Obj->ProcConnectPolicy(&tLocalCfg,&tCIBAddress);

	Sleep(10000);
	//发送数据时一定要加上'\0'，防止出错
	u8 data[]= {'a','f','j','\0'};
	//发送数据
	Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8));

	if (getchar() == 'e')
	{
		//销毁连接对象
		Obj->DestroyConnetPolicy();
	}

	delete Obj;
	Obj = NULL;
	while(getchar() != 'q');
}

#endif