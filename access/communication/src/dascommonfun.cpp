#include "dascommonfun.h"
#include <ctime>

#ifdef CSP_OS_FAMILY_WINDOWS
#include <Windows.h>
#include <WinSock2.h>
#include <cstdarg> 
#include <xutility>
#include <tchar.h>
#elif CSP_OS_FAMILY_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#endif

namespace dascommon
{
	time_t CurrentTimeT()
	{
		struct tm* local;
		time_t t;
		t = time(NULL);
		local = localtime(&t);

		return t;
	}

	const time_t TimeStrToUl( const s8* pTime, const s8* szToken/* = NULL*/, s32 nTokenLen/* = 0*/ )
	{
		if (!IsStrValid(pTime))
		{
			return time_t(0);
		}

		CspString strTemp(pTime);
		if (NULL != szToken)
		{
			strTemp.trim(szToken, nTokenLen);
		}

		if (strTemp.size() != 14)
		{
			return time_t(0);
		}

		std::string strTime;
		struct tm tt;
		strTime = strTemp.substr(0, 4);
		tt.tm_year=atoi(strTime.c_str())-1900;

		strTime = strTemp.substr(4, 2);
		tt.tm_mon=atoi(strTime.c_str())-1;

		strTime = strTemp.substr(6, 2);
		tt.tm_mday=atoi(strTime.c_str());

		strTime = strTemp.substr(8, 2);
		tt.tm_hour=atoi(strTime.c_str());

		strTime = strTemp.substr(10, 2);
		tt.tm_min=atoi(strTime.c_str());

		strTime = strTemp.substr(12, 2);
		tt.tm_sec=atoi(strTime.c_str());
		tt.tm_isdst=0;
		time_t dwTime = mktime(&tt);

		return dwTime;
	}
	
	void StrTimestamp(char* timeBuf, const int bufMaxSize, time_t * pTime)
	{
		if(NULL == timeBuf || NULL == pTime)
		{
			return;
		}
		if (*pTime < 0)
		{
			return;	
		}
		struct tm * tTime = localtime(pTime);
		strftime(timeBuf, bufMaxSize, "%Y-%m-%d %H:%M:%S", tTime);
	}

	u32 StrToHexUL(const s8* pszStr)
	{
		if(NULL == pszStr)
		{
			return -1;
		}

		s8 * pszStop;
		return strtoul(pszStr, &pszStop, 16);
	}

	u32 StrToOctUL(const s8* pszStr)
	{
		if(NULL == pszStr)
		{
			return -1;
		}

		s8 * pszStop;
		return strtoul(pszStr, &pszStop, 10);
	}

	u64 StrToHexULL(const s8* pszStr)
	{
		if(NULL == pszStr)
		{
			return -1;
		}

		s8 * pszStop;
		return _strtoui64(pszStr, &pszStop, 16);
	}

	u64 StrToOctULL(const s8* pszStr)
	{
		if(NULL == pszStr)
		{
			return -1;
		}

		s8 * pszStop;
		return _strtoui64(pszStr, &pszStop, 10);
	}

	size_t ParseString( const CspString& str, const s8* szToken, s32 nTokenLen, std::vector<CspString>& vec )
	{
		CspString strRecord;
		size_t nLastPos = 0;
		size_t nPos = str.find(szToken, 0);
		while(nPos != CspString::npos)
		{
			strRecord = str.substr(nLastPos, nPos - nLastPos);
			vec.push_back(strRecord);
			nLastPos = nPos + nTokenLen;
			nPos = str.find(szToken, nLastPos);
		}

		if (nLastPos != str.size())
		{
			strRecord = str.substr(nLastPos, str.size()-nLastPos);
			vec.push_back(strRecord);
		}

		return vec.size();
	}

	CspString IpU32ToStr( const u32 dwIp )
	{
		CspString str;
		IpU32ToStr(dwIp, str);
		return STD_MOVE(str);
	}

	void IpU32ToStr( const u32 dwIp, CspString &str )
	{
		// 		struct in_addr inaddr;
		// 		inaddr.s_addr = dwIp;
		// 		str = inet_ntoa(inaddr);
		str.format("%d.%d.%d.%d", 
			((dwIp) >> 0  ) & 0xFF, 
			((dwIp) >> 8  ) & 0xFF, 
			((dwIp) >> 16 ) & 0xFF, 
			((dwIp) >> 24 ) & 0xFF); 
	}

	u32 IpStrToU32( const s8* str )
	{
		return inet_addr(str);
	}

	void ComIntToStr( const u16 wPort, CspString &str )
	{
		str = ComIntToStr(wPort);
	}

	CspString ComIntToStr(const u16 wPort)
	{
		CspString str;
		if (wPort<10)
		{
			str.format("COM%d", wPort);
		}
		else
		{
			str.format("\\\\.\\COM%d", wPort);
		}

		return STD_MOVE(str);
	}

	bool IsStrValid( const s8* pszStr )
	{
#if UNICODE
		return pszStr != NULL && wcslen(pszStr) != 0;
#else
		return pszStr != NULL && strlen(pszStr) != 0;
#endif
	}

#ifdef CSP_OS_FAMILY_WINDOWS
	//字符 转码
	std::string UTF16_2_UTF8( const std::wstring& strUTF16 )
	{
		int nUTF8Length = ::WideCharToMultiByte(CP_UTF8,
			0,
			strUTF16.c_str(),
			-1,
			NULL,
			0,
			0,0);

		std::string strUTF8(nUTF8Length+1,'\0');
		int nRet = ::WideCharToMultiByte(CP_UTF8,
			0,
			strUTF16.c_str(),
			-1,
			&strUTF8[0],
			nUTF8Length+1,
			0,
			0);


		return strUTF8;
	}

