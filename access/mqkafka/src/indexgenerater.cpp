#include "indexgenerater.h"
#include <algorithm>
CIndexGenerater::CIndexGenerater() : m_dwCurIndex(1), m_dwMaxIndex(0)
{}
CIndexGenerater::~CIndexGenerater()
{
    m_vecUsedIdxArr.clear();
}
BOOL32 CIndexGenerater::Init(u32 dwMaxIndex)
{
    if (dwMaxIndex < 1){return FALSE;}
    m_dwMaxIndex = dwMaxIndex;
    return TRUE;
}
//��������
u32 CIndexGenerater::GenIndex()
{
    u32 dwPollCnt = 0;
    u32 dwRetNO = 0;
    while ( dwPollCnt <= m_dwMaxIndex )
    {
        std::vector<u32>::iterator iteIdx = find(m_vecUsedIdxArr.begin(), m_vecUsedIdxArr.end(),m_dwCurIndex);
        if (iteIdx == m_vecUsedIdxArr.end())
        {
            dwRetNO = m_dwCurIndex;
            m_vecUsedIdxArr.push_back(m_dwCurIndex);
            NextIndex();   //������m_dwCurIndex��������
            return dwRetNO;
        }
        NextIndex();
        ++dwPollCnt;
    }
    return 0;
}
//�ͷ�����
void CIndexGenerater::RevokeIndex(const u32 dwIndex)
{
    if ( dwIndex <= m_dwMaxIndex)
    {
        std::vector<u32>::iterator iteIdx = find(m_vecUsedIdxArr.begin(), m_vecUsedIdxArr.end(),dwIndex);
        if (iteIdx != m_vecUsedIdxArr.end())
        {
            m_vecUsedIdxArr.erase(iteIdx);
        }
    }
}
//��ȡ�ѷ�����������
u32 CIndexGenerater::GetIndexCount()
{
    return m_vecUsedIdxArr.size();
}

u32 CIndexGenerater::NextIndex()
{
    //ȡģ�������õ���0~(m-1)��ֵ�����˴�Ҫ�õ���1~m��ֵ��������ģֵ����Ҫ��1
    m_dwCurIndex = ++m_dwCurIndex % (m_dwMaxIndex+1);
    if ( 0 == m_dwCurIndex )
    {
        ++m_dwCurIndex;
    }
    return m_dwCurIndex;
}
