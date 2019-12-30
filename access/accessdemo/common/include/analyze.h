/*========================================================================================
    ģ����    ��analyze
    �ļ���    ��analyze.h
    ����ļ�  ��
    ʵ�ֹ���  �����ݽ��� json/xml�ķ�װʵ�� ʾ��
    ����      ��zhaojunxiang
    ��Ȩ      ��<Copyright(C) 2003-2020 Suzhou Keda Technology Co., Ltd. All rights reserved.>
    -------------------------------------------------------------------------------------------
    �޸ļ�¼��
    ����               �汾              �޸���             �߶���              �޸ļ�¼
    2019/12/18         V1.0        zhaojunxiang                         �½��ļ�
    =========================================================================================*/

#ifndef _ANALYZE_H
#define _ANALYZE_H

#include "cJSON.h" //json ��װ��������

#include "ticpp/ticpp.h" //XML������


#endif

/************************************************************************/
/* 

JSON ����ʾ����

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


JSON ����ʾ����

cJSON *pJsonRoot = cJSON_CreateObject();
if(NULL == pJsonRoot)return FALSE;
cJSON_AddStringToObject(pJsonRoot,"cmd","getname"); //������ӽڵ�

cJSON* pRecode  = cJSON_CreateObject();//�ӽڵ�
cJSON_AddNumberToObject(pRecode,"sessionid",ptHttpMsgData->GetSessionId());
cJSON_AddItemToObject(pRecode,"head",pHead);

cJSON* pSpVidResArray = cJSON_CreateArray();//array
cJSON_AddItemToObject(pSpVideoMode, "spvidres", "test");
cJSON_AddItemToArray(pSpVideoModeArray, pSpVideoMode);
cJSON_AddItemToObject(pSpVidResArray,"head",pHead);

XML ����ʾ����

TiXmlDocument* myDocument = new TiXmlDocument();

myDocument->Parse(cDate); //�������ݽ���

s8 * fileName = "config\\adapterconfig.xml";
myDocument->LoadFile(fileName); //���ļ�����

const char *MsgName = myDocument->Attribute("encoding");  //���rootElement��encoding����
const char *MsgTYPE = myDocument->Attribute("version");  //���rootElement��version����

TiXmlElement* rootElement = myDocument->RootElement();  //resouce
TiXmlElement* adapterElement = rootElement->FirstChildElement();  //adaptertypes
TiXmlElement * sqliteElement = adapterElement->FirstChildElement();//sqlite

TiXmlElement* pathElement = sqliteElement->FirstChildElement();//path
printf("path: %s\n", pathElement->GetText());
sqlitePath = pathElement->GetText();

���£�
<?xml version="1.0" encoding="utf-8"?>
<resouce>
    <adaptertypes>
           <sqlite>
                <path>D:\\configdb\\ismp_adpmng.db</path>
            </sqlite>
    </adaptertypes>
</resouce>

XML ����ʾ����

//����һ��XML���ĵ�����
TiXmlDocument *myDocument =new TiXmlDocument();
//����һ����Ԫ�ز����ӡ�
TiXmlElement *RootElement =new TiXmlElement("Persons");
myDocument->LinkEndChild(RootElement);
//����һ��PersonԪ�ز����ӡ�
TiXmlElement *PersonElement =new TiXmlElement("Person");
RootElement->LinkEndChild(PersonElement);
//����PersonԪ�ص����ԡ�
PersonElement->SetAttribute("ID","1");
//����nameԪ�ء�ageԪ�ز����ӡ�
TiXmlElement *NameElement =new TiXmlElement("name");
TiXmlElement *AgeElement =new TiXmlElement("age");
PersonElement->LinkEndChild(NameElement);
PersonElement->LinkEndChild(AgeElement);
//����nameԪ�غ�ageԪ�ص����ݲ����ӡ�
TiXmlText *NameContent =new TiXmlText("������");
TiXmlText *AgeContent =new TiXmlText("22");
NameElement->LinkEndChild(NameContent);
AgeElement->LinkEndChild(AgeContent);
CString appPath = GetAppPath();
string seperator ="\\";
string fullPath = appPath.GetBuffer(0)+seperator+szFileName;
myDocument->SaveFile(fullPath.c_str());//���浽�ļ�


�����ļ� .ini��ȡʾ����

#define LEBON_CONFIG_FILE				"d:/ismp/lebon_cfg.ini"
#define LEBON_CONFIG_SECTION		"CONFIG"
#define LEBON_CONFIG_DEVNO		    "DevNo"

//���ƴ������ļ���ȡ�������޷��޸�����
s8 szConfigKey[64] = {0};
s8 szChanKeyCall[64] = {0};
s8 szChanKeyCalled[64] = {0};
s8 szChanValueCall[128] = {0};
s8 szChanValueCalled[128] = {0};        
_snprintf(szChanKeyCall, sizeof(szChanKeyCall) - 1, "%s%s", LEBON_CONFIG_DEVNO, cCallNo);
_snprintf(szChanKeyCalled, sizeof(szChanKeyCalled) - 1, "%s%s", LEBON_CONFIG_DEVNO, cCalledNo);
u32 dwRead = GetPrivateProfileString(LEBON_CONFIG_SECTION, szChanKeyCall, "", szChanValueCall, sizeof(szChanValueCall) - 1, LEBON_CONFIG_FILE);
dwRead = GetPrivateProfileString(LEBON_CONFIG_SECTION, szChanKeyCalled, "", szChanValueCalled, sizeof(szChanValueCalled) - 1, LEBON_CONFIG_FILE);
if(dwRead <= 0)	// ��ȡ��������
{
    LogPrintf("��ȡ�����ļ�����: %s\n",LEBON_CONFIG_FILE);
    strCallName = strCallName;
    strCalledName = strCalledName;
}else
{
    strCallName = ASCII2UTF_8(std::string(szChanValueCall)); //�����ȡ�������������ļ����������
    strCalledName = ASCII2UTF_8(std::string(szChanValueCalled)); //�����ȡ�������������ļ����������
}

*/
/************************************************************************/
