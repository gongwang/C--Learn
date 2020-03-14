/*========================================================================================
    模块名    ：iconnectbase
    文件名    ：iconnectbase.h
    相关文件  ：
    实现功能  ：通信库 tcp/udp/com/tcp server/http client/http server/https库的封装实现
    作者      ：zhaojunxiang
    版权      ：<Copyright(C) 2003-2020 Suzhou Keda Technology Co., Ltd. All rights reserved.>
    -------------------------------------------------------------------------------------------
    修改记录：
    日期               版本              修改人             走读人              修改记录
    2019/12/18         V1.0        zhaojunxiang                         新建文件
    =========================================================================================*/
#ifndef _CONNECT_BASE_H
#define _CONNECT_BASE_H

#include "iconnectbase.h"//连接库 tcp、udp、com、tcpserver
#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#include "httpclient.h" //连接 httpclient

#include "http_server.h"//连接 http server

#include "libcurlPack.h"//https

//连接的协议类型
enum emuCBProtocolType
{
    emuCB_PROTO_Tcp = 0,	         //TCP连接 ,TKDProtocolTCP有效
    emuCB_PROTO_Udp,	                //UDP连接
    emuCB_PROTO_Com,	                //串口连接
    emuCB_PROTO_TCPSERVER,      //作为TCP 服务器监听设备
    emuCB_PROTO_HTTPClient,      //HTTPClient
    emuCB_PROTO_HTTPServer,     //HTTPServer端    
    emuCB_PROTO_HTTPS,             //HTTPS
};

//协议配置信息
typedef struct tagHTTPServerCfg
{
    tagHTTPServerCfg(){m_strListenUrl ="";m_wListenPort = 0;}
    
    std::string m_strListenUrl;//监听url
    u16 m_wListenPort;//本地端口

}HTTPServerCfg, *PtHTTPServerCfg;
#ifndef WIN32
__attribute__ ((packed))
#endif

//协议配置信息
typedef struct tagKDICONCfg
{
    tagKDICONCfg(){memset(this, 0, sizeof(tagKDICONCfg));}

    s8  m_szPeerIp[17];   //远端ip 一般为第三方服务器ip
    s8  m_szLocalIp[17]; //本地IP
    u16 m_wPeerPort;//远端目的端口
    u16 m_wLocalPort;//本地端口
    TICBCOMCfg m_tICBCOMCfg; //COM口配置
    HTTPServerCfg m_tICBHTTPServerCfg; //HTTP client 配置
}TKDICONCfg, *PtKDICONCfg;
#ifndef WIN32
__attribute__ ((packed))
#endif

