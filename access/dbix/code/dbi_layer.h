/*========================================================================================
ģ����    ��dbix.lib
�ļ���    ��dbix_layer.h
����ļ�  ��
ʵ�ֹ���  ����װdbi�ĵ��úͶ�������ȡ�
����      ��huzhiyun
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/05/09         V1.0              huzhiyun                              �½��ļ�
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

    const s8* GetString(const s8* field);  // ȡ�꼴��ʹ��, �ַ������ܱȽϳ����ʲ���ʽ������ʱ����
    CCMSTime GetCmsDate(const s8* field);
    s32 GetInt(const s8* field);
    bool GetBool(const s8* field);
    double GetDouble(const s8* field);

    bool MoveNext(); // ����false��ʾ�ƶ�ʱ����ע�⣬��ǰ�������һ��ʱ���ƶ�Ҳ�������
    CDBConnection::ENMultiRsProcCode NextResultset(); // ��ȡ�¸����������

private:
    static void DBClientCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);
    static void DBServerCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext);
private:
    CDBHandler  m_cDBOprer;
    CDBRs  m_cDBRs;
    bool m_bConnected;

private:
    s8 __szFieldVal[1024*30]; // �ֶ�ֵ��ʱ��ű������ַ����ֶ���󳤶ȣ�20120608�ղ���dbiʱ����30k����������
};

DBIX_NS_END
#endif // _DBI_LAYER_H_
