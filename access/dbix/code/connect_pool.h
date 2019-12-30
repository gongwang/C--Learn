/*========================================================================================
ģ����    ��dbix.lib
�ļ���    ��connect_pool.h
����ļ�  ��
ʵ�ֹ���  �����ӹ����ࡣ
����      ��huzhiyun
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/06/08         V1.0              huzhiyun                              �½��ļ�
=========================================================================================*/
#ifndef _CONNECT_POOL_H_
#define _CONNECT_POOL_H_
#include "inner_common.h"
#include "sql_layer.h"

DBIX_NS_BEGIN

struct TDBConnInfo
{
    TDBConnInfo(): pcDBConn(NULL), bAssigned(false)
    {
    }
    TDBConnInfo(CDBConnection* pcDBConn_, bool bAssigned_)
        : pcDBConn(pcDBConn_), bAssigned(bAssigned_)
    {
    }

    CDBConnection* pcDBConn; // ������Ϣ
    bool bAssigned;          // �Ƿ����
};

class CConnectPool
{
public:
    CConnectPool():m_hDBConnsLock(NULL), m_dwDBConnectMaxNum(0){}
    bool Init(u32 dwDBConnectMaxNum, const TDBConnectCfg &tDBConnDefaultCfg);
    void Quit();
    void Dump();

    CDBConnection* AllocateDBConnection(const TDBConnectCfg* ptCfg = NULL);
    void DeallocateDBConnection(const CDBConnection *pcDBConn);

private:
    vector<TDBConnInfo> m_cDBConns;
    SEMHANDLE m_hDBConnsLock;

    TDBConnectCfg m_tDBConnectDefaultCfg;
    u32 m_dwDBConnectMaxNum;
};

DBIX_NS_END
#endif // _CONNECT_POOL_H_
