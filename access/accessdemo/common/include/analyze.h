/*========================================================================================
    模块名    ：analyze
    文件名    ：analyze.h
    相关文件  ：
    实现功能  ：数据解析 json/xml的封装实现 示例
    作者      ：zhaojunxiang
    版权      ：<Copyright(C) 2003-2020 Suzhou Keda Technology Co., Ltd. All rights reserved.>
    -------------------------------------------------------------------------------------------
    修改记录：
    日期               版本              修改人             走读人              修改记录
    2019/12/18         V1.0        zhaojunxiang                         新建文件
    =========================================================================================*/

#ifndef _ANALYZE_H
#define _ANALYZE_H

#include "cJSON.h" //json 组装、解析库

#include "ticpp/ticpp.h" //XML解析库


#endif

/************************************************************************/
/* 

JSON 解析示例：

cJSON *pJson = cJSON_Parse(strResp.c_str());
if(!pJson)
{
    printf("init json data failed!\n");
    return "";
}
cJSON* pChildren = cJSON_GetObjectItem(pJson,"children");
if (!pChildren)
{
    cJSON_Delete(pChildren);
    cJSON_Delete(pJson);
    return "";
}
int  nPayloadCount = cJSON_GetArraySize(pChildren);
for (int nIndex =  0; nIndex < nPayloadCount; nIndex++)
{
    cJSON* pChiChildren = cJSON_GetArrayItem(pChildren, nIndex);

    if (!pChiChildren)
    {
        cJSON_Delete(pChiChildren);
        cJSON_Delete(pJson);
        return "";
    }
}


JSON 构造示例：

cJSON *pJsonRoot = cJSON_CreateObject();
if(NULL == pJsonRoot)return FALSE;
cJSON_AddStringToObject(pJsonRoot,"cmd","getname"); //单纯添加节点

cJSON* pRecode  = cJSON_CreateObject();//子节点
cJSON_AddNumberToObject(pRecode,"sessionid",ptHttpMsgData->GetSessionId());
cJSON_AddItemToObject(pRecode,"head",pHead);

cJSON* pSpVidResArray = cJSON_CreateArray();//array
cJSON_AddItemToObject(pSpVideoMode, "spvidres", "test");
cJSON_AddItemToArray(pSpVideoModeArray, pSpVideoMode);
cJSON_AddItemToObject(pSpVidResArray,"head",pHead);

XML 解析示例：

TiXmlDocument* myDocument = new TiXmlDocument();

myDocument->Parse(cDate); //从裸数据解析

s8 * fileName = "config\\adapterconfig.xml";
myDocument->LoadFile(fileName); //从文件解析

const char *MsgName = myDocument->Attribute("encoding");  //获得rootElement的encoding属性
const char *MsgTYPE = myDocument->Attribute("version");  //获得rootElement的version属性

TiXmlElement* rootElement = myDocument->RootElement();  //resouce
TiXmlElement* adapterElement = rootElement->FirstChildElement();  //adaptertypes
TiXmlElement * sqliteElement = adapterElement->FirstChildElement();//sqlite

TiXmlElement* pathElement = sqliteElement->FirstChildElement();//path
printf("path: %s\n", pathElement->GetText());
sqlitePath = pathElement->GetText();

如下：
<?xml version="1.0" encoding="utf-8"?>
<resouce>
    <adaptertypes>
           <sqlite>
                <path>D:\\configdb\\ismp_adpmng.db</path>
            </sqlite>
    </adaptertypes>
</resouce>

XML 构造示例：

//创建一个XML的文档对象。
TiXmlDocument *myDocument =new TiXmlDocument();
//创建一个根元素并连接。
TiXmlElement *RootElement =new TiXmlElement("Persons");
myDocument->LinkEndChild(RootElement);
//创建一个Person元素并连接。
TiXmlElement *PersonElement =new TiXmlElement("Person");
RootElement->LinkEndChild(PersonElement);
//设置Person元素的属性。
PersonElement->SetAttribute("ID","1");
//创建name元素、age元素并连接。
TiXmlElement *NameElement =new TiXmlElement("name");
TiXmlElement *AgeElement =new TiXmlElement("age");
PersonElement->LinkEndChild(NameElement);
PersonElement->LinkEndChild(AgeElement);
//设置name元素和age元素的内容并连接。
TiXmlText *NameContent =new TiXmlText("周星星");
TiXmlText *AgeContent =new TiXmlText("22");
NameElement->LinkEndChild(NameContent);
AgeElement->LinkEndChild(AgeContent);
CString appPath = GetAppPath();
string seperator ="\\";
string fullPath = appPath.GetBuffer(0)+seperator+szFileName;
myDocument->SaveFile(fullPath.c_str());//保存到文件


配置文件 .ini读取示例：

#define LEBON_CONFIG_FILE				"d:/ismp/lebon_cfg.ini"
#define LEBON_CONFIG_SECTION		"CONFIG"
#define LEBON_CONFIG_DEVNO		    "DevNo"

//名称从配置文件获取，来邦无法修改名称
s8 szConfigKey[64] = {0};
s8 szChanKeyCall[64] = {0};
s8 szChanKeyCalled[64] = {0};
s8 szChanValueCall[128] = {0};
s8 szChanValueCalled[128] = {0};        
_snprintf(szChanKeyCall, sizeof(szChanKeyCall) - 1, "%s%s", LEBON_CONFIG_DEVNO, cCallNo);
_snprintf(szChanKeyCalled, sizeof(szChanKeyCalled) - 1, "%s%s", LEBON_CONFIG_DEVNO, cCalledNo);
u32 dwRead = GetPrivateProfileString(LEBON_CONFIG_SECTION, szChanKeyCall, "", szChanValueCall, sizeof(szChanValueCall) - 1, LEBON_CONFIG_FILE);
dwRead = GetPrivateProfileString(LEBON_CONFIG_SECTION, szChanKeyCalled, "", szChanValueCalled, sizeof(szChanValueCalled) - 1, LEBON_CONFIG_FILE);
if(dwRead <= 0)	// 读取不到数据
{
    LogPrintf("读取配置文件出错: %s\n",LEBON_CONFIG_FILE);
    strCallName = strCallName;
    strCalledName = strCalledName;
}else
{
    strCallName = ASCII2UTF_8(std::string(szChanValueCall)); //如果读取到，则用配置文件的内容替代
    strCalledName = ASCII2UTF_8(std::string(szChanValueCalled)); //如果读取到，则用配置文件的内容替代
}

*/
/************************************************************************/
