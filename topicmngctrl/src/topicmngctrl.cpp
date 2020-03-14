#include "topicmngctrl.h"
#include "kafkahelper.h"
#include <stdarg.h>

CTopicMngCtrl::CTopicMngCtrl():m_pcKafkaHelper(NULL),m_pOutputCB(NULL),m_pPrintLogCB(NULL)
{
	memset(m_achModuleId, 0, sizeof(m_achModuleId));
}

CTopicMngCtrl::~CTopicMngCtrl()
{
	if (m_pcKafkaHelper)
	{
		delete m_pcKafkaHelper;
		m_pcKafkaHelper = NULL;
	}
}

EmBIRetCode CTopicMngCtrl::Init(const s8* pszModuleId, const POutputCBFun pOutPutCB, 
	const PPrintLogCBFun pPrintLogCB, EmCharsetType eCharSetType)
{
	if (pszModuleId)
	{
		strncpy(m_achModuleId, pszModuleId, sizeof(m_achModuleId)-1);
	}

	m_pOutputCB    = pOutPutCB;
	m_pPrintLogCB  = pPrintLogCB;
	m_eCharSetType = eCharSetType;
	if (NULL == m_pcKafkaHelper)
	{
		m_pcKafkaHelper = new CKafkaHelper(this);
	}
	return BI_NO_ERR;
}

EmBIRetCode CTopicMngCtrl::UnInit(void)
{
	if (m_pcKafkaHelper)
	{
		m_pcKafkaHelper->StopKafka();
	}
	return BI_NO_ERR;
}

EmBIRetCode CTopicMngCtrl::BindTopicInfo(CMITopic2MsgNoInfo topicMsg[], u32 infoSize)
{
	if (NULL == m_pcKafkaHelper) {return BI_NO_INIT_ERR;}
	if (FALSE == m_pcKafkaHelper->BindTopicInfo(topicMsg, infoSize))
	{
		return BI_NO_RUN_ERR;
	}

	return BI_NO_ERR;
}

EmBIRetCode CTopicMngCtrl::UnBindTopicInfo(CMITopic topic[],  u32 topicSize)
{
	if (NULL == m_pcKafkaHelper) {return BI_NO_INIT_ERR;}
	if (FALSE == m_pcKafkaHelper->UnBindTopicInfo(topic, topicSize))
	{
		return BI_NO_RUN_ERR;
	} 

	return BI_NO_ERR;
}

EmBIRetCode CTopicMngCtrl::SetRunInfo(CMIRunInfo achRunInfo[], u32 infoSize)
{
	if (NULL == m_pcKafkaHelper) {return BI_NO_INIT_ERR;}
	if (FALSE == m_pcKafkaHelper->StartKafka(achRunInfo, infoSize))
	{
		return BI_NORMAL_ERR;
	}
	return BI_NO_ERR;
}

EmBIRetCode CTopicMngCtrl::RunModule(void)
{
	return BI_NO_ERR;
}

s32 CTopicMngCtrl::GetRunInfo(const s8* pszBusinessName, const s8* pszHost, const u16 wPort)
{
	if (NULL == m_pcKafkaHelper) {return NULL;}
	return m_pcKafkaHelper->GetKafkaSerHandle(pszHost, wPort);
}

const s8* CTopicMngCtrl::GetModuleId(void)
{
	return m_achModuleId;
}


const s8* CTopicMngCtrl::GetModuleStat(EmModuleStat& eModuleStat)
{
	if (NULL == m_pcKafkaHelper) 
	{
		eModuleStat = MS_NO_RUN;
		return "module is no run.";
	}

	if(m_pcKafkaHelper->GetKafkaStat() > 0)
	{
		eModuleStat = MS_RUN_SUCC;
		return "module run succ.";
	}

	eModuleStat = MS_COMM_DISCONN;

	return "module is disconn";
}


EmBIRetCode CTopicMngCtrl::InputMsg(const u32 dwAppInstID, const u32 dwMsgNo, const s8* pszTaskKey, const u32 taskKeyLen,
	const u8* pszMsgContent, const u32 msgContentLen, const void* pUserData)
{
	if (NULL == m_pcKafkaHelper) {return BI_NO_INIT_ERR;}
	if (FALSE == m_pcKafkaHelper->SendMsg(dwAppInstID, dwMsgNo, pszTaskKey, 
		taskKeyLen, pszMsgContent, msgContentLen, pUserData))
	{
		return BI_NORMAL_ERR;
	}

	return BI_NO_ERR;
}

EmBIRetCode CTopicMngCtrl::InputMsg(const s8 * pszTopic, const s8* pszTaskKey, const u32 taskKeyLen,
	const u8* pszMsgContent, const u32 msgContentLen, const void* pUserData)
{
	if (NULL == m_pcKafkaHelper) {return BI_NO_INIT_ERR;}
	if (FALSE == m_pcKafkaHelper->SendMsg( pszTopic, pszTaskKey, 
		taskKeyLen, pszMsgContent, msgContentLen, pUserData))
	{
		return BI_NORMAL_ERR;
	}

	return BI_NO_ERR;
}


void CTopicMngCtrl::OutputMsg(const u32 dwAppInstID, const u32 dwMsgNo, const s8* pszTaskKey, 
	const u32 taskKeyLen, const u8* pszMsgContent, const u32 msgContentLen, const void* pUserData)
{
	if (NULL == m_pOutputCB) {return ;}
	m_pOutputCB(dwAppInstID,dwMsgNo,pszTaskKey,taskKeyLen, pszMsgContent, msgContentLen, pUserData);

}

void CTopicMngCtrl::PrintLog(EmMdleLogLevel eMdLogLevel, const s8 *const szFormat, ...)
{
	if (NULL == m_pPrintLogCB) {return ;}
	s8 achLog[MI_MAX_DATA_LEN+1] = {0};
	va_list pvlist;
	va_start(pvlist, szFormat);
	int nLen = vsnprintf((char *)achLog, MI_MAX_DATA_LEN, szFormat, pvlist);
	va_end(pvlist);
	m_pPrintLogCB(eMdLogLevel, achLog, strlen(achLog));
}

extern "C" MIAPI CModuleInterface *CreateMInterObj(const s8 *pchContent, u32 dwContentLen)
{
	return new CTopicMngCtrl();
}

extern "C" MIAPI EmBIRetCode ReleaseMInterObj(CModuleInterface **pMInter)
{
	if (NULL == pMInter) {return BI_NO_ERR;}
	delete *pMInter;
	*pMInter = NULL;
	return BI_NO_ERR;
}