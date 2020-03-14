#pragma once
#include "subclass.h"

//子类名称
#define USING_POLICY_NAME   subclass
//生成dll的名称
#define CREATE_DLL_NAME       "accessdemo"

API void help()
{
    //////////////////////////////////////////////////////////////////////////
    OspPrintf(TRUE,FALSE,"****************API Help*******************\n");
    OspPrintf(TRUE,FALSE,"[1]getallstate            获取所有设备状态\n"); //三个空格
    //////////////////////////////////////////////////////////////////////////   

}
//common 不允许删除功能

//获取所有设备的状态
API void getallstate()
{
    OspPrintf( TRUE, FALSE, "getallstate success\n" );

    int nDevCount = GetDevObjectSize();

    for (int nIndex = 0; nIndex <nDevCount; nIndex++)
    {
        void *pDev = GetDevObject(nIndex);

        AccessBasePolicy* accessDemo =(AccessBasePolicy*)pDev;

        if (accessDemo == NULL)
        {
            OspPrintf(TRUE, FALSE,"获取设备为空\n");
            break;
        }

        OspPrintf( TRUE, FALSE, "DevName: %s\n DevID:%s \n DevState: %d\n",
            accessDemo->m_tConnInfo.m_szObjectName, accessDemo->m_tConnInfo.m_szObjectNo, accessDemo->m_bOnline);

    }

}

KDI_SDK_API BOOL32 KDICALL KDI_InitDll(const TKDIInitInfo *ptInit)
{
    printf("初始化成功！\n");

    return TRUE;
}

KDI_SDK_API BOOL32 KDICALL KDI_UnInitDll()
{
    printf("反初始化成功！\n");
	return TRUE;
}

KDI_SDK_API BOOL32 KDICALL KDI_IsInitDll()
{
	return FALSE;
} 

KDI_SDK_API KDIHANDLE KDICALL KDI_CreateAccessObject()
{
	AccessBasePolicy *p = new USING_POLICY_NAME(CREATE_DLL_NAME);
	return p;
}

KDI_SDK_API u32 KDICALL KDI_DestroyAccessObject(KDIHANDLE pKdiHandle)
{
    if (!pKdiHandle)
    {
        return GSS_ERR_PARAM_IS_NULL;
    }
	delete pKdiHandle;
	pKdiHandle = NULL;

	return GSS_SUCCESS;
}

KDI_SDK_API u32 KDICALL KDI_ConnectAccessObject(KDIHANDLE pKdiHandle, 
	TKDIConnInfo* ptConnInfo, EMKDIProtocolType eProtocolType, 
	PKDIPARAM pProtocolCfg, u32 dwCfgSize, void* pExtInfo)
{
    if (!pKdiHandle)
    {
        return GSS_ERR_PARAM_IS_NULL;
    }

	return ((AccessBasePolicy*)pKdiHandle)->Open(ptConnInfo,eProtocolType,pProtocolCfg,dwCfgSize,pExtInfo);
}

KDI_SDK_API u32 KDICALL KDI_DisConnectAccessObject(KDIHANDLE pKdiHandle)
{
    if (!pKdiHandle)
    {
        return GSS_ERR_PARAM_IS_NULL;
    }
	return ((AccessBasePolicy*)pKdiHandle)->Close();
}

KDI_SDK_API s8* KDICALL KDI_GetLastError(KDIHANDLE pKdiHandle)//
{
    if (!pKdiHandle)
    {
        return "";
    }
	return ((AccessBasePolicy*)pKdiHandle)->GetLastError();
}

KDI_SDK_API u32 KDICALL KDI_GetLastErrNo(KDIHANDLE pKdiHandle)
{
    if (!pKdiHandle)
    {
        return GSS_ERR_PARAM_IS_NULL;
    }
	return ((AccessBasePolicy*)pKdiHandle)->GetLastErrNo();
}

KDI_SDK_API s8* KDICALL KDI_GetDllVersion()
{
	return "1.0.0";
}
KDI_SDK_API void KDICALL KDI_PrintfDllStatus()
{
	
}
KDI_SDK_API void KDICALL KDI_SetLogLev(EMKDILogLevel eLevel)
{
	return;
}

KDI_SDK_API void KDICALL KDI_SetTransParentDataCB(KDIHANDLE pKdiHandle, PKDITPDataCB pTPDataCB, void *pContext)
{
    if (!pKdiHandle)
    {
        printf("pKdiHandle Null\n");
        return;
    }
    ((AccessBasePolicy*)pKdiHandle)->SetTransParentDataCB(pTPDataCB, pContext);
}

