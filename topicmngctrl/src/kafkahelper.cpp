#include "topicmngctrl.h"
#include "kafkahelper.h"
#include "topics.h"
//#include <process.h>
#ifdef _MSC_VER
#include "pthread_w32.h"
#else
#include <unistd.h>
#include <pthread.h>
#endif //_MSC_VER

#ifdef _MSC_VER
#define sleep(x) Sleep(x * 1000)
#else
#include <unistd.h>
#endif //_MSC_VER

//事件回调
void pReceEventCB(MQTOPICHANDLE handle, s32 nErrno, s8* pszErr, s32 nErrLen, void* pUser)
{
}

//kafka消息回调
void pReceMsgCB(MQTOPICHANDLE handle, PTMQMessage pMessage, void* pUser)
{
	string strKey="";
	if(pMessage->pszKey || pMessage->nKeyLen > 0){
		strKey.append((s8*)pMessage->pszKey, 0, pMessage->nKeyLen);
	}
	printf("收到资源管理服务器的topic消息 %s, key： %s, key长度： %d，value：%s\n", 
		pMessage->szTopic, strKey.c_str(), pMessage->nKeyLen, pMessage->pszValue);
	CKafkaHelper *pHelper = (CKafkaHelper*)pUser;
	if(NULL == pHelper) {printf("pHelper is null\n");return;}
	
	string strTopic = pMessage->szTopic;
	//查找对应的topic
	CMIRecTopic2MsgNoInfo *pTopicInfo = pHelper->GetTopicMng().FindRecMsgNo2TopicByTopic(strTopic);
	if(NULL == pTopicInfo) {printf("topic is null\n");return;}

	if(pTopicInfo->GetTopicHandle() != handle) {printf("handle is diff, %d - %d\n", pTopicInfo->GetTopicHandle(), handle); return;}

	//判断类型走向
	u32 dwDstIID = 0;
	u32 dwMsgNo  = 0;
	if(!pTopicInfo->GetTopicExInfoMng().FindTopicEx("", dwDstIID, dwMsgNo))
	{
		printf("未找到对应的消息号或者进程号\n");
		return;
	}

	if(E_CHARSET_ANSI == pHelper->GetTopicMngCtrl()->GetCharsetType())
	{
		s8 *psDstValue = new s8[MAX_KH_BUFF_LEN];
		if(NULL == psDstValue) {return;}
		memset(psDstValue, 0, MAX_KH_BUFF_LEN);
		UTF8ToAnsi((const s8*)pMessage->pszValue, pMessage->nValueLen, psDstValue);
		printf("转化后的接收json结构体为%s\n", psDstValue);
		pHelper->GetTopicMngCtrl()->OutputMsg(dwDstIID, dwMsgNo, 
			(s8*)strKey.c_str(), pMessage->nKeyLen, (u8*)psDstValue, strlen(psDstValue));
		delete[] psDstValue;
	}
	else
	{
		pHelper->GetTopicMngCtrl()->OutputMsg(dwDstIID, dwMsgNo, 
			(s8*)strKey.c_str(), pMessage->nKeyLen, (u8*)pMessage->pszValue, pMessage->nValueLen);
	}
}

void KafkaLogCB(eMQLoglev eLevel, const s8* pszLog, u32 nLogLen, void* pUser)
{
	if(NULL == pUser) {return;}
	CKafkaHelper *pHelper = (CKafkaHelper*)pUser;
	if(NULL == pHelper) {return;}
	pHelper->GetTopicMngCtrl()->PrintLog((EmMdleLogLevel)eLevel, pszLog);
	printf("LOG:lev=%d %s\n", eLevel, pszLog);
}

void KafkaSerDownCB(MQHANDLE handle, void* pUser)
{
	//TODO check mqkafkaserver has been down or not
	//创建线程用来进行延迟处理，防止主线程阻塞导致后续行为出错
	CKafkaHelper *pHelper = (CKafkaHelper*)pUser;
	if (NULL == pHelper) {return;}
	pHelper->PostDisConnKafka(handle);
	//pHelper->CreateStatusCheckThread();
}

CKafkaHelper::CKafkaHelper(CTopicMngCtrl *pTopicMngCtrl):m_pSerHandle(NULL),
	m_bQuit(FALSE),m_bConnSucc(FALSE)
{
	m_pTopicMngCtrl = pTopicMngCtrl;
}

