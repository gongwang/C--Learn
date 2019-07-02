// xmlTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
/*
	xml 解析示例 一定不允许包含空格 否则解析不出来 by.coco
*/

#include "pch.h"
#include <iostream>
#include "ticpp.h"
using namespace std;

struct  structDOORSTATE
{
public:
	structDOORSTATE()
	{
		this->m_bHasGetDoorState = FALSE;
		this->m_tDoorNo.clear();
		this->m_tLinkState.clear();
		this->m_tAlarmState.clear();
		this->m_tDoorState.clear();
		this->m_tMenace.clear();
		this->m_tInfrState.clear();
		this->m_tAlwayOpen.clear();
		this->m_tAlwayClose.clear();
	};
	BOOL m_bHasGetDoorState;
	string m_tDoorNo;
	string m_tLinkState;
	string m_tAlarmState;
	string m_tDoorState;
	string m_tMenace;
	string m_tInfrState;
	string m_tAlwayOpen;
	string m_tAlwayClose;
};

int main()
{


	std::cout << "Hello World!\n";
	//system("pause");
	structDOORSTATE m_tDoorState;

	//XML 解析
	//XML 解析
	//错误的XML格式 多了空格
	string pchXmlStr = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\
									<MESSAGE NAME = \"DOORSTATE\" TYPE = \"RSP\" SEQNO = \"1\">\
										<DoorNo>0001 < / DoorNo >\
										<LinkState>2 < / LinkState >\
										<AlarmState>3 < / AlarmState >\
										<DoorState>4< / DoorState >\
										<Menace>5 < / Menace >\
										<InfrState>6</ InfrState >\
										<AlwayOpen>7< / AlwayOpen>\
										<AlwayClose>8< / AlwayClose>\
									< / MESSAGE>";
	//正确的XML格式 去掉空格
	const char* cxml = "<?xml version=\"1.0\" encoding=\"gb2312\"?>\
		<MESSAGE NAME = \"DOORSTATE\" TYPE = \"RSP\" SEQNO = \"1\">\
		<DoorNo>0001</DoorNo>\
		<LinkState>1</LinkState>\
		<AlarmState>1</AlarmState>\
		<DoorState>1</DoorState>\
		<Menace>1</Menace>\
		<InfrState>1</InfrState>\
		<AlwayOpen>True</AlwayOpen>\
		<AlwayClose>False</AlwayClose>\
		</MESSAGE>\
		";

	TiXmlDocument *doc = new TiXmlDocument();

	doc->Parse(cxml);

	//TiXmlElement *DataRoot = doc->FirstChildElement();

	TiXmlElement* rootElement = doc->RootElement(); //MESSAGE

	if (NULL == rootElement)
	{
		OutputDebugString("Failed to parse data.");
		doc->Clear();
		return FALSE;
	}

	const char *MsgName = rootElement->Attribute("NAME");  //获得rootElement的name属性
	const char *MsgTYPE = rootElement->Attribute("TYPE");  //获得rootElement的name属性
	const char *MsgSEQNO = rootElement->Attribute("SEQNO");  //获得rootElement的name属性
	
	cout << "MsgName:" << MsgName << "  MsgTYPE:" << MsgTYPE << "  MsgSEQNO：" << MsgSEQNO << endl;

	TiXmlElement* DoorNoElement = rootElement->FirstChildElement("DoorNo");  //DoorNo

	TiXmlElement* LinkStateElement = rootElement->FirstChildElement("LinkState");  //LinkState

	TiXmlElement* AlarmStateElement = rootElement->FirstChildElement("AlarmState");  //AlarmState

	TiXmlElement* DoorStateElement = rootElement->FirstChildElement("DoorState");  //DoorState

	TiXmlElement* MenaceElement = rootElement->FirstChildElement("Menace");  //Menace

	TiXmlElement* InfrStateElement = rootElement->FirstChildElement("InfrState");  //InfrState

	TiXmlElement* AlwayOpenElement = rootElement->FirstChildElement("AlwayOpen");  //AlwayOpen

	TiXmlElement* AlwayCloseElement = rootElement->FirstChildElement("AlwayClose");  //AlwayClose

	if (!DoorNoElement || !LinkStateElement || !AlarmStateElement || !DoorStateElement || !MenaceElement || !InfrStateElement || !AlwayOpenElement || !AlwayCloseElement)
	{
		printf("节点解析失败!\n");
	}
	//while (DoorNoElement)
	//{
	//	cout << "name: " << DoorNoElement->Value() << " value: " << DoorNoElement->GetText() << endl; //7 ？？？

	//	DoorNoElement = DoorNoElement->NextSiblingElement();
	//}

	/*
	TiXmlElement* LinkStateElement		= DoorNoElement->NextSiblingElement();

	TiXmlElement* AlarmStateElement		= LinkStateElement->NextSiblingElement();

	TiXmlElement* DoorStateElement			= AlarmStateElement->NextSiblingElement();

	TiXmlElement* MenaceElement				= DoorStateElement->NextSiblingElement();

	TiXmlElement* InfrStateElement				= MenaceElement->NextSiblingElement();

	TiXmlElement* AlwayOpenElement			= InfrStateElement->NextSiblingElement();

	TiXmlElement* AlwayCloseElement			= AlwayOpenElement->NextSiblingElement();
	*/
	cout << "DoorNoElement:" << DoorNoElement->GetText() << "  LinkStateElement:" << LinkStateElement->GetText() << "  LinkStateElement：" << LinkStateElement->GetText()
		<<"  AlarmStateElement: "<< AlarmStateElement ->GetText()<<"  DoorStateElement:"<< DoorStateElement ->GetText()<<"  MenaceElement:"<< MenaceElement ->GetText()<<
		"  InfrStateElement:"<< InfrStateElement ->GetText()<<"  AlwayOpenElement:"<< AlwayOpenElement ->GetText()<<"  AlwayCloseElement:"<< AlwayCloseElement ->GetText()<< endl;

	m_tDoorState.m_tLinkState = LinkStateElement->GetText();
	m_tDoorState.m_tAlarmState = AlarmStateElement->GetText();
	m_tDoorState.m_tDoorState = DoorStateElement->GetText();
	m_tDoorState.m_tMenace = MenaceElement->GetText();
	m_tDoorState.m_tInfrState = InfrStateElement->GetText();
	m_tDoorState.m_tAlwayOpen = "False";
	m_tDoorState.m_tAlwayClose = "True";

	//memset(&m_tDoorState, 0, sizeof(structDOORSTATE));

	//区别对待
	if (0 == strcmp( m_tDoorState.m_tAlarmState.c_str(), AlarmStateElement->GetText())) //告警 EFORCE_OPEN_WARNING
	{
		cout << "AlarmStateElement";
	}
	else
	{
		cout << "AlarmStateElement";
	}

	if (0 == strcmp( m_tDoorState.m_tDoorState.c_str(), DoorStateElement->GetText())) //门状态
	{
		cout << "DoorStateElement";
	}
	else
	{
		cout << "AlarmStateElement";
	}

	if (0 == strcmp( m_tDoorState.m_tMenace.c_str(), MenaceElement->GetText())) //胁迫告警
	{
		cout << "MenaceElement";
	}
	else
	{
		cout << "AlarmStateElement";
	}

	if (0 == strcmp( m_tDoorState.m_tAlwayOpen.c_str(), AlwayOpenElement->GetText())) //门长开
	{
		cout << "AlwayOpenElement";
	}
	else
	{
		cout << "AlarmStateElement";
	}

	if (0 == strcmp( m_tDoorState.m_tAlwayClose.c_str(), AlwayCloseElement->GetText())) //门长闭
	{
		cout << "AlwayCloseElement";
	}
	else
	{
		cout << "AlarmStateElement";
	}

	/*
	string pchXmlStr =
		"<Class name=\"计算机软件班\">\
			<Students>\
				<student name=\"张三\" studentNo=\"13031001\" sex=\"男\" age=\"22\">\
					<phone>88208888</phone>\
					<address>西安市太白南路二号</address>\
					<tel>172.0.0.1</tel>\
					<DoorNo>0001 </ DoorNo >\
					<LinkState>1 </ LinkState >\
					<AlarmState>1 </ AlarmState >\
					<DoorState>1 </ DoorState >\
					<Menace>1 </ Menace >\
					<InfrState>1 </ InfrState >\
					<AlwayOpen>True</ AlwayOpen>\
					<AlwayClose>False</ AlwayClose>\
				</student>\
				<student name=\"李四\" studentNo=\"13031002\" sex=\"男\" age=\"20\">\
					<phone>88206666</phone>\
					<address>西安市光华路</address>\
					<tel>172.0.0.2</tel>\
					<DoorNo>0001 </ DoorNo >\
					<LinkState>1 </ LinkState >\
					<AlarmState>1 </ AlarmState >\
					<DoorState>1 </ DoorState >\
					<Menace>1 </ Menace >\
					<InfrState>1 </ InfrState >\
					<AlwayOpen>True</ AlwayOpen>\
					<AlwayClose>False</ AlwayClose>\
				</student>\
			</Students>\
		</Class>";
	TiXmlDocument* myDocument = new TiXmlDocument();
	myDocument->Parse(pchXmlStr.c_str());

	TiXmlElement* rootElement = myDocument->RootElement();  //Class
	TiXmlElement* studentsElement = rootElement->FirstChildElement();  //Students
	TiXmlElement* studentElement = studentsElement->FirstChildElement();  //Students

	while (studentElement)
	{
		//崩溃 不知道原因
		//TiXmlAttribute* attributeOfStudent = studentElement->FirstAttribute();  //获得student的name属性
		//
		//if (attributeOfStudent)
		//{
		//	cout<<attributeOfStudent->Name() << ":"<<attributeOfStudent->Value();
		//}
	

		const char *name = studentElement->Attribute("name");
		const char *studentNo = studentElement->Attribute("studentNo");
		const char *sex = studentElement->Attribute("sex");
		const char *age = studentElement->Attribute("age");
		cout << "name:" << name << "studentNo:" << studentNo << "sex：" << sex << "age:" << age << endl;

		TiXmlElement* phoneElement = studentElement->FirstChildElement();//获得student的phone元素
		std::cout << "phone" << " : " << phoneElement->GetText() << std::endl;
		TiXmlElement* addressElement = phoneElement->NextSiblingElement();
		std::cout << "address" << " : " << addressElement->GetText() << std::endl;
		TiXmlElement* telElement = addressElement->NextSiblingElement();
		std::cout << "telphone" << " : " << telElement->GetText() << std::endl;

		TiXmlElement* doornoElement = telElement->NextSiblingElement();
		std::cout << "doorno" << " : " << doornoElement->GetText() << std::endl;
		

		studentElement = studentElement->NextSiblingElement();
		system("pause");
	}*/
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