KDI_SDK_API void KDICALL KDI_TransParentData(KDIHANDLE pKdiHandle, s8* pszFiled, 
    u16 wMsgNo, u32 dwSn, u8 byDataFormat, void* pTransData, 
    s32 dwDataLen, void *pContext)
{
    if (!pKdiHandle)
    {
        printf("pKdiHandle Null\n");
        return;
    }
    ((AccessBasePolicy*)pKdiHandle)->TransParentData(pszFiled, wMsgNo, dwSn, byDataFormat, pTransData, dwDataLen, pContext);
}

KDI_SDK_API u32 KDICALL KDI_SubAcessNodeStat(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 
    PKDIAcessObjectStatCB pStatCB, void *pContext)
{
    if (!pKdiHandle)
    {
        return GSS_ERR_PARAM_IS_NULL;
    }
    return ((AccessBasePolicy*)pKdiHandle)->SubAcessObjectStat(emSubType, pStatCB, pContext);
}


KDI_SDK_API u32 KDICALL KDI_SubAcessObjectCapSet(KDIHANDLE pKdiHandle, 
    PKDIAcessObjectCapSetCB pCapSetCB, void *pContext)
{
    if (!pKdiHandle)
    {
        return GSS_ERR_PARAM_IS_NULL;
    }
    return ((AccessBasePolicy*)pKdiHandle)->SubAcessObjectCapSet(pCapSetCB, pContext);
}


// 对讲需要实现如下接口 kdi_sdk_speak.h
KDI_SDK_API u32 KDICALL KDI_SubAcessNodeStat(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 
    PKDIAcessNodeStatCB pStatCB, void *pContext)
{
    if (!pKdiHandle)
    {
        return GSS_ERR_PARAM_IS_NULL;
    }
    return ((AccessBasePolicy*)pKdiHandle)->SubAcessNodeStat(emSubType, pStatCB, pContext);
}

KDI_SDK_API u32 KDICALL KDI_SubSpeakHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 
	PKDISpeakHostInfoCB pInfoCB, void *pContext)
{
    if (!pKdiHandle)
    {
        return GSS_ERR_PARAM_IS_NULL;
    }
	return ((AccessBasePolicy*)pKdiHandle)->SubSpeakHostInfo(emSubType, pInfoCB, pContext);
}

KDI_SDK_API u32 KDICALL KDI_SubSpeakHostData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 
	PKDISpeakHostDataCB pDataCB, void *pContext)
{
    if (!pKdiHandle)
    {
        return GSS_ERR_PARAM_IS_NULL;
    }
	return ((AccessBasePolicy*)pKdiHandle)->SubSpeakHostData(emSubType, pDataCB, pContext);
}

