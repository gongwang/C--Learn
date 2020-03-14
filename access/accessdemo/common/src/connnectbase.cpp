#include "connectbase.h"

ConnectBase::ConnectBase()
{
    m_ptrIConnectBase = NULL;

    m_ptrCBstate = NULL;
    m_ptrCBData = NULL;
    m_ptrContext = NULL;

    s_pHttpServer = NULL;

}

ConnectBase::~ConnectBase()
{
    m_ptrIConnectBase = NULL;
}

void * ConnectBase::NewConnectBase( emuCBProtocolType connectType )
{
    SetConnectType(connectType); //设置连接类型

    printf("NewConnectBase Protocol is %d\n",connectType);

    if (m_ptrIConnectBase != NULL)
    {
        printf("ConnectObject had created, return now.\n");
        return NULL;
    }

    switch (connectType)
    {
    case emuCB_PROTO_Tcp:         //TCP连接 ,TKDProtocolTCP有效
        m_ptrIConnectBase = NewConnectObject(EMICB_PROTO_Tcp);
        break;
    case emuCB_PROTO_Udp:                //UDP连接
        m_ptrIConnectBase = NewConnectObject(EMICB_PROTO_Udp);
        break;
    case emuCB_PROTO_Com:                //串口连接
        m_ptrIConnectBase = NewConnectObject(EMICB_PROTO_Com);
        break;
    case emuCB_PROTO_TCPSERVER:      //作为TCP 服务器监听设备
        m_ptrIConnectBase = NewConnectObject(EMICB_PROTO_TCPSERVER);
        break;
    case emuCB_PROTO_HTTPClient:     //HTTPClient 短连接 返回this
        m_ptrIConnectBase = this;
        break;
    case emuCB_PROTO_HTTPServer:   //HTTPServer端 
        {
            if(s_pHttpServer == NULL)
            {
                s_pHttpServer = new HttpServer();
            }
        }
        m_ptrIConnectBase = s_pHttpServer;

        break;
    case emuCB_PROTO_HTTPS:           //HTTPS 短连接 返回this
        m_ptrIConnectBase = this;
        break;
    }

    return m_ptrIConnectBase;
}

BOOL32 ConnectBase::SetConnectType( emuCBProtocolType connectType )
{ 
    m_emuConnectBase = connectType;
    printf("SetConnectType Protocol is %d\n",m_emuConnectBase);
    return TRUE;
}

emuCBProtocolType ConnectBase::GetConnectType()
{
    printf("GetConnectType Protocol is %d\n",m_emuConnectBase);
    return m_emuConnectBase;
}

BOOL32 ConnectBase::SetDataCallBack(void*cbState /*= NULL*/, void*cbData /*= NULL*/,void *pContext/*=NULL*/ )
{
    BOOL32 bRet = FALSE;

    emuCBProtocolType emuType = GetConnectType();

    switch (emuType)
    {
    case emuCB_PROTO_Tcp:         //TCP连接 ,TKDProtocolTCP有效
    case emuCB_PROTO_Udp:                //UDP连接
    case emuCB_PROTO_Com:                //串口连接
    case emuCB_PROTO_TCPSERVER:      //作为TCP 服务器监听设备
    case emuCB_PROTO_HTTPServer:
        {
            m_ptrCBstate = cbState;
            bRet = TRUE;
            break;
        }
    case emuCB_PROTO_HTTPClient:     //HTTPClient 短连接 返回this
        break;
    case emuCB_PROTO_HTTPS:           //HTTPS 短连接 返回this
        break;
    }
    return bRet;
}

