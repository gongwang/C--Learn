/********************************************************************
	created:	2013/09/12 13:25 
	file base:	CspString.h
	author:		Bing
	
	purpose:	Define our customed CspString from std::string or std::wstring
*********************************************************************/
#pragma once

#include "csp_bulid_config.h"
#include "kdvtype.h"
#include <string>		// std::basic_string
#include <sstream>      // std::basic_stringstream && std::basic_istringstream

namespace dascommon{

	#ifdef _UNICODE
		//template class CSP_API	std::allocator<wchar_t>;//for warning4251
		typedef wchar_t				CspChar;
	#else
		//template class CSP_API	std::allocator<char>;	//for warning4251
		typedef char				CspChar;
	#endif // Unicode

	typedef std::basic_string<CspChar, std::char_traits<CspChar>, 
		std::allocator<CspChar> > StdString;

	typedef std::basic_stringstream<CspChar, std::char_traits<CspChar>, 
		std::allocator<CspChar> > StdStringStream;

	typedef std::basic_istringstream<CspChar, std::char_traits<CspChar>,
		std::allocator<CspChar> > StdIStringStream;

	class CspString : public StdString
	{
	public:
		CspString();
		CspString(const StdString& str);
		CspString(const CspString& str);
		CspString(const CspChar* pstr);
		CspString(const CspChar* pstr, size_type _Count);
		~CspString(void);

		// replace strsrc of this CspString with strdst 
		void replace(const CspString &strsrc, const CspString &strdst);

		// format this CspString
		// The max string len = MAX_FORMAT_LEN = 4096
		void format(const CspChar* lpszFormat, ...);

		void trim(const CspChar szToken);
		void trim(const CspChar* pszToken, size_t nTokenLen);

		//比较字符串不区分大小写
		s32 compare_nocase(const CspString& strsrc);

		u32 Convert2U32(size_type _Radix = 10) const;
		u64 Convert2U64(size_type _Radix = 10) const;
		s32 Convert2S32() const;
		s64 Convert2S64() const;
		double Convert2Double() const;
		// 2012-08-16 00:00:00 to time_t
		time_t Convert2Time() const;
		// 2012-08-16 to time_t
		time_t Convert2Date() const;

#ifdef CSP_OS_FAMILY_WINDOWS
		static CspString ToString(s32 value, size_type _Radix = 10);
		static CspString ToString(u32 value, size_type _Radix = 10);
		static CspString ToString(u64 value, size_type _Radix = 10);
		static CspString ToString(s64 value, size_type _Radix = 10);
#endif
		template < class T >
		static CspString ToString( const T& value )
		{
			StdStringStream convert;
			convert << value;
			if ( convert.fail() )
			{
				//TICPPTHROW( "Could not convert "<< value <<" to text" );
				return "";
			}
			return convert.str();
		}

		static CspString ToString( const CspString& value )
		{
			return value;
		}

		/**
		Converts a std::string to any class with a proper overload of the >> opertor
		@param temp			The string to be converted
		@param out	[OUT]	The container for the returned value
		@throws Exception When temp cannot be converted to the target type
		*/
		template < class T >
		static void FromString( CspString& temp, T* out )
		{
			StdIStringStream val( temp );
			val >> *out;

			if ( val.fail() )
			{
				//TICPPTHROW( "Could not convert " << temp << " to target type" );
			}
		}

		/**
		Specialization for static CspString
		*/
		static void FromString( CspString& temp, CspString* out )
		{
			*out = temp;
		}

		CspString& operator=(const StdString& _Right);
		CspString& operator=(const CspString& _Right);
		CspString& operator=(const CspChar *_Ptr);
		CspString& operator=(CspChar _Ch);
		
		CspString& operator-(const CspString& _Right); 
		CspString& operator-=(const CspString& _Right);
	};


	inline CspString& dascommon::CspString::operator=(const StdString& _Right)
	{	// assign _Right
		StdString::operator=(_Right);
		return (*this);
	}

	inline CspString& dascommon::CspString::operator=(const CspString& _Right)
	{	// assign _Right
		StdString::operator=(_Right);
		return (*this);
	}

	inline CspString& dascommon::CspString::operator=(const CspChar *_Ptr)
	{	// assign [_Ptr, <null>)
		StdString::operator=(_Ptr);
		return (*this);
	}

	inline CspString& dascommon::CspString::operator=(CspChar _Ch)
	{	// assign 1 * _Ch
		StdString::operator=(_Ch);
		return (*this);
	}

	inline CspString& dascommon::CspString::operator-(const CspString& _Right)
	{
		size_type i = find(_Right); 
		if(i != npos)  
			erase(i, _Right.size()); 
		return (*this); 
	}

	inline CspString& dascommon::CspString::operator-=(const CspString& _Right)
	{
		size_type i = find(_Right); 
		if(i != npos)  
			erase(i, _Right.size()); 
		return (*this); 
	}
	
} //namespace dascommon

