#include "dasstring.h"

#include <cstdarg>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace dascommon;

CspString::CspString(void) : StdString()
{
}

CspString::CspString(const StdString& str): StdString(str)
{

}

CspString::CspString( const CspString& str ): StdString(str)
{

}

CspString::CspString( const CspChar* pstr ): StdString(pstr)
{

}

CspString::CspString( const CspChar* pstr, size_type _Count ): StdString(pstr, _Count)
{

}

CspString::~CspString(void)
{
}

void CspString::replace( const CspString &strsrc, const CspString &strdst )
{
	size_type pos=0;
	size_type srclen=strsrc.size();         
	size_type dstlen=strdst.size();       
	while((pos=find(strsrc, pos)) != npos)
	{ 
		StdString::replace(pos, srclen, strdst);                 
		pos += dstlen;         
	}
}

static const std::size_t MAX_FORMAT_LEN = 4096;
void CspString::format( const CspChar* lpszFormat, ... )
{
	va_list args;
	va_start(args, lpszFormat);

	CspChar szBuffer[MAX_FORMAT_LEN+1] = {0};

	vsnprintf(szBuffer, MAX_FORMAT_LEN, lpszFormat, args);
	perror(szBuffer);
	va_end(args);

	this->assign(szBuffer);
}

void CspString::trim( const CspChar szToken )
{
	size_type pos = find(szToken);
	while (pos != npos)
	{
		erase(pos, 1);
		pos = find(szToken);
	}
}

void CspString::trim( const CspChar* pszToken, size_t nTokenLen )
{
	size_type nCount = 0;
	while(nCount < nTokenLen)
	{
		trim(pszToken[nCount]);
		++nCount;
	}
}

s32 CspString::compare_nocase(const CspString& strsrc)
{
#ifdef CSP_OS_FAMILY_WINDOWS
	return strnicmp(this->c_str(), strsrc.c_str(), this->size());
#else
	return strncasecmp(this->c_str(), strsrc.c_str(), this->size());
#endif
}

u32 CspString::Convert2U32(size_type _Radix/* = 10*/) const
{
	CspChar * pszStop;
	return strtoul(this->c_str(), &pszStop, _Radix);
}

u64 CspString::Convert2U64(size_type _Radix/* = 10*/) const
{
	CspChar * pszStop;
#ifdef CSP_OS_FAMILY_WINDOWS
	return _strtoui64(this->c_str(), &pszStop, _Radix);
#else
	return strtoull(this->c_str(), &pszStop, _Radix);
#endif
}

s64 CspString::Convert2S64() const
{
#ifdef CSP_OS_FAMILY_WINDOWS
	return _atoi64(this->c_str());
#else
	return atoll(this->c_str());
#endif

}

s32 CspString::Convert2S32() const
{
	return atoi(this->c_str());
}

double CspString::Convert2Double() const
{
	return atof(this->c_str());
}

time_t CspString::Convert2Time() const
{
	CspString strTime;
	size_t posBegin = 0;
	size_t posEnd = find('-');
	if (posEnd == npos)
		return 0;

	struct tm tt;
	strTime = substr(posBegin, posEnd-posBegin);
	tt.tm_year=strTime.Convert2S32()-1900;

	posBegin = posEnd+1;
	posEnd = find('-');
	if (posEnd == npos)
		return 0;

	strTime = substr(posBegin, posEnd-posBegin);
	tt.tm_mon=strTime.Convert2S32()-1;

	posBegin = posEnd+1;
	posEnd = find('-');
	if (posEnd == npos)
		return 0;

	strTime = substr(posBegin, posEnd-posBegin);
	tt.tm_mday=strTime.Convert2S32();

	posBegin = posEnd+1;
	posEnd = find('-');
	if (posEnd == npos)
		return 0;

	strTime = substr(posBegin, posEnd-posBegin);
	tt.tm_hour=strTime.Convert2S32();

	posBegin = posEnd+1;
	posEnd = find('-');
	if (posEnd == npos)
		return 0;

	strTime = substr(posBegin, posEnd-posBegin);
	tt.tm_min=strTime.Convert2S32();

	posBegin = posEnd+1;
	posEnd = find('-');
	if (posEnd == npos)
		return 0;

	strTime = substr(posBegin, posEnd-posBegin);
	tt.tm_sec=strTime.Convert2S32();
	tt.tm_isdst=0;
	return mktime(&tt);
}

time_t CspString::Convert2Date() const
{
	CspString strTime;
	size_t posBegin = 0;
	size_t posEnd = find('-');
	if (posEnd == npos)
		return 0;

	struct tm tt;
	strTime = substr(posBegin, posEnd-posBegin);
	tt.tm_year=strTime.Convert2S32()-1900;

	posBegin = posEnd+1;
	posEnd = find('-');
	if (posEnd == npos)
		return 0;

	strTime = substr(posBegin, posEnd-posBegin);
	tt.tm_mon=strTime.Convert2S32()-1;

	posBegin = posEnd+1;
	posEnd = find('-');
	if (posEnd == npos)
		return 0;

	strTime = substr(posBegin, posEnd-posBegin);
	tt.tm_mday=strTime.Convert2S32();

	tt.tm_hour=0;
	tt.tm_min=0;
	tt.tm_sec=0;
	tt.tm_isdst=0;
	return mktime(&tt);
}

#ifdef CSP_OS_FAMILY_WINDOWS
CspString CspString::ToString(u32 value, size_type _Radix/* = 10*/)
{
	CspChar buffer[33] = {0}; //33bit+1
	ultoa(value, buffer, _Radix);
	return CspString(buffer);
}

CspString CspString::ToString(s32 value, size_type _Radix/* = 10*/)
{
	CspChar buffer[33] = {0}; //33bit+1
	ltoa(value, buffer, _Radix);
	return CspString(buffer);
}

CspString CspString::ToString(u64 value, size_type _Radix/* = 10*/)
{
	CspChar buffer[65] = {0}; //64bit+1
	_ui64toa_s(value, buffer, 65, _Radix);

	return CspString(buffer);
}

CspString CspString::ToString(s64 value, size_type _Radix/* = 10*/)
{
	CspChar buffer[65] = {0}; //64bit+1
	_i64toa_s(value, buffer, 65, _Radix);

	return CspString(buffer);
}
#endif //CSP_OS_FAMILY_WINDOWS