CKafkaHelper::~CKafkaHelper()
{
	;
}


BOOL32 CKafkaHelper::StartKafka(CMIRunInfo* pchRunInfo, unsigned int infoSize){

	if (infoSize == 0){return FALSE;}

	CThreadAutoLock cLock(m_HelperLock);
	if(m_pSerHandle) {return TRUE;}
	if(!MQInit(KafkaLogCB, KafkaSerDownCB, (void*)this))
	{
		printf("mqinit failed!\n");
		return FALSE;
	}

	TMQServerInfo tServerInfo;
	tServerInfo.nServerPort = pchRunInfo[0].GetPort(); //6703/*g_pcSerData->m_tSerAddr.m_wPort*/;
	strcpy(tServerInfo.szServerHost, pchRunInfo[0].GetHost());
	//strcpy(tServerInfo.szServerHost, "172.16.64.12"/*achKafkaIp*/);

	m_pSerHandle = MQLoginServer(&tServerInfo, 1, NULL, NULL, 5000);
	if (NULL == m_pSerHandle){return FALSE;}

	m_cRunInfoLst.push_back(pchRunInfo[0]);
	if (0 != MQGetServerState(m_pSerHandle))
	{
		m_bConnSucc = TRUE;
		PostConnSuccKafka(m_pSerHandle);
	}

	CreateStatusCheckThread();
	return TRUE;
}

BOOL32 CKafkaHelper::StopKafka(void)
{
	CThreadAutoLock cLock(m_HelperLock);
	DeleteStatusCheckThread();  //停止检测线程

	TTopicHandleLst recHandleLst = m_cTopicMng.GetRecHandleLst();
	TTopicHandleLst sendHandleLst = m_cTopicMng.GetSendHandleLst();
	m_cTopicMng.ClearAllRecMsgNo2Topic();
	m_cTopicMng.ClearAllSendMsgNo2Topic();
	IHandelIter curItr = recHandleLst.begin();
	for(; curItr != recHandleLst.end(); ++curItr)
	{
		DeleteReceiver(*curItr);
	}

	curItr = sendHandleLst.begin();
	for (; curItr != sendHandleLst.end(); ++curItr)
	{
		DeleteSender(*curItr);
	}

	if(m_pSerHandle)
	{
		MQLogoutServer(m_pSerHandle);
		m_pSerHandle = NULL;
	}

	return TRUE;
}

s32 CKafkaHelper::GetKafkaSerHandle(const s8* pszHost, const u16 wPort)
{
	CThreadAutoLock cLock(m_HelperLock);
	return m_pSerHandle;
}

