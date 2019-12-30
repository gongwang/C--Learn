/********************************************************************
	created:	2013/09/14 9:59 
	file base:	dascommonfun.h
	author:		Bing
	
	purpose:	
*********************************************************************/
#pragma once

#include "csp_bulid_config.h"
#include "kdvtype.h"
#include "databuff.h"
#include "dasstring.h"
#include <string>
#include <vector>

namespace dascommon
{
	time_t CurrentTimeT();
	//时间字符串的格式必须为：20110126160200
	const time_t TimeStrToUl( const s8* pTime, const s8* szToken = NULL, s32 nTokenLen = 0 );
	void StrTimestamp(char* timeBuf, const int bufMaxSize, time_t * pTime);

	bool IsStrValid( const s8* pszStr );

	CspString GetSysLastErrorStr();
	void GetSysLastErrorStr(u32& dwErr, CspString &strErr);

	size_t ParseString( const CspString& str, const s8* szToken, s32 nTokenLen, std::vector<CspString>& vec );
	CspString IpU32ToStr( const u32 dwIp );
	void IpU32ToStr( const u32 dwIp, CspString &str );
	u32 IpStrToU32( const s8* str );
	void ComIntToStr( const u16 wPort, CspString &str );
	CspString ComIntToStr(const u16 wPort);
	u32 StrToHexUL(const s8* pszStr); //字符串16进制
	u32 StrToOctUL(const s8* pszStr);
	u64 StrToHexULL(const s8* pszStr);
	u64 StrToOctULL(const s8* pszStr);

	void string_trim(std::string &str, const s8 szToken);
	void string_trim(std::string &str, const s8* szToken, s32 nTokenLen);

	CspString GetCurrentPath();
	const s8* const_basename(const char* filepath);

	//获取本地IP
	CspString GetLocalIp();

#ifdef CSP_OS_FAMILY_WINDOWS
	std::string UTF16_2_UTF8( const std::wstring& strUTF16 ); 
	std::wstring UTF8_2_UTF16( const std::string& strUTF8 );
	std::string UTF16_2_ANSI( const std::wstring& strUTF16 ); 
	std::wstring ANSI_2_UTF16( const std::string& strANSI );
	std::string UTF8_2_ANSI( const std::string& strUTF8 ); 
	std::string ANSI_2_UTF8( const std::string& strANSI ); 
#endif

	void PrintfDataBuff(const TDataBuff &tDataBuff, const s8* pDesc = "DataBuffBag");
	void PrintfDataBuff(const s8* pszData, const s32 len, const s8* pDesc = "DataBuffBag");
	void PrintfDataBuff(const u8* pszData, const s32 len, const s8* pDesc = "DataBuffBag");

	u64	MakeDWord(u32 dwHight, u32 dwLow);
}