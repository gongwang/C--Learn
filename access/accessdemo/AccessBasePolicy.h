#pragma once

//#include "osp/osp.h"
#include "ospext/ospext.h"

#include "kdi_sdk_speak.h"
#include "kdi_sdk_alarm.h"
#include "kdi_sdk_bayonet.h"
#include "kdi_sdk_door.h"
#include "kdi_sdk_patrol.h"
#include "kdi_sdk_sensor.h"
#include "kdi_sdk_sms.h"
#include "kdi_sdk_swall.h"

#include "gsserrcode.h"
#include "windows.h"
#include <string>

#include "accessproto.h" //入口

typedef bool (*BaseAliveCheckCB)(void* pData); //检查 是否离线，如果返回false，自动调用AliveLoginCB进行重连
typedef bool (*BaseAliveLoginCB)(void* pData); //参数基本都是this指针，需要保存open或者connect的参数

class AccessBasePolicy
{
public:
    AccessBasePolicy(void){};
    AccessBasePolicy(std::string strDllname){
        m_pAliveCheckCB = NULL;//自动检测
        m_pAliveLoginCB= NULL;//自动登录
        m_pAliveContext= NULL;//数据上下文
        m_nAliveCheckTimer = 30*1000;
        m_dwAliveThreadID = NULL;
        
        initOspServer(strDllname); //初始化telnet端口

        InitAliveProccess();//初始化自动断链库
        
        insertObject2Vector(this);

        //获取模块名称 初始化日志库
        static BOOL g_bInitModule = FALSE;

        if (!g_bInitModule)
        {
            s8 szExePath[MAX_PATH + 1]={0};
            s8 szExeName[MAX_PATH + 1]={0};
            GetModuleFileName(NULL, szExePath, MAX_PATH); // c://ismp//speakadater.exe
            //strrchr  从末尾第一次出现指定字符位置的指针;  strchr 在参数str所指向的字符串中搜索第一次出现字符c（一个无符号字符）的位置。
            char* pEnd = strrchr(szExePath, '\\');      
            strncpy(szExeName, pEnd+1, strlen(pEnd) -4 -1); //speakadater
            printf("exe: %s module name:%s\n", szExePath, szExeName);

            char szFilePath_coef[MAX_PATH + 1] = { 0 };
            strcat(szFilePath_coef, "D:\\");
            strcat(szFilePath_coef, szExeName);
            strcat(szFilePath_coef, "\\");
            InitLog(szFilePath_coef, 1024*1024*10, 20); //20*10MB
            LogPrintf("BASE","创建的日志目录： %s \n", szFilePath_coef);
            g_bInitModule = TRUE;
        }

    };
    virtual ~ AccessBasePolicy(void)
    {
        //UnInitLog(); //log为实时写入，即使异常崩溃，不关闭文件也不会有问题，暂时不调用反初始化（类析构不代表 进程所有线程NG）
        deleteObject2Vector(this);
    };

    //common 不许更改
    u32 Open(TKDIConnInfo* ptConnInfo, EMKDIProtocolType eProtocolType, PKDIPARAM pProtocolCfg, u32 dwCfgSize, void* pExtInfo)
    {
        //保存配置信息
        memcpy(&m_tConnInfo, ptConnInfo, sizeof(TKDIConnInfo));
        memcpy(&m_tKDITCPCfg, pProtocolCfg, sizeof(TKDITCPCfg));
        memcpy(&m_tKDIUDPCfg, pProtocolCfg, sizeof(TKDIUDPCfg));
        memcpy(&m_tKDITCPServerCfg, pProtocolCfg, sizeof(TKDITCPServerCfg));
        memcpy(&m_tKDICOMCfg, pProtocolCfg, sizeof(TKDICOMCfg));
        m_eProtocolType = eProtocolType;

        u32 dwRet = ConnectAccessObject(ptConnInfo, eProtocolType, pProtocolCfg, dwCfgSize, pExtInfo);
        if(m_dwAliveThreadID == 0 && m_pAliveCheckCB != NULL && m_pAliveLoginCB != NULL && m_pAliveContext != NULL)
        {
            m_dwAliveThreadID = CreateCheckAliveProc(m_pAliveCheckCB, m_pAliveContext, m_pAliveLoginCB, m_pAliveContext, m_nAliveCheckTimer); //30s检查一次
        }
        return dwRet;
    };
    u32 Close()
    {
        if(m_dwAliveThreadID != 0)
        {
            DestroyCheckAliveProc(m_dwAliveThreadID);
            m_dwAliveThreadID = 0;
        }
        return DisConnectAccessObject();
    };

