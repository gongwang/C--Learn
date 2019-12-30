#include "AccessBasePolicy.h"

//写log示例，路径D盘，speak模块
#define LOG_MODULE_NAME     "ACCESS101"
//LogPrintf(LOG_MODULE_NAME, "%s", "ACCESS102");

class subclass:public AccessBasePolicy
{
public:
    subclass(std::string strDllname);
    virtual ~subclass(void);

    //////////////////////////////////////////////////////////////////////////
     u32 SubAcessObjectStat(EMKDISubType emSubType, PKDIAcessObjectStatCB pStatCB, void *pContext);
     u32 SubAcessObjectCapSet(PKDIAcessObjectCapSetCB pCapSetCB, void *pContext);
     void SetTransParentDataCB(PKDITPDataCB pTPDataCB, void *pContext);
     void TransParentData(s8* pszFiled, u16 wMsgNo, u32 dwSn, u8 byDataFormat, void* pTransData, s32 dwDataLen, void *pContext) ;
     s8* GetLastError();
     u32 GetLastErrNo();
     u32 ConnectAccessObject(TKDIConnInfo* ptConnInfo, EMKDIProtocolType eProtocolType, PKDIPARAM pProtocolCfg, u32 dwCfgSize, void* pExtInfo);
     u32 DisConnectAccessObject();
    //////////////////////////////////////////////////////////////////////////

     //tcp/udp/tcpserver
     void ConnectCBState(u32 dwObjMark, EMICBDevConnState eDevState,void *pContext);
     void ConnectCBData(u32 dwObjMark, const u8* data, s32 len, void *pContext);  

    //speak
     u32 SubSpeakHostInfo(EMKDISubType emSubType, PKDISpeakHostInfoCB pInfoCB, void *pContext);
     u32 SubSpeakHostData(EMKDISubType emSubType, PKDISpeakHostDataCB pDataCB, void *pContext);
     u32 SendDevState( BOOL32 bObjectState);//设置发送状态

public:
    
    PKDISpeakHostDataCB m_cbData;
    void *m_pDataContext;//设备数据上下文指针
    PKDISpeakHostInfoCB m_cbInfo;
    void *m_pInfoContext;//设备info上下文指针

};
//////////////////////////////////////////////////////////////////////////
//common
static bool AliveCheckCB(void* pData); //检查 是否离线，如果返回false，自动调用AliveLoginCB进行重连
static bool AliveLoginCB(void* pData); //参数基本都是this指针，需要保存open或者connect的参数
bool HttpListen(std::string strUrl, std::string strBody, mg_connection *c, OnRspCallback ListenBack); //httplisten
//////////////////////////////////////////////////////////////////////////