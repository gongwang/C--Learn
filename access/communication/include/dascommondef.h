/********************************************************************
	created:	2012/09/12 11:23 
	file base:	dascommondef.h
	author:		Bing
	
	purpose:					
*********************************************************************/
#pragma once

#include "dasstring.h"

namespace dascommon
{

//2.0 定义的动态库存放路径：模块路径\\policy\\设备类型
#ifdef USE_GLOG
	#define DOOR_DLL_PATH		"policy\\door\\"
	#define ALARM_DLL_PATH		"policy\\alarm\\"
	#define SPEAK_DLL_PATH		"policy\\speak\\"
	#define SMS_DLL_PATH		"policy\\sms\\"
	#define LED_DLL_PATH		"policy\\led\\"
	#define POWERGRID_DLL_PATH	"policy\\powergrid\\"
	#define PUINT_DLL_PATH		"policy\\puint\\"
	#define BROADCAST_DLL_PATH  "policy\\broadcast\\"
	#define PATROL_DLL_PATH     "policy\\patrol\\"
    #define BAYONET_DLL_PATH    "policy\\bayonet\\"
#else
	#define DOOR_DLL_PATH		""
	#define ALARM_DLL_PATH		""
	#define SPEAK_DLL_PATH		""
	#define SMS_DLL_PATH		""
	#define LED_DLL_PATH		""
	#define POWERGRID_DLL_PATH	""
	#define PUINT_DLL_PATH		""
	#define BROADCAST_DLL_PATH  ""
	#define PATROL_DLL_PATH     "" 
    #define BAYONET_DLL_PATH    ""
#endif


	//Set class un-copyable if you declare this macro in you class.
#define CLASS_UN_COPYABLE(classname) \
private: \
	classname(const classname&); \
	classname& operator=(const classname&);

	//初始化错误码
#define  INIT_ERR() \
	/*m_strLastErr = "未知错误"*/;\
	m_dwLastErrNo = (u32)(0);\

	//声明错误获取和设置函数及其变量
#define  DECLARE_ERR() \
public:\
	const dascommon::CspChar* GetLastErrStr() const;\
	u32		GetLastErrNo() const;\
	void	SetLastErr(const u32 dwErrNo, const dascommon::CspChar* pErr);\
private:\
	dascommon::CspString	m_strLastErr;\
	u32		m_dwLastErrNo;\

	//声明错误获取和设置函数及其变量
#define  DECLARE_PURE_VRITUAL_ERR() \
public:\
	virtual const dascommon::CspChar* GetLastErrStr() const = 0;\
	virtual u32		GetLastErrNo() const = 0;\
	virtual void	SetLastErr(const u32 dwErrNo, const dascommon::CspChar* pErr) = 0;\

	//定义错误获取和设置函数
#define IMPLEMENT_ERR(class_name) \
	const dascommon::CspChar* class_name::GetLastErrStr() const{return m_strLastErr.c_str();}\
	u32 class_name::GetLastErrNo() const{return m_dwLastErrNo;}\
	void class_name::SetLastErr(const u32 dwErrNo, const dascommon::CspChar* pErr){\
	if (pErr != NULL){\
	m_strLastErr = pErr;\
	m_dwLastErrNo = dwErrNo;\
	}\
}\

	//声明错误获取和设置函数及其变量
#define  DECLARE_IMPLEMENT_ERR() \
private:\
	dascommon::CspString	m_strLastErr;\
	u32		m_dwLastErrNo;\
public:\
	const dascommon::CspChar* GetLastErrStr() const{\
	return m_strLastErr.c_str();\
}\
	\
	u32 GetLastErrNo() const{\
	return m_dwLastErrNo;\
}\
	\
	void SetLastErr(const u32 dwErrNo, const dascommon::CspChar* pErr){\
		if (pErr != NULL){\
			m_strLastErr = pErr;\
			m_dwLastErrNo = dwErrNo;\
		}\
	}\

} //namespace dascommon