BOOL32 CKafkaHelper::BindTopicInfo(CMITopic2MsgNoInfo* ptopicMsg, unsigned int infoSize)
{
	if(NULL == m_pSerHandle) {return FALSE;}
	if(infoSize <= 0 ) {return FALSE;}

	s32 nTopicNum = 0;
	unsigned int nRecTimeOut = 0;
	TMQTopicInfo *achTopicInfo = new TMQTopicInfo[infoSize]();
	list<CMIRecTopic2MsgNoInfo *> cRecTopcLst;
	
	for (unsigned int nIdx=0; nIdx <infoSize; ++nIdx)
	{
		CMITopic2MsgNoInfo* pInfoTmp = (ptopicMsg+nIdx);
		string strTopic = pInfoTmp->GetTopic();
		if(E_SEND_TPIC == pInfoTmp->GetType())
		{
			//如果是发送的
			CThreadAutoLock cLock(m_HelperLock);
			CMISendTopic2MsgNoInfo* pSendTopic2MsgInfo = m_cTopicMng.FindSendMsgNo2TopicByTopic(strTopic);

			if (pSendTopic2MsgInfo) 
			{
				//topic相同，进行累增
				m_cTopicMng.TiredSendTopicMsgNoInfo(pSendTopic2MsgInfo, pInfoTmp->GetDevTypeName(), 
					pInfoTmp->GetAppInstID(), pInfoTmp->GetMsgNo());
				continue;
			}
			s32 nSendHandle = CreateSender(pInfoTmp->GetTopic(), pInfoTmp->GetTimeoutMs());
			m_cTopicMng.AddSendTopicMsgNoInfo(*pInfoTmp, nSendHandle);
		}
		else
		{
			//如果增加的是接收部分，则将接收的打包成一个接收器进行接收
			CThreadAutoLock cLock(m_HelperLock);
			CMIRecTopic2MsgNoInfo* pRecTopic2MsgInfo = m_cTopicMng.FindRecMsgNo2TopicByTopic(strTopic);
			if (pRecTopic2MsgInfo) 
			{
				m_cTopicMng.TiredRecTopicMsgNoInfo(pRecTopic2MsgInfo, pInfoTmp->GetDevTypeName(), 
					pInfoTmp->GetAppInstID(), pInfoTmp->GetMsgNo());
				continue;
			}

			s32 nTopicLen = strlen(pInfoTmp->GetTopic());
			nTopicLen = (sizeof((achTopicInfo+nTopicNum)->szTopicName)-1) < nTopicLen ?
				(sizeof((achTopicInfo+nTopicNum)->szTopicName)-1) : nTopicLen;
			strncpy((achTopicInfo+nTopicNum)->szTopicName, pInfoTmp->GetTopic(), nTopicLen);
			CMIRecTopic2MsgNoInfo *recTopicTmp = m_cTopicMng.AddRecTopicMsgNoInfo(*pInfoTmp);
			cRecTopcLst.push_back(recTopicTmp);
			if(pInfoTmp->GetTimeoutMs() > nRecTimeOut)
			{
				nRecTimeOut = pInfoTmp->GetTimeoutMs();
			}
			++nTopicNum;
		}
	}

	if(nTopicNum > 0)
	{
		s32 nRecHandle = CreateReceiver(achTopicInfo, nTopicNum, nRecTimeOut);
		if (nRecHandle > 0)
		{
			m_cTopicMng.AddRecHandle(nRecHandle);
		}

		//补齐handle号
		{
			CThreadAutoLock cLock(m_HelperLock);
			list<CMIRecTopic2MsgNoInfo *>::iterator recTopicItr = cRecTopcLst.begin();
			for (; recTopicItr != cRecTopcLst.end(); ++recTopicItr)
			{
				(*recTopicItr)->SetTopicHandle(nRecHandle);
			}
		}
		
	}

	delete[] achTopicInfo;
	return TRUE;
}

BOOL32 CKafkaHelper::UnBindTopicInfo(CMITopic *pTopic,  unsigned int topicSize)
{
	for(unsigned int nIdx =0; nIdx < topicSize; ++nIdx)
	{
		CThreadAutoLock cLock(m_HelperLock);
		string strTopic = (pTopic+nIdx)->GetTopic();
		if (m_cTopicMng.DelRecTopicMsgNoInfo(strTopic, (pTopic+nIdx)->GetAppInstID())) {continue;}

		CMISendTopic2MsgNoInfo *pSendInfo = m_cTopicMng.FindSendMsgNo2TopicByTopic(strTopic);
		if(NULL == pSendInfo) {continue;}

		DeleteSender(pSendInfo->GetTopicHandle());
		m_cTopicMng.DelSendTopicMsgNoInfo(strTopic, (pTopic+nIdx)->GetAppInstID());
	}

	return TRUE;
	;
}

s32 CKafkaHelper::GetKafkaStat(void)
{
	if (0 == m_pSerHandle){return 0;}
	return MQGetServerState(m_pSerHandle);
}