BOOL32 ConnectBase::StartConnectDev( void* policy, const void* tkdicfg )
{
    BOOL32 bRet = FALSE;

    if (policy == NULL)
    {
        printf("StartConnectDev input param is null, error return.\n");
        return bRet;
    }

    emuCBProtocolType emuType = GetConnectType();
    printf("StartConnectDev Protocol is %d\n",emuType);
    switch (emuType)
    {
    case emuCB_PROTO_Tcp:         //TCP连接 ,TKDProtocolTCP有效
    case emuCB_PROTO_Udp:                //UDP连接
    case emuCB_PROTO_TCPSERVER:      //作为TCP 服务器监听设备
        {
            IConnectBase* pConnect = (IConnectBase*)policy;
            TKDICONCfg* pConfig = (TKDICONCfg*)tkdicfg;
            memcpy(&m_pConfig, tkdicfg, sizeof(TKDICONCfg)); //赋值保存连接参数
            //TODO:         
            TICBAddress tLoaclCfg;
            tLoaclCfg.m_dwIp = inet_addr(pConfig->m_szLocalIp);
            tLoaclCfg.m_wPort = pConfig->m_wLocalPort;
            pConnect->SetLocalCfg(&tLoaclCfg,sizeof(tLoaclCfg));
            TICBAddress tDevInfo;
            tDevInfo.m_dwIp = inet_addr(pConfig->m_szPeerIp);
            tDevInfo.m_wPort = pConfig->m_wPeerPort;

            printf("StartConnectDev peerIp: %s %d port:%d\n", pConfig->m_szPeerIp, tDevInfo.m_dwIp);

            if(pConnect->BindDeviceInfo((u32)this,&tDevInfo) != EMICB_OK)
            {
                printf("已存在相同设备\n");
                pConnect->UnbindDeviceInfo((u32)this,&tDevInfo);
                pConnect->BindDeviceInfo((u32)this,&tDevInfo); //解绑 再绑定
            }

            pConnect->SetConnectStateCB((pfICBConnectFunc)m_ptrCBstate, m_ptrContext);
            pConnect->SetTranslateCB((pfICBTranslateFunc)m_ptrCBData, m_ptrContext);

            printf("obj = %p\n",(u32)this);
            if (EMICB_OK == pConnect->StartConnect())
            {
                printf("open connect success\n");
                bRet = TRUE;
            }

            break;
        }
    case emuCB_PROTO_Com:                //串口连接
        {   
            IConnectBase* pConnect = (IConnectBase*)policy;
            TKDICONCfg* pConfig = (TKDICONCfg*)tkdicfg;
            memcpy(&m_pConfig, tkdicfg, sizeof(TKDICONCfg)); //赋值保存连接参数

            pConnect->SetLocalCfg(&m_pConfig.m_tICBCOMCfg,sizeof(TICBCOMCfg));

            printf("StartConnectDev port:%d Address:%d baudrate:%d bits:%d parity:%d stopbits:%d\n", m_pConfig.m_tICBCOMCfg.m_dwComport,
                m_pConfig.m_tICBCOMCfg.m_dwAddress,
                m_pConfig.m_tICBCOMCfg.m_emBaudrate,
                m_pConfig.m_tICBCOMCfg.m_emDatabits,
                m_pConfig.m_tICBCOMCfg.m_emParity,
                m_pConfig.m_tICBCOMCfg.m_emStopbits); 

            if(pConnect->BindDeviceInfo((u32)this) != EMICB_OK)
            {
                printf("已存在相同设备\n");
                pConnect->UnbindDeviceInfo((u32)this);
                pConnect->BindDeviceInfo((u32)this); //解绑 再绑定
            }

            pConnect->SetConnectStateCB((pfICBConnectFunc)m_ptrCBstate, m_ptrContext);
            pConnect->SetTranslateCB((pfICBTranslateFunc)m_ptrCBData, m_ptrContext);

            printf("obj = %p\n",(u32)this);
            if (EMICB_OK == pConnect->StartConnect())
            {
                printf("open connect success\n");
                bRet = TRUE;
            }
            break;
        }          
    case emuCB_PROTO_HTTPClient:     //HTTPClient 短连接 返回this
        break;
    case emuCB_PROTO_HTTPServer:   //HTTPServer端  
        {
            TKDICONCfg* pConfig = (TKDICONCfg*)tkdicfg;

            //如果url和端口一致，则不进行再次开线程监听，一直用同一个；否则如果多次开线程会崩溃
            if (m_pConfig.m_tICBHTTPServerCfg.m_strListenUrl == pConfig->m_tICBHTTPServerCfg.m_strListenUrl &&
                m_pConfig.m_tICBHTTPServerCfg.m_wListenPort == pConfig->m_tICBHTTPServerCfg.m_wListenPort)
            {
                printf("http server config is same, break.\n");
                break;
            }

            memcpy(&m_pConfig, tkdicfg, sizeof(TKDICONCfg)); //赋值保存连接参数

            if (!s_pHttpServer)
            {
                printf("http server is not init,return!\n");
                break;
            }
            char cLocalPort[32] = {0};
            sprintf(cLocalPort, "%d", m_pConfig.m_tICBHTTPServerCfg.m_wListenPort);

            s_pHttpServer->Init(cLocalPort);

            HANDLE hHandleStartHttp = NULL; //如果登陆不成功 不开监听
            //if (hHandleStartHttp == NULL)
            {
                hHandleStartHttp = CreateThread(NULL, 0, StartHttpThread, (void*)this, 0, NULL);
                CloseHandle(hHandleStartHttp);
            }

            printf("httplisten address: %p\n", m_ptrCBstate);

            if(m_ptrCBstate)s_pHttpServer->AddHandler(m_pConfig.m_tICBHTTPServerCfg.m_strListenUrl, (ReqHandler)m_ptrCBstate);

            printf("http server listen port: %d url: %s\n", m_pConfig.m_tICBHTTPServerCfg.m_wListenPort,m_pConfig.m_tICBHTTPServerCfg.m_strListenUrl.c_str());
        }

        break;
    case emuCB_PROTO_HTTPS:           //HTTPS 短连接 返回this
        break;
    }

    return bRet;
}

