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
	函数名    :	Init
	功  能    : 初始化接口
	算法实现  : 
	参数说明  : const pOutputCBFun& pCBFun  接口消息输出回调函数接口
	            const char* pszModuleId     接口对应模块的模块号
				emCharsetType eCharSetType  接口输出字符采用的编码方式
	返回值说明: emBIRetCode                 调用接口返回码
	注  明    :
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人		走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
	================================================================================*/
	virtual EmBIRetCode Init(const s8* pszModuleId, const POutputCBFun pOutPutCB, 
		const PPrintLogCBFun pPrintLogCB = NULL, EmCharsetType eCharSetType = E_CHARSET_ANSI);

	/*==============================================================================
	函数名    :	UnInit
	功  能    : 反初始化接口
	算法实现  : 
	参数说明  :   调用接口返回码
	注  明    :
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人	
	 返回值说明: emBIRetCode               	走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
	================================================================================*/
	virtual EmBIRetCode UnInit(void);

	/*==============================================================================
	函数名    :	BindTopicInfo
	功  能    : 绑定topic的信息
	算法实现  : 
	参数说明  : CMITopic2MsgNoInfo topicMsg[] topic和消息的绑定关系数组
	            u32 infoSize                topic和消息的绑定关系数组个数   
	返回值说明: emBIRetCode                 调用接口返回码
	注  明    :
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人		走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
	================================================================================*/
	virtual EmBIRetCode BindTopicInfo(CMITopic2MsgNoInfo topicMsg[], u32 infoSize);

	/*==============================================================================
	函数名    :	UnBindTopicInfo
	功  能    : 解绑定topic的消息
	算法实现  : 
	参数说明  : CMITopic topic[]            topic消息
	            u32 topicSize               topic消息的数目
	返回值说明: emBIRetCode                 调用接口返回码
	注  明    :
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人		走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
	================================================================================*/
	virtual EmBIRetCode UnBindTopicInfo(CMITopic topic[],  u32 topicSize);

	/*==============================================================================
	函数名    :	SetRunInfo
	功  能    : 设置运行参数
	算法实现  : 
	参数说明  : CRunInfoItem achRunInfo[]  设置运行参数数组
	            u32 infoSize             运行参数数组的个数
	返回值说明: emBIRetCode              调用接口返回码
	注  明    : 
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人		走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
	================================================================================*/
	virtual EmBIRetCode SetRunInfo(CMIRunInfo achRunInfo[], u32 infoSize);

	/*==============================================================================
	函数名    :	RunModule
	功  能    : 运行模块
	算法实现  : 
	参数说明  : 
	返回值说明: emBIRetCode                 调用接口返回码
	注  明    :
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人		走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
	================================================================================*/
	virtual EmBIRetCode RunModule(void);

	/*==============================================================================
	函数名    :	GetRunInfo
	功  能    : 获取运行信息
	算法实现  : 
	参数说明  : const s8* pszBusinessName 业务名称
	            const s8* pszHost         对应的地址
				const u16 wPort           对应的端口
	返回值说明: void*  如果有连接返回的为连接句柄，如果无连接，返回为空值
	注  明    :
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人		走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
	================================================================================*/
	virtual s32 GetRunInfo(const s8* pszBusinessName, const s8* pszHost, 
		const u16 wPort);

	/*==============================================================================
	函数名    :	GetModuleId
	功  能    : 获取模块的ID号
	算法实现  : 
	参数说明  : 
	返回值说明: 
	注  明    :
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人		走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
	================================================================================*/
	virtual const s8* GetModuleId(void);

	/*==============================================================================
	函数名    :	GetModuleStat
	功  能    : 获取运行模块的状态
	算法实现  : 
	参数说明  : emModuleStat& eModuleStat 运行模块的状态信息
	返回值说明: 状态信息描述
	注  明    :
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人		走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
	================================================================================*/
	virtual const s8* GetModuleStat(EmModuleStat& eModuleStat);

	/*==============================================================================
	函数名    :	InputMsg
	功  能    : 接口输入信息接口 
	算法实现  : 
	参数说明  : const u32 dwMsgNo           操作的消息号
	            const s8* pszTaskKey:       对应业务的键值
	            const u32 serialNoLen：     对应业务的键值的长度
                const u8* pszMsgContent：   消息的内容
	            const u32 msgContentLen：   对应消息的长度
	            const void* pUserData：     辅助参数
	返回值说明: emBIRetCode                 调用接口返回码
	注  明    :
	--------------------------------------------------------------------------------
	 修改记录:
	 日期		版本		修改人		走读人		修改记录
	 2015/2/3  V1.0		Z.H						创建 
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