    //common 可以重写
    virtual void ConnectCBState(u32 dwObjMark, EMICBDevConnState eDevState,void *pContext){};
    virtual void ConnectCBData(u32 dwObjMark, const u8* data, s32 len, void *pContext){};  
    
    virtual void SetAliveCB(BaseAliveCheckCB pAliveCheckCB, BaseAliveLoginCB pAliveLoginCB, void* pContext, s32 nAliveTimer = 30*1000) //设置自动登录
    {
        m_pAliveCheckCB = pAliveCheckCB;
        m_pAliveLoginCB = pAliveLoginCB;
        m_pAliveContext = pContext;
        m_nAliveCheckTimer = nAliveTimer;
    }

    //common 必须重写
    virtual u32 SubAcessObjectStat(EMKDISubType emSubType, PKDIAcessObjectStatCB pStatCB, void *pContext) = 0;
    virtual u32 SubAcessObjectCapSet(PKDIAcessObjectCapSetCB pCapSetCB, void *pContext) = 0;
	virtual void SetTransParentDataCB(PKDITPDataCB pTPDataCB, void *pContext) = 0;
	virtual void TransParentData(s8* pszFiled, u16 wMsgNo, u32 dwSn, u8 byDataFormat, void* pTransData, s32 dwDataLen, void *pContext) = 0;
	virtual s8* GetLastError() = 0;
	virtual u32 GetLastErrNo() = 0;
 