BOOL32 ConnectBase::StopConnectDev( void *policy )
{
    BOOL32 bRet = FALSE;

    if (policy == NULL)
    {
        printf("StopConnectDev input param is null, error return.\n");
        return FALSE;
    }

    emuCBProtocolType emuType = GetConnectType();

    switch (emuType)
    {
    case emuCB_PROTO_Tcp:         //TCP连接 ,TKDProtocolTCP有效
    case emuCB_PROTO_Udp:                //UDP连接
    case emuCB_PROTO_Com:                //串口连接
    case emuCB_PROTO_TCPSERVER:      //作为TCP 服务器监听设备
        {
            IConnectBase* pConnect = (IConnectBase*)policy;
            //TODO:
            TICBAddress tDevInfo;
            tDevInfo.m_dwIp = inet_addr(m_pConfig.m_szPeerIp);
            tDevInfo.m_wPort = m_pConfig.m_wPeerPort;
            pConnect->UnbindDeviceInfo((u32)this,&tDevInfo);
            u32 dwRet = pConnect->StopConnect();
            if (dwRet == EMICB_OK)
            {
                printf("断开连接成功 ip:%s port:%d\n", m_pConfig.m_szPeerIp, m_pConfig.m_wPeerPort);
                bRet = TRUE;
            }
            break;
        }
    case emuCB_PROTO_HTTPClient:     //HTTPClient 短连接 返回this
        break;
    case emuCB_PROTO_HTTPServer:   //HTTPServer端  
        {
            if (s_pHttpServer)
            {
                s_pHttpServer->RemoveHandler(m_pConfig.m_tICBHTTPServerCfg.m_strListenUrl);
            }

            break;
        }
    case emuCB_PROTO_HTTPS:           //HTTPS 短连接 返回this
        break;
    }

    return bRet;
}

BOOL32 ConnectBase::DelConnectBase( void *policy )
{
    BOOL32 bRet = FALSE;

    if (policy == NULL)
    {
        printf("StopConnectDev input param is null, error return.\n");
        return FALSE;
    }

    emuCBProtocolType emuType = GetConnectType();

    switch (emuType)
    {
    case emuCB_PROTO_Tcp:         //TCP连接 ,TKDProtocolTCP有效
    case emuCB_PROTO_Udp:                //UDP连接
    case emuCB_PROTO_Com:                //串口连接
    case emuCB_PROTO_TCPSERVER:      //作为TCP 服务器监听设备
        {
            IConnectBase* pConnect = (IConnectBase*)policy;
            //TODO:
            printf("%p\n",pConnect);
            printf("断开%s port: %d\n", m_pConfig.m_szPeerIp, m_pConfig.m_wPeerPort);
            DelConnectObject(pConnect);
            bRet = TRUE;
            break;
        }
    case emuCB_PROTO_HTTPClient:     //HTTPClient 短连接 返回this
        break;
    case emuCB_PROTO_HTTPServer:   //HTTPServer端  

        //TODO

        break;
    case emuCB_PROTO_HTTPS:           //HTTPS 短连接 返回this
        break;
    }

    return bRet;
}

