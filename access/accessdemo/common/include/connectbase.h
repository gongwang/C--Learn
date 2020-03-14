/*========================================================================================
    ģ����    ��iconnectbase
    �ļ���    ��iconnectbase.h
    ����ļ�  ��
    ʵ�ֹ���  ��ͨ�ſ� tcp/udp/com/tcp server/http client/http server/https��ķ�װʵ��
    ����      ��zhaojunxiang
    ��Ȩ      ��<Copyright(C) 2003-2020 Suzhou Keda Technology Co., Ltd. All rights reserved.>
    -------------------------------------------------------------------------------------------
    �޸ļ�¼��
    ����               �汾              �޸���             �߶���              �޸ļ�¼
    2019/12/18         V1.0        zhaojunxiang                         �½��ļ�
    =========================================================================================*/
#ifndef _CONNECT_BASE_H
#define _CONNECT_BASE_H

#include "iconnectbase.h"//���ӿ� tcp��udp��com��tcpserver
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#include "httpclient.h" //���� httpclient

#include "http_server.h"//���� http server

#include "libcurlPack.h"//https

//���ӵ�Э������
enum emuCBProtocolType
{
    emuCB_PROTO_Tcp = 0,	         //TCP���� ,TKDProtocolTCP��Ч
    emuCB_PROTO_Udp,	                //UDP����
    emuCB_PROTO_Com,	                //��������
    emuCB_PROTO_TCPSERVER,      //��ΪTCP �����������豸
    emuCB_PROTO_HTTPClient,      //HTTPClient
    emuCB_PROTO_HTTPServer,     //HTTPServer��    
    emuCB_PROTO_HTTPS,             //HTTPS
};

//Э��������Ϣ
typedef struct tagHTTPServerCfg
{
    tagHTTPServerCfg(){m_strListenUrl ="";m_wListenPort = 0;}
    
    std::string m_strListenUrl;//����url
    u16 m_wListenPort;//���ض˿�

}HTTPServerCfg, *PtHTTPServerCfg;
#ifndef WIN32
__attribute__ ((packed))
#endif

//Э��������Ϣ
typedef struct tagKDICONCfg
{
    tagKDICONCfg(){memset(this, 0, sizeof(tagKDICONCfg));}

    s8  m_szPeerIp[17];   //Զ��ip һ��Ϊ������������ip
    s8  m_szLocalIp[17]; //����IP
    u16 m_wPeerPort;//Զ��Ŀ�Ķ˿�
    u16 m_wLocalPort;//���ض˿�
    TICBCOMCfg m_tICBCOMCfg; //COM������
    HTTPServerCfg m_tICBHTTPServerCfg; //HTTP client ����
}TKDICONCfg, *PtKDICONCfg;
#ifndef WIN32
__attribute__ ((packed))
#endif

//ʹ��˵��
/*
1.tcp/udp/com/tcpserverʹ��˳��

m_pConnectBase = new ConnectBase();

void * ptrConnet = m_pConnectBase->NewConnectBase(emuCB_PROTO_Com);

//    static void TCPCBState(u32 dwObjMark, EMICBDevConnState eDevState,void *pContext);
//    static void TCPCBData(u32 dwObjMark, const u8* data, s32 len, void *pContext);   
m_pConnectBase->SetDataCallBack(TCPCBState ,TCPCBData,this);

m_pConnectBase->StartConnectDev(m_pConnectBase->m_ptrIConnectBase, (void*)&pTKDICONCfg);

m_pConnectBase->StopConnectDev(m_pConnectBase->m_ptrIConnectBase);
m_pConnectBase->DelConnectBase(m_pConnectBase->m_ptrIConnectBase);

2.httpclient ʹ��
m_pConnectBase = new ConnectBase();
void * ptrConnet = m_pConnectBase->NewConnectBase(emuCB_PROTO_HTTPClient);

Response response;
char strsend[150] = "0";
sprintf(strsend, "http://%s:%d/iamsweb/Login", "172.16.131.179", 80);
std::string urlsend = strsend;
std::string pJsonBuf = "{\"username\":\"kedacom\", \"password\":\"kedacom\"}";
m_pConnectBase->HttpReqSync(response, pJsonBuf, urlsend.c_str(), E_POST_METHOD);
std::string strJsonRes = response.get_body();
printf("[%s]get_body:%s\n",m_tConnInfo.m_szObjectName,strJsonRes.c_str());

m_pConnectBase->StopConnectDev(m_pConnectBase->m_ptrIConnectBase);
m_pConnectBase->DelConnectBase(m_pConnectBase->m_ptrIConnectBase);

3.httpserverʹ��
m_pConnectBase = new ConnectBase();
void * ptrConnet = m_pConnectBase->NewConnectBase(emuCB_PROTO_HTTPServer);

TKDICONCfg pTKDICONCfg; 

pTKDICONCfg.m_tICBHTTPServerCfg.m_strListenUrl = "/httplisten"; //ע�� б�� "/" ������ܽ���������
pTKDICONCfg.m_tICBHTTPServerCfg.m_wListenPort= 9900;

printf("ConnectAccessObject httplisten address: %p void: %p\n", HttpListen, (void*)&HttpListen);

m_pConnectBase->StartConnectDev(m_pConnectBase->m_ptrIConnectBase, (void*)&pTKDICONCfg);

//bool HttpListen(std::string strUrl, std::string strBody, mg_connection *c, OnRspCallback ListenBack);
m_pConnectBase->SetDataCallBack(HttpListen);

m_pConnectBase->StartConnectDev(m_pConnectBase->m_ptrIConnectBase, (void*)&pTKDICONCfg);

m_pConnectBase->StopConnectDev(m_pConnectBase->m_ptrIConnectBase);
m_pConnectBase->DelConnectBase(m_pConnectBase->m_ptrIConnectBase);

3.httpsʹ��
m_pConnectBase = new ConnectBase();
void * ptrConnet = m_pConnectBase->NewConnectBase(emuCB_PROTO_HTTPS);

string url = "https://172.16.130.92:24408/syris/device/topology";
string header = "";
HTTPSmethods method = HTTPSmethods::HTTPS_GET;
string content = "";

std::string recv = m_pConnectBase->HttpsReqSync(content, url, HTTPS_POST);

m_pConnectBase->StopConnectDev(m_pConnectBase->m_ptrIConnectBase);
m_pConnectBase->DelConnectBase(m_pConnectBase->m_ptrIConnectBase);

*/