    //speak
    virtual u32 SubAcessNodeStat(EMKDISubType emSubType, PKDIAcessNodeStatCB pStatCB, void *pContext){return NULL;}; //带子节点状态上报
    virtual u32 SubSpeakHostInfo(EMKDISubType emSubType, PKDISpeakHostInfoCB pInfoCB, void *pContext){return NULL;}; //暂时无用
    virtual u32 SubSpeakHostData(EMKDISubType emSubType, PKDISpeakHostDataCB pDataCB, void *pContext){return NULL;};//对讲数据上报
    //alarm
    virtual u32 SubAlarmHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIAlarmHostInfoCB pInfoCB, void *pContext){return NULL;};
    virtual u32 SubAlarmHostData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIAlarmHostDataCB pDataCB, void *pContext){return NULL;};
    virtual u32 SetAlarmControlCB(KDIHANDLE pKdiHandle, PKDIAlarmControlRspCB pControlRspCB){return NULL;};
    virtual u32 AlarmControl(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIAlarmControlReq *ptControlReq, void *pContext){return NULL;};
    //bayonet
    virtual u32  SubBayonetInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIBayonetInfoCB pInfoCB, void *pContext){return NULL;};
    virtual u32  SubBayonetData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIBayonetDataCB pDataCB, void *pContext){return NULL;};
    virtual u32  GetBayonetInfo(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIGetBayonetReq *ptGetBayonetReq, void *pContext){return NULL;};
    virtual u32  DealVehicleInfo(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIDealVehicleInfoReq *ptDealVehicleInfoReq, void *pContext){return NULL;};
    virtual u32  BatchDealVehicleInfo(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIBatchDealVehicleInfoReq *ptKDIBatchDealVehicleInfoReq, 
        TKDIBatchDealVehicleInfoRsp *ptKDIBatchDealVehicleInfoRsp, void *pContext){return NULL;};
    virtual u32  DealVehicleJurisdiction(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIDealVehicleJstReq *ptDealVehicleJstReq, void *pContext){return NULL;};
    virtual u32  ControlBayonet(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIControlBayonetReq *ptControlBayonetReq, void *pContext){return NULL;};
    //door
    KDI_SDK_API u32 KDICALL KDI_SubDoorHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIDoorHostInfoCB pInfoCB, void *pContext){return NULL;};
    KDI_SDK_API u32 KDICALL KDI_SubDoorHostData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIDoorHostDataCB pDataCB, void *pContext){return NULL;};
    KDI_SDK_API u32 KDICALL KDI_SubCreditCardInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDICreditCardCB pDataCB, void *pContext){return NULL;};
    KDI_SDK_API u32 KDICALL KDI_SetSyncCardInfoCB(KDIHANDLE pKdiHandle, PKDISyncCardInfoCB pDataCB){return NULL;};
    KDI_SDK_API void KDICALL KDI_SetDoorControlCB(KDIHANDLE pKdiHandle, PKDIDoorControlRspCB pControlRspCB, void *pContext){return;};
    KDI_SDK_API u32 KDICALL KDI_DoorControl(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIDoorControlReq *ptControlReq,  void *pContext){return NULL;};	
    KDI_SDK_API u32 KDICALL KDI_SyncCardInfoReq(KDIHANDLE pKdiHandle, u32 dwSn, void *pContext){return NULL;};	
    KDI_SDK_API u32 KDICALL KDI_AddDoorCard(KDIHANDLE pKdiHandle, const TKDICardList &tCardList, void *pContext){return NULL;};
    KDI_SDK_API u32 KDICALL KDI_DelDoorCard(KDIHANDLE pKdiHandle, const TKDICardList &tCardList, void *pContext){return NULL;};
    //patrol
    virtual u32 SubPatrolHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIPatrolHostInfoCB pInfoCB, void *pContext){return NULL;};
    virtual u32 SubPatrolHostData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIPatrolHostDataCB pDataCB, void *pContext){return NULL;};
    //sensor
    virtual u32 SubSensorInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDISensorInfoCB pInfoCB, void *pContext){return NULL;};
    virtual u32 SubSensortData(KDIHANDLE pKdiHandle, EMKDISubType emSubType,	PKDISensorDataCB pDataCB, void *pContext){return NULL;};
    //sms 
   virtual u32  SubSmsHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDISmsHostInfoCB pInfoCB, void *pContext){return NULL;};
   virtual u32  SetRecvSmsCB(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 	PKDIRecvSmsCB pDataCB, void *pContext){return NULL;};
   virtual void SendSmsInfo(KDIHANDLE pKdiHandle, TKDISmsInfo tSmsInfo){};
    //swall
   virtual u32  SubSwallHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 	PKDISwallHostInfoCB pInfoCB, void *pContext){return NULL;};
   virtual u32  SubSwallHostData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 	PKDISwallHostDataCB pDataCB, void *pContext){return NULL;};
   virtual u32  SetSwallRspCB(KDIHANDLE pKdiHandle, PKDISwallRspCB pControlRspCB, void *pContext){return NULL;};
   virtual u32  SwallCtlOpenWnd(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlOpenWndReq *ptSwallCtlOpenWndReq, 	void *pContext){return NULL;};
   virtual u32  SwallCtlCloseWnd(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlCloseWndReq *ptSwallCtlCloseWndReq, 	void *pContext){return NULL;};
   virtual u32  SwallCtlSaveScene(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlSaveSceneReq *ptSwallCtlSaveSceneReq, 	void *pContext){return NULL;};
   virtual u32  SwallCtlCallScene(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlCallSceneReq *ptSwalCtlCallSceneReq, 	void *pContext){return NULL;};
   virtual u32  SwallCtlMovePos(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlMovePosReq *ptSwallCtlMovePosReq, 	void *pContext){return NULL;};
   virtual u32  SwallCtlSetTopLev(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlSetTopLevReq *ptSwallCtlSetTopLevReq, 	void *pContext){return NULL;};
   virtual u32  SwallCtlSwitchSrc(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlSwitchSrcReq *ptSwallCtlSwitchSrcReq, 	void *pContext){return NULL;};
public:
    //////////////////////////////////////////////////////////////////////////
    //common
    ConnectBase *m_pConnectBase;

    DWORD m_dwAliveThreadID;
    BOOL32 m_bOnline; //通过赋值此值，来判断是否需要重连

	TKDIConnInfo m_tConnInfo;
	EMKDIProtocolType m_eProtocolType;
	TKDITCPCfg m_tKDITCPCfg;
    TKDIUDPCfg m_tKDIUDPCfg;
    TKDITCPServerCfg m_tKDITCPServerCfg;
    TKDICOMCfg m_tKDICOMCfg;

    PKDITPDataCB m_cbTPData; //透明数据
    PKDIAcessObjectStatCB m_cbStat; //设备状态
    void *m_pStateContext; //设备状态上下文指针
    void *m_pDataContext; //设备数据上下文指针
    void *m_pcbTPDataContext; //设备数据上下文指针
    
    BaseAliveCheckCB m_pAliveCheckCB;//自动检测
    BaseAliveLoginCB m_pAliveLoginCB;//自动登录
    s32  m_nAliveCheckTimer;//检测间隔
    void* m_pAliveContext;//数据上下文

protected:
    virtual u32 ConnectAccessObject(TKDIConnInfo* ptConnInfo, EMKDIProtocolType eProtocolType, PKDIPARAM pProtocolCfg, u32 dwCfgSize, void* pExtInfo) = 0;
    virtual u32 DisConnectAccessObject() = 0;

};