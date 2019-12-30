/********************************************************************
	created:	2012/09/21 14:55 
	file base:	databuff.h
	author:		Milo
	
	purpose:	
*********************************************************************/

#pragma once
//#include "cspcommon/cspcommon_util.h"
#include "kdvtype.h"

//////////////////////////////////////////////////////////////////////////
#define  MAX_LEN_DATA_BUF		4096//80
/********************************************************************
	created:		2010/09/16 9:29 
	author:			MILO
	
	description:	数据缓存区
*********************************************************************/

template<int nMaxDataBuff = MAX_LEN_DATA_BUF>
class TDataBuffTemplate
{
public:
	enum{
		invalid_index = -1,
	};

	TDataBuffTemplate()
	{
		Clear();
	}
	
	TDataBuffTemplate(const TDataBuffTemplate& tRight)
	{
		*this = tRight;
	}
	
	void Clear()
	{
		m_wLen = 0;
 		memset(m_abyBuffer, 0, sizeof(m_abyBuffer));
	}
	
	TDataBuffTemplate& operator = (const TDataBuffTemplate& tRight)
	{
		if(this == &tRight)
		{
			return *this;
		}

		memcpy(this, &tRight, sizeof(TDataBuffTemplate));
		return *this;
	}

	//设置消息体
    void SetBuff(const void* pMsgBody, u16 wBodyLen)
    {
		//防止越界
		u16 wRealCpy = wBodyLen < MAX_LEN_DATA_BUF ? wBodyLen : MAX_LEN_DATA_BUF;

        memcpy(&m_abyBuffer, pMsgBody, wRealCpy);
        m_wLen = wRealCpy;

		m_abyBuffer[m_wLen] = '\0';
    }

	//在原有消息体之后附加一个消息体
    void CatBuff(const void* pMsgBody, u16 wBodyLen)
    {
		u16 wRealCat = wBodyLen + m_wLen < MAX_LEN_DATA_BUF ? wBodyLen : MAX_LEN_DATA_BUF-m_wLen;

        memcpy(&m_abyBuffer[m_wLen], pMsgBody, wRealCat);
        m_wLen += wRealCat;

		m_abyBuffer[m_wLen] = '\0';
    }

	//附加一个结构， 如果超出了长度。则覆盖以前的长度  overwrite
	void CatBuffOW(const void* pMsgBody, u16 wBodyLen)
    {
		u16 wRealCat = wBodyLen + m_wLen < MAX_LEN_DATA_BUF ? wBodyLen : MAX_LEN_DATA_BUF-m_wLen;
		if (wBodyLen + m_wLen < MAX_LEN_DATA_BUF)
		{
			SetBuff(pMsgBody, wBodyLen);
		}
		else
		{
			memcpy(&m_abyBuffer[m_wLen], pMsgBody, wRealCat);
			m_wLen += wRealCat;

			m_abyBuffer[m_wLen] = '\0';
		}
    }

	void TrimLeftBuff(u16 wLen)
	{
		if (wLen > m_wLen || wLen > nMaxDataBuff)
		{
			return;
		}
		
		m_wLen -= wLen;
		//u16 wRealMoveLen = nMaxDataBuff - wLen > wLen ? wLen :  nMaxDataBuff - wLen;
		memcpy(m_abyBuffer, (void *)(m_abyBuffer+wLen), m_wLen);

		m_abyBuffer[m_wLen] = '\0';
	}

	const u8* GetBuff( void ) const
    {
        return( ( const  u8* )( m_abyBuffer ) );
    }

	const u8 GetBuffAt(const u16 wPos) const 
	{
		if (wPos > m_wLen)
		{
			return 0;
		}

		return m_abyBuffer[wPos];
	}

	const u16 GetBuffPos(const u8 byBuf, u16 wBeginPos = 0) const 
	{
		const u16 wLastBuf = m_wLen > nMaxDataBuff ? nMaxDataBuff : m_wLen;
		const u16 wRealBein = wBeginPos > wLastBuf ? wLastBuf : wBeginPos;
		for (u16 index = wRealBein; index < wLastBuf; ++index)
		{
			if (byBuf == m_abyBuffer[index])
			{
				return index;
			}
		}
		
		return invalid_index;
	}
	
	//设置bufdata，如果wPos位置未初始化过，设置失败
	void SetBuffAt(const u16 wPos ,const u8 buff) 
	{
		if (wPos > m_wLen)
		{
			return;
		}
		
		m_abyBuffer[wPos] = buff;
	}
	
	//设置bufdata，如果wPos位置未初始化过，则在wPos位置重新设置
	void SetBuffAtEx(const u16 wPos ,const u8 buff) {
		if (wPos > nMaxDataBuff){
			return;
		}
		
		if (wPos >= m_wLen){
			m_wLen = wPos+1;
		}
		
		m_abyBuffer[wPos] = buff;
	}
	
	u16 GetBuffLen() const {
		return m_wLen;
	}
	
private:	
	u8 m_abyBuffer[nMaxDataBuff+1];		//数据
	u16 m_wLen;
};

//template class TDataBuffTemplate<MAX_LEN_DATA_BUF>; //for warning C4251
typedef TDataBuffTemplate<MAX_LEN_DATA_BUF> TDataBuff;
typedef TDataBuffTemplate<255> TDataBuff255;
typedef TDataBuffTemplate<128> TDataBuff128;