class ConnectBase
{
    //���ⲿ����
public:
    ConnectBase();
    ~ConnectBase();

    /************************************************************************/
    /* ��������                                                  */
    /* connectType:��������                              */
    /************************************************************************/
    void *NewConnectBase(emuCBProtocolType connectType);

    /************************************************************************/
    /* ��ʼ����                                                  */
    /* connectType:��������                              */
    /* cbState��Ϊ״̬�ص�����   
                        tcp/udp/com/tcpserver  �ص� ���� pfICBConnectFunc*/
    /* cbData��Ϊ������Ϣ�ص�����                           ��
                        tcp/udp/com/tcpserver  �ص� ���� pfICBTranslateFunc*/
    /* pContextΪ���ò������ص���������           */
    /************************************************************************/
    BOOL32 SetDataCallBack(void*cbState = NULL, void*cbData = NULL,void *pContext=NULL);

    /************************************************************************/
    /* ���ö�������         ����Ҫ�ó�ȥ ����                                          */
    /************************************************************************/
    BOOL32 SetAliveChek();
    /************************************************************************
    /* ��ʼ����                                                  */
    /* connectType:��������                              */
    /* cb��Ϊtcp��udp��com��tcpserver������Ϣ�ص�����                           */
    /* pContextΪ���ò������ص���������           */
    /************************************************************************/
    BOOL32 StartConnectDev(void* policy, const void* tkdicfg);
        /************************************************************************
    /* �Ͽ�����                                                  */
    /* connectType:��������                              */
    /************************************************************************/
    BOOL32 StopConnectDev(void *policy);
  /************************************************************************
    /* ���ٴ���������ʵ��                                                  */
    /* connectType:��������                              */
    /************************************************************************/
    BOOL32 DelConnectBase(void *policy);
    /************************************************************************/
    /* �������豸�������ݵĽӿ�(�첽)                                       */
    /* dwObjMark:Ϊʹ�ø����Ӷ���ı�ʶ����BindDeviceInfo�ӿ��д���ֵ��ͬ   */
    /* pszData:����ֵָ��                                                   */
    /* nLength:���ݳ���                                                     */
    /* dwTimeOut:���ݴ��ݵĳ�ʱʱ��                                         */ 
    /* ����ֵ�ο� EMICBErrcode                                              */
    /************************************************************************/
    u32 PostAsynData(void *policy, const u8* pszData, s32 nLength, u32 dwTimeOut = 0);
    /************************************************************************/
    /* �������豸�������ݵĽӿ�(ͬ��)                                       */
    /* dwObjMark:Ϊʹ�ø����Ӷ���ı�ʶ����BindDeviceInfo�ӿ��д���ֵ��ͬ   */
    /* pszData:����ֵָ��                                                   */
    /* nLength:���ݳ���                                                     */
    /* dwTimeOut:���ݴ��ݵĳ�ʱʱ��                                         */ 
    /* ����udp������Ч                                                       */
    /* ����ֵ�ο� EMICBErrcode                                              */
    /************************************************************************/
    u32 PostSynData(void *policy, const u8* pszData, s32 nLength, u32 dwTimeOut = 0);

    //http client ֧���Զ�������ͷ
    Response& HttpReqSync(/*OUT*/ Response& response, const std:: string &content, const char* url,
        emMethod method= E_GET_METHOD,int nTimeout = 5000,std::map<std::string,std::string> header = std::map<std::string,std::string>());

    //https ֧���Զ���ͷ
    std::string HttpsReqSync(const std:: string content, const std:: string url,
        HTTPSmethods method= HTTPS_POST,int nTimeout = 5000,std::map<std::string,std::string> header = std::map<std::string,std::string>());

//�ڲ�ʹ��    
public:
    void * m_ptrIConnectBase;
    TKDICONCfg m_pConfig;

    void *m_ptrCBstate;
    void *m_ptrCBData;
    void *m_ptrContext;

    HttpServer* s_pHttpServer;

    BOOL32 SetConnectType(emuCBProtocolType connectType);
    emuCBProtocolType GetConnectType();
private:
    emuCBProtocolType m_emuConnectBase;
};


DWORD WINAPI StartHttpThread(LPVOID lpThreadParameter);

#endif