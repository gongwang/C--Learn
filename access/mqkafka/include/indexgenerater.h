#ifndef _INDEX_GENERATRE_H_
#define _INDEX_GENERATRE_H_
#include "kdvtype.h"
#include <vector>
class CIndexGenerater
{
public:
    CIndexGenerater();
    ~CIndexGenerater();
    BOOL32 Init(u32 dwMaxIndex);
    //��������
    u32 GenIndex();
    //�ͷ�����
    void RevokeIndex(const u32 dwIndex);
    //��ȡ�ѷ�����������
    u32 GetIndexCount();

protected:
    u32 NextIndex();
private:
    u32 m_dwMaxIndex;              //����������
    u32 m_dwCurIndex;              //��ǰ�������������
    std::vector<u32> m_vecUsedIdxArr;//�ѷ����б�
};
#endif //_INDEX_GENERATRE_H_