u32 ConnectBase::PostAsynData( void *policy, const u8* pszData, s32 nLength, u32 dwTimeOut /*= 0*/ )
{
    BOOL32 bRet = FALSE;

    if (policy == NULL)
    {
        printf("PostAsynData input param is null, error return.\n");
        return FALSE;
    }

    emuCBProtocolType emuType = GetConnectType();

    switch (emuType)
    {
    case emuCB_PROTO_Tcp:         //TCP连接 ,TKDProtocolTCP有效
    case emuCB_PROTO_Com:                //串口连接
    case emuCB_PROTO_Udp:       //UDP
    case emuCB_PROTO_TCPSERVER:      //作为TCP 服务器监听设备
        {
            ConnectBase* pConnect = (ConnectBase*)policy;
            IConnectBase* pIConnect = ( IConnectBase*)pConnect->m_ptrIConnectBase;
            //TODO:
            printf("SendAsynData:Len:%d\n%s\n", nLength, pszData);
            
            pIConnect->PostData((u32)this, pszData, nLength, dwTimeOut);

            bRet = TRUE;
            break;
        }
    case emuCB_PROTO_HTTPClient:     //HTTPClient 短连接 返回this
        break;
    case emuCB_PROTO_HTTPServer:   //HTTPServer端  

        //TODO

        break;
    case emuCB_PROTO_HTTPS:           //HTTPS 短连接 返回this
        break;
    }

    return bRet;
}

u32 ConnectBase::PostSynData( void *policy, const u8* pszData, s32 nLength, u32 dwTimeOut /*= 0*/ )
{
    BOOL32 bRet = FALSE;

    if (policy == NULL)
    {
        printf("PostAsynData input param is null, error return.\n");
        return FALSE;
    }

    emuCBProtocolType emuType = GetConnectType();

    printf("PostSynData Protocol is %d\n",emuType);

    switch (emuType)
    {
    case emuCB_PROTO_Tcp:         //TCP连接 ,TKDProtocolTCP有效
    case emuCB_PROTO_Com:                //串口连接
    case emuCB_PROTO_TCPSERVER:      //作为TCP 服务器监听设备
        {
            ConnectBase* pConnect = (ConnectBase*)policy;
            IConnectBase* pIConnect = ( IConnectBase*)pConnect->m_ptrIConnectBase;
            //TODO:
            printf("SendSynData:Len:%d\n%s\n", nLength, pszData);

            pIConnect->SendData((u32)this, pszData, nLength, dwTimeOut);

            bRet = TRUE;
            break;
        }
   case emuCB_PROTO_Udp:                //UDP连接
            break;
    case emuCB_PROTO_HTTPClient:     //HTTPClient 短连接 返回this
        break;
    case emuCB_PROTO_HTTPServer:   //HTTPServer端  

        //TODO

        break;
    case emuCB_PROTO_HTTPS:           //HTTPS 短连接 返回this
        break;
    }

    return bRet;
}

Response& ConnectBase::HttpReqSync( /*OUT*/ Response& response, const std:: string &content, const char* url,emMethod method/*= E_GET_METHOD*/,int nTimeout /*= 5000*/,std::map<std::string,std::string> header /*= std::map<std::string,std::string>()*/ )
{
    //直接调用
    return HttpRequestSync(response, content, url, method, nTimeout,header);
}


DWORD WINAPI StartHttpThread(LPVOID lpThreadParameter)
{
    ConnectBase* pConnectBase = ( ConnectBase* )lpThreadParameter;
    while(TRUE)
    {
        pConnectBase->s_pHttpServer->Start();
    }
}

std::string ConnectBase::HttpsReqSync(const std:: string content, const std:: string url, HTTPSmethods method/*= HTTPS_POST*/,int nTimeout /*= 5000*/,std::map<std::string,std::string> header /*= std::map<std::string,std::string>()*/ )
{
    std::string headers = "";
    std::string strrecv = "";
    //设置请求头
    if (!header.empty())//使用默认请求头
    {  
        std::map<std::string,std::string>::iterator itr;
        for (itr = header.begin();itr != header.end();++itr)
        {
            headers.append(itr->first);
            headers.append(itr->second);
            headers.append("\r\n");
        }
        headers.append("\r\n");
    }

    TData tdata(url, headers, content);

    memcpy(tdata.pUrl, url.c_str(), url.length() + 1);
    memcpy(tdata.header, headers.c_str(), headers.length() + 1);
    memcpy(tdata.content, content.c_str(), content.length() + 1);

    HttpsReqAndRes(method, tdata);

    printf("call back : %s  \n", tdata.pRcv);

    strrecv = tdata.pRcv;

    return strrecv;
}