BOOL32 CKafkaHelper::SendMsg(const u32 dwAppInstID, const u32 dwMsgNo, const s8* pszTaskKey, const u32 taskKeyLen, 
	const u8* pszMsgContent, const u32 msgContentLen, const void* pUserData)
{
	BOOL32 bRet = FALSE;
	CMISendTopic2MsgNoInfo *pSendTopic = m_cTopicMng.FindSendMsgNo2TopicByAppInstMsgNo(dwAppInstID, dwMsgNo);
	if (NULL == pSendTopic) {return FALSE;}

	if (E_CHARSET_ANSI == m_pTopicMngCtrl->GetCharsetType())
	{
		s8 *psDstValue = new s8[MAX_KH_BUFF_LEN];
		if(NULL == psDstValue) {return FALSE;}
		AnsiToUTF8((s8*)pszMsgContent, msgContentLen, psDstValue, MAX_KH_BUFF_LEN);
		int nRet = MQSendMessage(pSendTopic->GetTopicHandle(), (const u8*)psDstValue, 
			strlen(psDstValue), (u8*)pszTaskKey, taskKeyLen, false, 0, 1000);
		if (0 != nRet)
		{
			m_pTopicMngCtrl->PrintLog(ML_LOG_LEVEL_ERR, "发送msgNo:%d,topic:%s 消息失败，错误码%d, 错误描述: %s\n", 
				dwMsgNo, pSendTopic->GetTopic(), nRet, MQGetErrDesc(nRet));
		}
		bRet = TRUE;
		delete psDstValue;
	}
	else
	{
		int nRet = MQSendMessage(pSendTopic->GetTopicHandle(), (const u8*)pszMsgContent, msgContentLen, 
			(u8*)pszTaskKey, taskKeyLen, false, 0, 1000);
		if (0 != nRet)
		{
			m_pTopicMngCtrl->PrintLog(ML_LOG_LEVEL_ERR, "发送msgNo:%d,topic:%s 消息失败，错误码%d, 错误描述: %s\n", 
				dwMsgNo, pSendTopic->GetTopic(), nRet, MQGetErrDesc(nRet));
		}
		bRet = TRUE;
	}

	return bRet;
}


BOOL32 CKafkaHelper::SendMsg(const s8* pszTopic, const s8* pszTaskKey, const u32 taskKeyLen, 
	const u8* pszMsgContent, const u32 msgContentLen, const void* pUserData)
{
	BOOL32 bRet = FALSE;
	string topic = pszTopic;
	CMISendTopic2MsgNoInfo *pSendTopic = m_cTopicMng.FindSendMsgNo2TopicByTopic(topic);
	if (NULL == pSendTopic) {return FALSE;}

	if (E_CHARSET_ANSI == m_pTopicMngCtrl->GetCharsetType())
	{
		s8 *psDstValue = new s8[MAX_KH_BUFF_LEN];
		if(NULL == psDstValue) {return FALSE;}
		AnsiToUTF8((s8*)pszMsgContent, msgContentLen, psDstValue, MAX_KH_BUFF_LEN);
		int nRet = MQSendMessage(pSendTopic->GetTopicHandle(), (const u8*)psDstValue, 
			strlen(psDstValue), (u8*)pszTaskKey, taskKeyLen, false, 0, 1000);
		if (0 != nRet)
		{
			m_pTopicMngCtrl->PrintLog(ML_LOG_LEVEL_ERR, "topic:%s 消息失败，错误码%d, 错误描述: %s\n", pSendTopic->GetTopic(), nRet, MQGetErrDesc(nRet));
		    printf("topic:%s 消息失败，错误码%d, 错误描述: %s\n", pSendTopic->GetTopic(), nRet, MQGetErrDesc(nRet));
		}
		bRet = TRUE;
		delete psDstValue;
	}
	else
	{
		int nRet = MQSendMessage(pSendTopic->GetTopicHandle(), (const u8*)pszMsgContent, msgContentLen, 
			(u8*)pszTaskKey, taskKeyLen, false, 0, 1000);
		if (0 != nRet)
		{
			m_pTopicMngCtrl->PrintLog(ML_LOG_LEVEL_ERR, "发送topic:%s 消息失败，错误码%d, 错误描述: %s\n",pSendTopic->GetTopic(), nRet, MQGetErrDesc(nRet));
		    printf("发送topic:%s 消息失败，错误码%d, 错误描述: %s\n",pSendTopic->GetTopic(), nRet, MQGetErrDesc(nRet));
		}
		bRet = TRUE;
	}

	return bRet;
}


void CKafkaHelper::PostConnSuccKafka(MQHANDLE pSerHandle)
{
	list<CMIRunInfo>::iterator itr = m_cRunInfoLst.begin();
	u32 dwAppIsntId = 0;
	u32 dwMsgNo = 0;
	for(;itr != m_cRunInfoLst.end(); ++itr)
	{
		dwAppIsntId = itr->GetAppInstID();
		dwMsgNo = itr->GetConnMsgNo();
		break;
	}
	m_pTopicMngCtrl->OutputMsg(dwAppIsntId, dwMsgNo);
}

