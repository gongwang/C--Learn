#include "testtcpserver.h"

//静态变量必须在类外声明
ConMap CTestTcpServerObj::s_ConnetMap;
IConnectBase *CTestTcpServerObj::s_policy;

int main()
{
	printf("SERVER\n");

	CTestTcpServerObj *Obj = new CTestTcpServerObj();

	TICBAddress tICBAdress;
	tICBAdress.m_wPort = 2503;//确保该端口没有被占用
	//创建连接对象
	Obj->CreateConnectPolicy(tICBAdress.m_wPort);

	printf("obj = %d\n",(u32)Obj);
	TICBAddress tClient1;
	//如果是本机，就写127.0.0.1,不要写实际IP，否则回调数据收不到
	tClient1.m_dwIp = inet_addr("127.0.0.1");
	//处理连接过程
	Obj->ProcConnectPolicy(&tICBAdress,&tClient1);

	for(int i = 0;i < 3;i++)
	{
		Sleep(5000);
		//发送数据时最好加上'\0'，防止出错
		u8 data[]= {'a','f','j','\0'};
		//发送数据
		//Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8),10);
		Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8),5);
	}
	

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

#include "testcomobj.h"
int main()
{

	CTestComObj* Obj = new CTestComObj();
	Obj->CreateConnectPolicy();
	TICBCOMCfg tComCfg;//必填这四项，串口地址不得重复
	tComCfg.m_dwAddress = 1000;
	tComCfg.m_dwComport = 3;
	tComCfg.m_emBaudrate = tagICBCOMCfg::EBaud9600;
	tComCfg.m_emStopbits = tagICBCOMCfg::EStop1;

	Obj->ProcConnectPolicy(&tComCfg);

	for (u8 i = 0;i < 5;i++)
	{
		Sleep(5000);
		u8 data[]= {'j','k','l','m','n','f','a','c','d','\0'};
		//发送数据
		Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8));
	}


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
//UDP连接
#include "testudpobj.h"
int main()
{
	CTestUdpObj *Obj = new CTestUdpObj();

	TICBAddress tLocalCfg;
	tLocalCfg.m_dwIp =inet_addr("127.0.0.1");
	tLocalCfg.m_wPort = 2501;//确保该端口没有被占用
	//创建连接对象
	Obj->CreateConnectPolicy();

	printf("obj = %d\n",(u32)Obj);
	TICBAddress tCIBAddress;
	//如果是本机，就写127.0.0.1,不要写实际IP，否则回调数据收不到
	tCIBAddress.m_dwIp = inet_addr("127.0.0.1");
	tCIBAddress.m_wPort = 2502;
	//处理连接过程
	Obj->ProcConnectPolicy(&tLocalCfg,&tCIBAddress);

	for (u8 i = 0;i < 5;i++)
	{ 
		//间隔5秒发送5次数据
		Sleep(5000);
		//发送数据时最好加上'\0'，防止出错
		u8 data[]= {'n','c','q','a','z','w','e','r','y','b','q','\0'};
		//发送数据
		Obj->SendDataConnetcPolicy(data,sizeof(data)/sizeof(u8));
	}

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