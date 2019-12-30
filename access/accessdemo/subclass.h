#include "AccessBasePolicy.h"

//дlogʾ����·��D�̣�speakģ��
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
     u32 SendDevState( BOOL32 bObjectState);//���÷���״̬

public:
    
    PKDISpeakHostDataCB m_cbData;
    void *m_pDataContext;//�豸����������ָ��
    PKDISpeakHostInfoCB m_cbInfo;
    void *m_pInfoContext;//�豸info������ָ��

};
//////////////////////////////////////////////////////////////////////////
//common
static bool AliveCheckCB(void* pData); //��� �Ƿ����ߣ��������false���Զ�����AliveLoginCB��������
static bool AliveLoginCB(void* pData); //������������thisָ�룬��Ҫ����open����connect�Ĳ���
bool HttpListen(std::string strUrl, std::string strBody, mg_connection *c, OnRspCallback ListenBack); //httplisten
//////////////////////////////////////////////////////////////////////////