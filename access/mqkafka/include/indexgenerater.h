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
    //分配索引
    u32 GenIndex();
    //释放索引
    void RevokeIndex(const u32 dwIndex);
    //获取已分配索引个数
    u32 GetIndexCount();

protected:
    u32 NextIndex();
private:
    u32 m_dwMaxIndex;              //最大的索引号
    u32 m_dwCurIndex;              //当前待分配的索引号
    std::vector<u32> m_vecUsedIdxArr;//已分配列表
};
#endif //_INDEX_GENERATRE_H_