//使用说明
/*
1.tcp/udp/com/tcpserver使用顺序

m_pConnectBase = new ConnectBase();

void * ptrConnet = m_pConnectBase->NewConnectBase(emuCB_PROTO_Com);

//    static void TCPCBState(u32 dwObjMark, EMICBDevConnState eDevState,void *pContext);
//    static void TCPCBData(u32 dwObjMark, const u8* data, s32 len, void *pContext);   
m_pConnectBase->SetDataCallBack(TCPCBState ,TCPCBData,this);

m_pConnectBase->StartConnectDev(m_pConnectBase->m_ptrIConnectBase, (void*)&pTKDICONCfg);

m_pConnectBase->StopConnectDev(m_pConnectBase->m_ptrIConnectBase);
m_pConnectBase->DelConnectBase(m_pConnectBase->m_ptrIConnectBase);

2.httpclient 使用
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

3.httpserver使用
m_pConnectBase = new ConnectBase();
void * ptrConnet = m_pConnectBase->NewConnectBase(emuCB_PROTO_HTTPServer);

TKDICONCfg pTKDICONCfg; 

pTKDICONCfg.m_tICBHTTPServerCfg.m_strListenUrl = "/httplisten"; //注意 斜杠 "/" 否则可能解析不出来
pTKDICONCfg.m_tICBHTTPServerCfg.m_wListenPort= 9900;

printf("ConnectAccessObject httplisten address: %p void: %p\n", HttpListen, (void*)&HttpListen);

m_pConnectBase->StartConnectDev(m_pConnectBase->m_ptrIConnectBase, (void*)&pTKDICONCfg);

//bool HttpListen(std::string strUrl, std::string strBody, mg_connection *c, OnRspCallback ListenBack);
m_pConnectBase->SetDataCallBack(HttpListen);

m_pConnectBase->StartConnectDev(m_pConnectBase->m_ptrIConnectBase, (void*)&pTKDICONCfg);

m_pConnectBase->StopConnectDev(m_pConnectBase->m_ptrIConnectBase);
m_pConnectBase->DelConnectBase(m_pConnectBase->m_ptrIConnectBase);

3.https使用
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
    //供外部调用
public:
    ConnectBase();
    ~ConnectBase();

    /************************************************************************/
    /* 创建连接                                                  */
    /* connectType:连接类型                              */
    /************************************************************************/
    void *NewConnectBase(emuCBProtocolType connectType);

    /************************************************************************/
    /* 开始连接                                                  */
    /* connectType:连接类型                              */
    /* cbState：为状态回调函数   
                        tcp/udp/com/tcpserver  回调 类型 pfICBConnectFunc*/
    /* cbData：为数据信息回调函数                           、
                        tcp/udp/com/tcpserver  回调 类型 pfICBTranslateFunc*/
    /* pContext为设置参数，回调函数传出           */
    /************************************************************************/
    BOOL32 SetDataCallBack(void*cbState = NULL, void*cbData = NULL,void *pContext=NULL);

    /************************************************************************/
    /* 设置断链重连         可能要拿出去 再议                                          */
    /************************************************************************/
    BOOL32 SetAliveChek();
    /************************************************************************
    /* 开始连接                                                  */
    /* connectType:连接类型                              */
    /* cb：为tcp、udp、com、tcpserver数据信息回调函数                           */
    /* pContext为设置参数，回调函数传出           */
    /************************************************************************/
    BOOL32 StartConnectDev(void* policy, const void* tkdicfg);
        /************************************************************************
    /* 断开连接                                                  */
    /* connectType:连接类型                              */
    /************************************************************************/
    BOOL32 StopConnectDev(void *policy);
  /************************************************************************
    /* 销毁创建的链接实例                                                  */
    /* connectType:连接类型                              */
    /************************************************************************/
    BOOL32 DelConnectBase(void *policy);
    /************************************************************************/
    /* 向连接设备发送数据的接口(异步)                                       */
    /* dwObjMark:为使用该连接对象的标识，与BindDeviceInfo接口中传递值相同   */
    /* pszData:数据值指针                                                   */
    /* nLength:数据长度                                                     */
    /* dwTimeOut:数据传递的超时时间                                         */ 
    /* 返回值参考 EMICBErrcode                                              */
    /************************************************************************/
    u32 PostAsynData(void *policy, const u8* pszData, s32 nLength, u32 dwTimeOut = 0);
    /************************************************************************/
    /* 向连接设备发送数据的接口(同步)                                       */
    /* dwObjMark:为使用该连接对象的标识，与BindDeviceInfo接口中传递值相同   */
    /* pszData:数据值指针                                                   */
    /* nLength:数据长度                                                     */
    /* dwTimeOut:数据传递的超时时间                                         */ 
    /* 对于udp连接无效                                                       */
    /* 返回值参考 EMICBErrcode                                              */
    /************************************************************************/
    u32 PostSynData(void *policy, const u8* pszData, s32 nLength, u32 dwTimeOut = 0);

    //http client 支持自定义请求头
    Response& HttpReqSync(/*OUT*/ Response& response, const std:: string &content, const char* url,
        emMethod method= E_GET_METHOD,int nTimeout = 5000,std::map<std::string,std::string> header = std::map<std::string,std::string>());

    //https 支持自定义头
    std::string HttpsReqSync(const std:: string content, const std:: string url,
        HTTPSmethods method= HTTPS_POST,int nTimeout = 5000,std::map<std::string,std::string> header = std::map<std::string,std::string>());

//内部使用    
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