	std::wstring UTF8_2_UTF16( const std::string& strUTF8 )
	{
		int nUTF16Length = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,NULL,0);

		nUTF16Length += 1;
		std::wstring strUTF16(nUTF16Length ,'\0');

		int nRet = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,
			&strUTF16[0],nUTF16Length);
		_ASSERT(0 != nRet);

		return strUTF16;
	}

	std::string UTF16_2_ANSI( const std::wstring& strUTF16 )
	{
		int nANSILength = ::WideCharToMultiByte(CP_ACP,0,strUTF16.c_str(),-1,NULL,0,0,0);

		std::string strANSI(nANSILength,' ');
		int nRet = ::WideCharToMultiByte(CP_ACP,0,strUTF16.c_str(),-1,&strANSI[0],nANSILength,0,0);
		_ASSERT(0 != nRet);
		return strANSI;
	}

	std::wstring ANSI_2_UTF16( const std::string& strANSI )
	{
		int nUnicodeLength = ::MultiByteToWideChar(CP_ACP,0,strANSI.c_str(),-1,NULL,0)	;

		std::wstring strUTF16(nUnicodeLength,' ');
		int nRet = ::MultiByteToWideChar(CP_ACP,0,strANSI.c_str(),-1,&strUTF16[0],nUnicodeLength);
		_ASSERT(0 != nRet);

		return strUTF16;
	}

	std::string UTF8_2_ANSI( const std::string& strUTF8 )
	{
		std::wstring wstrUTF16 = UTF8_2_UTF16(strUTF8);
		std::string  strANSI   = UTF16_2_ANSI(wstrUTF16);

		return strANSI;
	}

	std::string ANSI_2_UTF8( const std::string& strANSI )
	{
		std::wstring wstrUTF16 = ANSI_2_UTF16(strANSI);
		std::string strUTF8    = UTF16_2_UTF8(wstrUTF16);

		return strUTF8;
	}
#endif //CSP_OS_FAMILY_WINDOWS

	void PrintfDataBuff( const u8* pszData, const s32 len, const s8* pDesc /*= "DataBuffBag"*/ )
	{
		if (NULL == pszData || NULL == pDesc){
			return;
		}

		CspString strOut;
		CspString strTemp;
		strOut.format("[%s]: size: %d bytes. \n{", pDesc, len);
		for (s32 i =0; i<len; ++i) {
			if (i%10 == 0) {
				strOut += "\n";
			}

			strTemp.format("%02X ", (u8)pszData[i]);
			strOut += strTemp;
		}
		strOut += "\n}\n";
		//ILog("PrintfDataBuff") << strOut;
	}

	void PrintfDataBuff( const s8* pszData, const s32 len, const s8* pDesc /*= "DataBuffBag"*/ )
	{
		PrintfDataBuff((u8*) pszData, len, pDesc);
	}

	void PrintfDataBuff( const TDataBuff &tDataBuff, const s8* pDesc /*= "DataBuffBag"*/ )
	{
		PrintfDataBuff(tDataBuff.GetBuff(), tDataBuff.GetBuffLen(), pDesc);
	}

	//获取系统最后一次错误字符串的函数
	void GetSysLastErrorStr(u32& dwErr, CspString &strErr)
	{
#ifdef CSP_OS_FAMILY_WINDOWS
		CspChar buffer[255] = {0};
		dwErr = GetLastError();
		::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buffer, sizeof(buffer), NULL);

		strErr = buffer;
#else
		strErr = dlerror();
#endif
	}

	CspString GetSysLastErrorStr()
	{
		u32 dwErr = 0;
		CspString strErr;
		GetSysLastErrorStr(dwErr, strErr);

		return strErr;
	}

	const s8* const_basename(const char* filepath) 
	{
		const s8* base = strrchr(filepath, '/');
#ifdef CSP_OS_FAMILY_WINDOWS  // Look for either path separator in Windows
		if (!base)
			base = strrchr(filepath, '\\');
#endif
		return base ? (base+1) : filepath;
	}

	CspString GetCurrentPath()
	{
#ifdef CSP_OS_FAMILY_WINDOWS
		s8 szAppPath[MAX_PATH] = {0};
		::GetModuleFileName(NULL, szAppPath, MAX_PATH);
		CspString strFilePath = szAppPath;
		int nPos = strFilePath.rfind('\\');
		strFilePath = strFilePath.substr(0, nPos + 1);
#else
		CspString strFilePath(Poco::Path::current());
#endif
		return strFilePath;
	}

	CspString GetLocalIp()
	{
		in_addr inaddr;
		s8 szTmp[200] = {0};
		gethostname(szTmp, 200);
		struct  hostent* ph;
		ph = gethostbyname(szTmp);
		memcpy(&inaddr, ph->h_addr_list[0], sizeof(struct in_addr));
		CspString strIp;
		const s8* pszIp = inet_ntoa(inaddr);
		strIp = pszIp;
		return strIp;
	}

	u64 MakeDWord(u32 dwHight, u32 dwLow)
	{
		u64 qVal = dwHight;
		qVal = qVal << 32;
		qVal += dwLow;
		return qVal;
	}

	void string_trim(std::string &str, const s8 szToken)
	{
		std::string::size_type pos = str.find(szToken);
		while (pos != std::string::npos)
		{
			str.erase(pos, 1);
			pos = str.find(szToken);
		}
	}

	void string_trim(std::string &str, const s8* szToken, s32 nTokenLen)
	{
		s32 nCount = 0;
		while(nCount < nTokenLen)
		{
			string_trim(str, szToken[nCount]);
			++nCount;
		}
	}
}