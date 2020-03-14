/*========================================================================================
模块名    ：dbix.lib
文件名    ：dbix_layer.h
相关文件  ：
实现功能  ：封装dbi的调用和断链处理等。
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/05/09         V1.0              huzhiyun                              新建文件
=========================================================================================*/
#ifndef _DBI_LAYER_H_
#define _DBI_LAYER_H_
#include "dbinterface.h"
#include "dbix_util.h"
#include "inner_common.h"

DBIX_NS_BEGIN

bool DBIInit(ENDBType enDBType);
void DBIExit();

class CDBILayer
{
public:
    CDBILayer();
    ~CDBILayer();

    bool Connect(const TDBConnectCfg &tCfg);
    void Disconnect();
    bool IsConnect();

    bool ExecSql(const s8* cstrSql, bool bNeedRs);
    u32 GetAffectedRowCount();
    bool IsNotEof();

    const s8* GetString(const s8* field);  // 取完即刻使用, 字符串可能比较长，故不显式生成临时对象
    CCMSTime GetCmsDate(const s8* field);
    s32 GetInt(const s8* field);
    bool GetBool(const s8* field);
    double GetDouble(const s8* field);

    bool MoveNext(); // 返回false表示移动时出错，注意，当前处于最后一行时再移动也是允许的
    CDBConnection::ENMultiRsProcCode NextResultset(); // 获取下个结果集数据

private:
    static void DBClientCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);
    static void DBServerCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);
private:
    CDBHandler  m_cDBOprer;
    CDBRs  m_cDBRs;
    bool m_bConnected;

private:
    s8 __szFieldVal[1024*30]; // 字段值临时存放变量；字符串字段最大长度，20120608日测试dbi时发现30k是它的限制
};

DBIX_NS_END
#endif // _DBI_LAYER_H_