//以下 需要实现的内容 自行安排，不需要的删除即可
// alram 告警 需要实现如下接口  "kdi_sdk_alarm.h"
KDI_SDK_API u32 KDICALL KDI_SubAlarmHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIAlarmHostInfoCB pInfoCB, void *pContext){return NULL;};
KDI_SDK_API u32 KDICALL KDI_SubAlarmHostData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIAlarmHostDataCB pDataCB, void *pContext){return NULL;};
KDI_SDK_API void KDICALL KDI_SetAlarmControlCB(KDIHANDLE pKdiHandle, PKDIAlarmControlRspCB pControlRspCB){return;};
KDI_SDK_API u32 KDICALL KDI_AlarmControl(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIAlarmControlReq *ptControlReq, void *pContext){return NULL;};
//bayonet // 道闸 需要实现如下接口  "kdi_sdk_bayonet.h"
KDI_SDK_API u32  KDICALL KDI_SubBayonetInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIBayonetInfoCB pInfoCB, void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SubBayonetData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIBayonetDataCB pDataCB, void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_GetBayonetInfo(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIGetBayonetReq *ptGetBayonetReq, void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_DealVehicleInfo(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIDealVehicleInfoReq *ptDealVehicleInfoReq, void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_BatchDealVehicleInfo(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIBatchDealVehicleInfoReq *ptKDIBatchDealVehicleInfoReq, 
    TKDIBatchDealVehicleInfoRsp *ptKDIBatchDealVehicleInfoRsp, void *pContext){return NULL;};
KDI_SDK_API u32 KDICALL KDI_DealVehicleJurisdiction(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIDealVehicleJstReq *ptDealVehicleJstReq, void *pContext){return NULL;};
KDI_SDK_API u32 KDICALL KDI_ControlBayonet(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIControlBayonetReq *ptControlBayonetReq, void *pContext){return NULL;};
//door// 门禁 需要实现如下接口  "kdi_sdk_door.h"
KDI_SDK_API u32 KDICALL KDI_SubDoorHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIDoorHostInfoCB pInfoCB, void *pContext){return NULL;};
KDI_SDK_API u32 KDICALL KDI_SubDoorHostData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIDoorHostDataCB pDataCB, void *pContext){return NULL;};
KDI_SDK_API u32 KDICALL KDI_SubCreditCardInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDICreditCardCB pDataCB, void *pContext){return NULL;};
KDI_SDK_API u32 KDICALL KDI_SetSyncCardInfoCB(KDIHANDLE pKdiHandle, PKDISyncCardInfoCB pDataCB){return NULL;};
KDI_SDK_API void KDICALL KDI_SetDoorControlCB(KDIHANDLE pKdiHandle, PKDIDoorControlRspCB pControlRspCB, void *pContext){return;};
KDI_SDK_API u32 KDICALL KDI_DoorControl(KDIHANDLE pKdiHandle, u32 dwSn, const TKDIDoorControlReq *ptControlReq,  void *pContext){return NULL;};	
KDI_SDK_API u32 KDICALL KDI_SyncCardInfoReq(KDIHANDLE pKdiHandle, u32 dwSn, void *pContext){return NULL;};	
KDI_SDK_API u32 KDICALL KDI_AddDoorCard(KDIHANDLE pKdiHandle, const TKDICardList &tCardList, void *pContext){return NULL;};
KDI_SDK_API u32 KDICALL KDI_DelDoorCard(KDIHANDLE pKdiHandle, const TKDICardList &tCardList, void *pContext){return NULL;};
//patrol// 巡更 需要实现如下接口  "kdi_sdk_patrol.h"
KDI_SDK_API u32 KDICALL KDI_SubPatrolHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIPatrolHostInfoCB pInfoCB, void *pContext){return NULL;};
KDI_SDK_API u32 KDICALL KDI_SubPatrolHostData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDIPatrolHostDataCB pDataCB, void *pContext){return NULL;};
//sensor// 传感器 需要实现如下接口  "kdi_sdk_sensor.h"                
KDI_SDK_API u32 KDICALL KDI_SubSensorInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDISensorInfoCB pInfoCB, void *pContext){return NULL;};
KDI_SDK_API u32 KDICALL KDI_SubSensortData(KDIHANDLE pKdiHandle, EMKDISubType emSubType,	PKDISensorDataCB pDataCB, void *pContext){return NULL;};
//sms // 短信猫 需要实现如下接口  "kdi_sdk_sms.h"
KDI_SDK_API u32  KDICALL KDI_SubSmsHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, PKDISmsHostInfoCB pInfoCB, void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SetRecvSmsCB(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 	PKDIRecvSmsCB pDataCB, void *pContext){return NULL;};
KDI_SDK_API void KDICALL KDI_SendSmsInfo(KDIHANDLE pKdiHandle, TKDISmsInfo tSmsInfo){};
//swall// 拼控系统 需要实现如下接口  "kdi_sdk_swall.h"
KDI_SDK_API u32  KDICALL KDI_SubSwallHostInfo(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 	PKDISwallHostInfoCB pInfoCB, void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SubSwallHostData(KDIHANDLE pKdiHandle, EMKDISubType emSubType, 	PKDISwallHostDataCB pDataCB, void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SetSwallRspCB(KDIHANDLE pKdiHandle, PKDISwallRspCB pControlRspCB, void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SwallCtlOpenWnd(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlOpenWndReq *ptSwallCtlOpenWndReq, 	void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SwallCtlCloseWnd(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlCloseWndReq *ptSwallCtlCloseWndReq, 	void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SwallCtlSaveScene(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlSaveSceneReq *ptSwallCtlSaveSceneReq, 	void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SwallCtlCallScene(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlCallSceneReq *ptSwalCtlCallSceneReq, 	void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SwallCtlMovePos(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlMovePosReq *ptSwallCtlMovePosReq, 	void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SwallCtlSetTopLev(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlSetTopLevReq *ptSwallCtlSetTopLevReq, 	void *pContext){return NULL;};
KDI_SDK_API u32  KDICALL KDI_SwallCtlSwitchSrc(KDIHANDLE pKdiHandle, u32 dwSn, const TKDISwallCtlSwitchSrcReq *ptSwallCtlSwitchSrcReq, 	void *pContext){return NULL;};