void CKafkaHelper::PostDisConnKafka(MQHANDLE pSerHandle)
{
	list<CMIRunInfo>::iterator itr = m_cRunInfoLst.begin();
	u32 dwAppIsntId = 0;
	u32 dwMsgNo = 0;
	for(;itr != m_cRunInfoLst.end(); ++itr)
	{
		dwAppIsntId = itr->GetAppInstID();
		dwMsgNo = itr->GetDisconnMsgNo();
		break;
	}
	m_pTopicMngCtrl->OutputMsg(dwAppIsntId, dwMsgNo);
}

s32 CKafkaHelper::CreateReceiver(TMQTopicInfo *pchTopicInfoArr, s32 nTopicNum, s32 nTimeoutMs)
{
	if(NULL == m_pSerHandle) {return 0;}

	TMQReceiverParam tReceiverParam;
	tReceiverParam.nMsgTimeout = nTimeoutMs;
	tReceiverParam.pEventCb = pReceEventCB;
	tReceiverParam.pMessageCb = pReceMsgCB;
	tReceiverParam.pUser = (void*)this;
	strcpy(tReceiverParam.szName, "");
	//创建recevier
	MQTOPICHANDLE recHandle = MQCreateReceiver(m_pSerHandle, pchTopicInfoArr, 
		nTopicNum, tReceiverParam, nTimeoutMs);
	if (0 == recHandle)
	{
		printf("ERROR:create receiver failed!\n");
		return 0;
	}
	//开始接受数据
	MQStartReveive(recHandle);
	printf("recv started!\n");

	return recHandle;
}


BOOL32 CKafkaHelper::DeleteReceiver(s32 nRecHandle)
{
	//停止接受kafka消息
	MQStopReceive(nRecHandle);
	MQDeleteReceiver(nRecHandle);
	return TRUE;
}

s32 CKafkaHelper::CreateSender(const char* pszTopic, s32 nTimeoutMs)
{
	if(NULL == m_pSerHandle) {return 0;}
	TMQSenderParam tSenderParam;
	tSenderParam.pEventCb = pReceEventCB;
	strcpy(tSenderParam.szName, "");
	tSenderParam.nMaxQueueNum = 1000;
	//tSenderParam.nMaxQueueRetainTime = 100;
	tSenderParam.pUser = (void*)this;

	MQTOPICHANDLE nTopicHandle = MQCreateSender(
		m_pSerHandle, pszTopic, tSenderParam, nTimeoutMs);
	return nTopicHandle;
}

BOOL32 CKafkaHelper::DeleteSender(s32 nSendHandle)
{
	MQDeleteSender(nSendHandle);
	return TRUE;
}

void CKafkaHelper::CreateStatusCheckThread(void)
{
	pthread_t tThread;
	// 创建detached线程 
	pthread_attr_t attr;     
	pthread_attr_init(&attr); 
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);  
	//if (0 == MQGetServerState(m_pSerHandle))
	{
		int ret = pthread_create(&tThread, &attr, StatusCheckThread, (void*)this);
		printf("启动加测连接状态线程\n");
		if (ret != 0)
		{
			printf("create ReConnectThread failed: error_code=%d\n", ret );
		}
	}

	pthread_attr_destroy (&attr);
}

void CKafkaHelper::DeleteStatusCheckThread(void)
{
	m_bQuit = TRUE;
	s32 nCount = 0;
	do 
	{
		if(FALSE == m_bQuit) {break;}
		sleep(1);
		//Sleep(500);
		++nCount;
	} while (nCount < 16);
	
	m_bQuit = FALSE;
}

void* CKafkaHelper::StatusCheckThread(void* args)
{
	CKafkaHelper *pHelper = (CKafkaHelper*)args;
	if (NULL == pHelper|| 0 == pHelper->GetSerHandle()) {return NULL;}
	do 
	{
		sleep(3);
		//Sleep(3000);
		if(pHelper->GetQuit())
		{
			//线程退出
			pHelper->SetQuit(FALSE);
			break;
		}

		//避免时序问题
		if(0 == pHelper->GetSerHandle()) {break;}

		if (0 != MQGetServerState(pHelper->GetSerHandle()))
		{
			//printf("检测连接kafka状态正常\n");
			if (!pHelper->GetConnSucc())
			{
				pHelper->SetConnSucc(TRUE);
				pHelper->PostConnSuccKafka(pHelper->GetSerHandle());
			}
			continue;
		}

		MQReConnectServer(pHelper->GetSerHandle(), 5000);
	} while (TRUE);

	return 0;
}