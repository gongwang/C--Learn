#include "subclass.h"

using namespace std;

subclass::subclass(std::string strDllname):AccessBasePolicy(strDllname)
{
    ////////////////////////////////公共模块//////////////////////////////////////////
    //生成dump信息
    CCreateDump::Instance()->DeclarDumpFile("access_dumpfile"); //参数为 dump文件名称
    //设置断链检测回调
    SetAliveCB(AliveCheckCB, AliveLoginCB, this, 30*1000);
    //////////////////////////////////////////////////////////////////////////
}

subclass::~subclass(void)
{
    printf("hello end\n");
}

//HTTPS 测试
u32 subclass::ConnectAccessObject(TKDIConnInfo* ptConnInfo, EMKDIProtocolType eProtocolType, PKDIPARAM pProtocolCfg, u32 dwCfgSize, void* pExtInfo)
{
    //TODO:  
    //////////////////////////////////////////////////////////////////////////
    //登陆

    m_pConnectBase = new ConnectBase();

    void * ptrConnet = m_pConnectBase->NewConnectBase(emuCB_PROTO_HTTPS);

    //m_pConnectBase->SetDataCallBack(ConnectCBState,ConnectCBData,this);

    string url = "https://172.16.130.92:24408/syris/device/topology";
    string header = "";
    HTTPSmethods method = HTTPSmethods::HTTPS_GET;
    string content = "";

    //std::string recv = m_pConnectBase->HttpsReqSync(content, url, HTTPS_POST);

    LogPrintf(m_tConnInfo.m_szObjectName, "%s\n", "ConnectAccessObject");

    return GSS_SUCCESS;
}

u32 subclass::DisConnectAccessObject()
{
    //退出登录
     m_pConnectBase->StopConnectDev(m_pConnectBase->m_ptrIConnectBase);
     m_pConnectBase->DelConnectBase(m_pConnectBase->m_ptrIConnectBase);
	return GSS_SUCCESS;

}
void subclass::SetTransParentDataCB(PKDITPDataCB pTPDataCB, void *pContext)
{
    m_cbTPData = pTPDataCB;
    m_pcbTPDataContext = pContext;
	return;
}
void subclass::TransParentData(s8* pszFiled, u16 wMsgNo, u32 dwSn, u8 byDataFormat, void* pTransData, s32 dwDataLen, void *pContext)
{
	return;
}
s8* subclass::GetLastError()
{
	return "";
}
u32 subclass::GetLastErrNo()
{
	return GSS_SUCCESS;
}
u32 subclass::SubAcessObjectStat(EMKDISubType emSubType, PKDIAcessObjectStatCB pStatCB, void *pContext)
{
    m_cbStat = pStatCB;
    m_pStateContext = pContext;
    return GSS_SUCCESS;
}

u32 subclass::SubAcessObjectCapSet(PKDIAcessObjectCapSetCB pCapSetCB, void *pContext)
{
    return GSS_SUCCESS;
}

//speak 独有
u32 subclass::SubSpeakHostInfo(EMKDISubType emSubType, PKDISpeakHostInfoCB pInfoCB, void *pContext)
{
	return GSS_SUCCESS;
}
u32 subclass::SubSpeakHostData(EMKDISubType emSubType, PKDISpeakHostDataCB pDataCB, void *pContext)
{
    //TODO:
    m_cbData = pDataCB;
    m_pDataContext = pContext;
	return GSS_SUCCESS;
}

u32 subclass::SendDevState( BOOL32 bObjectState )
{
    if(!m_cbStat)
    {
        return GSS_ERROR_DLL_DEV_HANDLE_NULL_ERROR;
    }

    TKDIAcessObjectStat tKDIAcessObjectStat;
    memcpy(tKDIAcessObjectStat.m_szObjectNo, m_tConnInfo.m_szObjectNo, strlen(m_tConnInfo.m_szObjectNo));
    memcpy(tKDIAcessObjectStat.m_szObjectName, m_tConnInfo.m_szObjectName, strlen(m_tConnInfo.m_szObjectName));

    if(bObjectState) tKDIAcessObjectStat.m_emState = TKDIAcessObjectStat::EONLINE;
    else tKDIAcessObjectStat.m_emState = TKDIAcessObjectStat::EOFFLINE;

    m_cbStat((KDIHANDLE)this, &tKDIAcessObjectStat, m_pStateContext);

    return GSS_SUCCESS;
}

void subclass::ConnectCBData( u32 dwObjMark, const u8* data, s32 len, void *pContext )
{
    printf("subclass::TCPCBData\n");

    s8 buf[1024] = {0};
    sprintf(buf, "Length%d\n", len);
    printf(buf);
    
    if (len > 65536)
    {
        printf("lenth is erroro\n");
        return;
    }

    subclass *pPolicy = (subclass *)pContext;
    if (pPolicy == NULL)
    {
        printf("pPolicy is null \n");
        return;
    }else
    {
        printf("Recv: %s\n",data);

        u8 cSendCon[32] = {0};
        cSendCon[0] = '1';
        cSendCon[1] = '2';
        cSendCon[2] = '3';
        
        printf("PostSynData this: %p m_pConnectBase:%p m_ptrIConnectBase:%p\n", pPolicy, pPolicy->m_pConnectBase, pPolicy->m_pConnectBase->m_ptrIConnectBase);

        //同步接口
        pPolicy->m_pConnectBase->PostSynData((void*)pPolicy->m_pConnectBase, (u8*)cSendCon, 3);
        //异步接口
        pPolicy->m_pConnectBase->PostAsynData((void*)pPolicy->m_pConnectBase, (u8*)cSendCon, 3);
    }
}

void subclass::ConnectCBState( u32 dwObjMark, EMICBDevConnState eDevState,void *pContext )
{
    printf("subclass::TCPCBState\n");

    subclass *pPolicy = (subclass *)pContext;
    if (pPolicy == NULL)
    {
        printf("pPolicy is null \n");
        return;
    }

    printf("设备连接状态:%d\n", eDevState);

}

bool HttpListen( std::string strUrl, std::string strBody, mg_connection *c, OnRspCallback ListenBack )
{
    printf("HttpListen Recv:%s\n", strBody.c_str());
    return true;
}

bool AliveCheckCB(void* pData)//检查 是否离线，如果返回false，自动调用AliveLoginCB进行重连
{
    printf("AliveCheckCB ...\n");
    return false;
}
bool AliveLoginCB(void* pData) //参数基本都是this指针，需要保存open或者connect的参数
{
    return false;
}
