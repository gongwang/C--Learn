#ifndef _KAFKA_HELPER_H
#define _KAFKA_HELPER_H

#include "mqinterface.h"
#include "moduleinterface.h"
#include "topicmngdef.h"
#include "icp/icp.h"
#include "utf8_ansi.h"
#include <string.h>
#include <map>

const u16 MAX_KH_BUFF_LEN = (u16)102400;

using std::map;
//map<string, MQTOPICHANDLE> g_SenderMap;
void pReceEventCB(MQTOPICHANDLE handle, s32 nErrno, s8* pszErr, s32 nErrLen, KD_PTR pUser);

class CTopicMngCtrl;
class CKafkaHelper{
public:
	CKafkaHelper(CTopicMngCtrl *pTopicMngCtrl);
	~CKafkaHelper();
public:
	BOOL32 StartKafka(CMIRunInfo* pchRunInfo, unsigned int infoSize);
	BOOL32 StopKafka(void);
	s32 GetKafkaSerHandle(const s8* pszHost, const u16 wPort);
	BOOL32 BindTopicInfo(CMITopic2MsgNoInfo* ptopicMsg, unsigned int infoSize);
	BOOL32 UnBindTopicInfo(CMITopic *pTopic,  unsigned int topicSize);
	s32 GetKafkaStat(void);
	BOOL32 SendMsg(const u32 dwAppInstID, const u32 dwMsgNo, const s8* pszTaskKey, const u32 taskKeyLen, 
		const u8* pszMsgContent, const u32 msgContentLen, const void* pUserData);
	BOOL32 SendMsg(const s8* pszTopic, const s8* pszTaskKey, const u32 taskKeyLen, 
		const u8* pszMsgContent, const u32 msgContentLen, const void* pUserData);
public:
	void PostConnSuccKafka(MQHANDLE pSerHandle);
	void PostDisConnKafka(MQHANDLE pSerHandle);



private:
	s32 CreateReceiver(TMQTopicInfo *pchTopicInfoArr, s32 nTopicNum, s32 nTimeoutMs);
	BOOL32 DeleteReceiver(s32 nRecHandle);
	s32 CreateSender(const char* pszTopic, s32 nTimeoutMs);
	BOOL32 DeleteSender(s32 nSendHandle);

	//��kafka������Ϣͳһ���
// 	template<class CMsg>
// 	static void PostMsgToKafka(const s8* szEvent, CMsg& cMsg, const u8* szKey, u32 dwKeyLen)
// 	{
// 		string strJson = cMsg.ToJson();
// 
// 		s8 aszJson[102400] = {0};
// 		AnsiToUTF8(strJson.c_str(), strJson.length(), aszJson, 102400);
// 
// 		printf("������Ϣ ת��ǰ��json %d|%s\n", strJson.length(),strJson.c_str());
// 		printf("������Ϣ ת�����json %d|%s\n",strlen(aszJson), aszJson);
// 
// 		TMQSenderParam tSenderParam;
// 		tSenderParam.pEventCb = pReceEventCB;
// 		strcpy(tSenderParam.szName, "");
// 		tSenderParam.nMaxQueueNum = 1000;
// 		tSenderParam.nMaxQueueRetainTime = 100;
// 
// 		MQTOPICHANDLE nTopicHandle = 0;
// 		string strEvent = szEvent;
// 
// 		map<string, int>::iterator iter = g_pcSerData->m_SenderMap.find(strEvent);
// 		if (iter == g_pcSerData->m_SenderMap.end())
// 		{
// 			//map�в����ڸ�topicΪkey��ֵ�������topic�ʹ�����kafka������
// 			//������Ӧ��kafka��Ϣ������
// 			nTopicHandle = MQCreateSender(g_pcSerData->m_pSerHandle, 
// 				szEvent, tSenderParam, 1000);
// 			g_pcSerData->m_SenderMap.insert(std::make_pair(strEvent, nTopicHandle));
// 		}
// 		else
// 		{
// 			//map���и�ֵ��ȡ����kafka�����������ʹ��
// 			for (; iter != g_pcSerData->m_SenderMap.end(); ++iter)
// 			{
// 				if (szEvent == iter->first)
// 				{
// 					nTopicHandle = iter->second;
// 				}
// 			}
// 		}
// 
// 		if (0 == nTopicHandle)
// 		{
// 			LogPrintNormal(LOG_LEV_DEBUG, "���� %s ��Ϣ������ʧ��!\n", szEvent);
// 			printf("���� %s ��Ϣ������ʧ��!\n", szEvent);
// 		}
// 
// 		//������Ϣ
// 		int nRet = MQSendMessage(nTopicHandle, (const u8*)aszJson, strlen(aszJson), szKey, dwKeyLen, false, 0, 1000);
// 		if (0 != nRet)
// 		{
// 			LogPrintNormal(LOG_LEV_DEBUG, "���� %s ��Ϣʧ�ܣ�������%d, ��������: %s\n", szEvent, nRet, MQGetErrDesc(nRet));
// 			printf("���� %s ��Ϣʧ�ܣ�������%d, ��������: %s\n", szEvent, nRet, MQGetErrDesc(nRet));
// 		}
// 	}
private:
	void CreateStatusCheckThread(void);
	void DeleteStatusCheckThread(void);
public:
	MQHANDLE GetSerHandle() {return m_pSerHandle;}
	void SetQuit(BOOL32 bQuit){ m_bQuit = bQuit;}
	BOOL32 GetQuit() {return m_bQuit;}
	CMITopicMng& GetTopicMng(){return m_cTopicMng;}
	CTopicMngCtrl* GetTopicMngCtrl() {return m_pTopicMngCtrl;}
	void SetConnSucc(BOOL32 bConnSucc) {m_bConnSucc = bConnSucc;}
	BOOL32 GetConnSucc() {return m_bConnSucc;}

private:
	static void* StatusCheckThread(void* args);
private:
	CTopicMngCtrl *m_pTopicMngCtrl;      //topic������
	MQHANDLE m_pSerHandle;               //server���
	BOOL32 m_bQuit;                      //�Ƿ��˳��ı�־
	CMITopicMng m_cTopicMng;             //topic������
	CThreadLockMutex m_HelperLock;       //help��ͬ����
	list<CMIRunInfo> m_cRunInfoLst;      //������Ϣ
	BOOL32 m_bConnSucc;                  //����kafka�ɹ�
};

#endif