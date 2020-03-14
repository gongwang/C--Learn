/*========================================================================================
模块名    ：dbix.lib
文件名    ：connect_pool.h
相关文件  ：
实现功能  ：连接管理类。
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/06/08         V1.0              huzhiyun                              新建文件
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

    CDBConnection* pcDBConn; // 连接信息
    bool bAssigned;          // 是否分配
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
