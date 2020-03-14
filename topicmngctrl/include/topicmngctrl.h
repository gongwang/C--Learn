#ifndef _TOPIC_MNG_CTRL_H
#define _TOPIC_MNG_CTRL_H
#include "moduleinterface.h"
class CKafkaHelper;
class MIAPI CTopicMngCtrl:public CModuleInterface
{
public:
	CTopicMngCtrl();
	~CTopicMngCtrl();

public:
	/*==============================================================================
	������    :	Init
	��  ��    : ��ʼ���ӿ�
	�㷨ʵ��  : 
	����˵��  : const pOutputCBFun& pCBFun  �ӿ���Ϣ����ص������ӿ�
	            const char* pszModuleId     �ӿڶ�Ӧģ���ģ���
				emCharsetType eCharSetType  �ӿ�����ַ����õı��뷽ʽ
	����ֵ˵��: emBIRetCode                 ���ýӿڷ�����
	ע  ��    :
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���		�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual EmBIRetCode Init(const s8* pszModuleId, const POutputCBFun pOutPutCB, 
		const PPrintLogCBFun pPrintLogCB = NULL, EmCharsetType eCharSetType = E_CHARSET_ANSI);

	/*==============================================================================
	������    :	UnInit
	��  ��    : ����ʼ���ӿ�
	�㷨ʵ��  : 
	����˵��  :   ���ýӿڷ�����
	ע  ��    :
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���	
	 ����ֵ˵��: emBIRetCode               	�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual EmBIRetCode UnInit(void);

	/*==============================================================================
	������    :	BindTopicInfo
	��  ��    : ��topic����Ϣ
	�㷨ʵ��  : 
	����˵��  : CMITopic2MsgNoInfo topicMsg[] topic����Ϣ�İ󶨹�ϵ����
	            u32 infoSize                topic����Ϣ�İ󶨹�ϵ�������   
	����ֵ˵��: emBIRetCode                 ���ýӿڷ�����
	ע  ��    :
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���		�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual EmBIRetCode BindTopicInfo(CMITopic2MsgNoInfo topicMsg[], u32 infoSize);

	/*==============================================================================
	������    :	UnBindTopicInfo
	��  ��    : ���topic����Ϣ
	�㷨ʵ��  : 
	����˵��  : CMITopic topic[]            topic��Ϣ
	            u32 topicSize               topic��Ϣ����Ŀ
	����ֵ˵��: emBIRetCode                 ���ýӿڷ�����
	ע  ��    :
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���		�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual EmBIRetCode UnBindTopicInfo(CMITopic topic[],  u32 topicSize);

	/*==============================================================================
	������    :	SetRunInfo
	��  ��    : �������в���
	�㷨ʵ��  : 
	����˵��  : CRunInfoItem achRunInfo[]  �������в�������
	            u32 infoSize             ���в�������ĸ���
	����ֵ˵��: emBIRetCode              ���ýӿڷ�����
	ע  ��    : 
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���		�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual EmBIRetCode SetRunInfo(CMIRunInfo achRunInfo[], u32 infoSize);

	/*==============================================================================
	������    :	RunModule
	��  ��    : ����ģ��
	�㷨ʵ��  : 
	����˵��  : 
	����ֵ˵��: emBIRetCode                 ���ýӿڷ�����
	ע  ��    :
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���		�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual EmBIRetCode RunModule(void);

	/*==============================================================================
	������    :	GetRunInfo
	��  ��    : ��ȡ������Ϣ
	�㷨ʵ��  : 
	����˵��  : const s8* pszBusinessName ҵ������
	            const s8* pszHost         ��Ӧ�ĵ�ַ
				const u16 wPort           ��Ӧ�Ķ˿�
	����ֵ˵��: void*  ��������ӷ��ص�Ϊ���Ӿ������������ӣ�����Ϊ��ֵ
	ע  ��    :
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���		�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual s32 GetRunInfo(const s8* pszBusinessName, const s8* pszHost, 
		const u16 wPort);

	/*==============================================================================
	������    :	GetModuleId
	��  ��    : ��ȡģ���ID��
	�㷨ʵ��  : 
	����˵��  : 
	����ֵ˵��: 
	ע  ��    :
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���		�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual const s8* GetModuleId(void);

	/*==============================================================================
	������    :	GetModuleStat
	��  ��    : ��ȡ����ģ���״̬
	�㷨ʵ��  : 
	����˵��  : emModuleStat& eModuleStat ����ģ���״̬��Ϣ
	����ֵ˵��: ״̬��Ϣ����
	ע  ��    :
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���		�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual const s8* GetModuleStat(EmModuleStat& eModuleStat);

	/*==============================================================================
	������    :	InputMsg
	��  ��    : �ӿ�������Ϣ�ӿ� 
	�㷨ʵ��  : 
	����˵��  : const u32 dwMsgNo           ��������Ϣ��
	            const s8* pszTaskKey:       ��Ӧҵ��ļ�ֵ
	            const u32 serialNoLen��     ��Ӧҵ��ļ�ֵ�ĳ���
                const u8* pszMsgContent��   ��Ϣ������
	            const u32 msgContentLen��   ��Ӧ��Ϣ�ĳ���
	            const void* pUserData��     ��������
	����ֵ˵��: emBIRetCode                 ���ýӿڷ�����
	ע  ��    :
	--------------------------------------------------------------------------------
	 �޸ļ�¼:
	 ����		�汾		�޸���		�߶���		�޸ļ�¼
	 2015/2/3  V1.0		Z.H						���� 
	================================================================================*/
	virtual EmBIRetCode InputMsg(const u32 dwAppInstID, const u32 dwMsgNo,  const s8* pszTaskKey, const u32 taskKeyLen,
		const u8* pszMsgContent, const u32 msgContentLen, const void* pUserData = NULL);
	virtual EmBIRetCode InputMsg(const s8 * pszTopic, const s8* pszTaskKey, const u32 taskKeyLen,
		const u8* pszMsgContent, const u32 msgContentLen, const void* pUserData = NULL);
public:
	void OutputMsg(const u32 dwAppInstID, const u32 dwMsgNo, const s8* pszTaskKey = NULL, 
		const u32 taskKeyLen = 0, const u8* pszMsgContent = NULL, const u32 msgContentLen = 0, const void* pUserData = NULL);

	void PrintLog(EmMdleLogLevel eMdLogLevel, const s8* const szFormat, ...);

	EmCharsetType GetCharsetType(){return m_eCharSetType;}


private:
	CKafkaHelper *m_pcKafkaHelper;
	POutputCBFun m_pOutputCB;
	PPrintLogCBFun m_pPrintLogCB;
	EmCharsetType m_eCharSetType;
	s8 m_achModuleId[MI_MAX_STR_LEN];
};
#endif //_PU_MNG_CTRL_H