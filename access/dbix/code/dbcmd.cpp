#if defined(WIN32) && (_MSC_VER <= 1200)
#pragma warning(disable : 4786)
#endif

#include "inner_common.h"

DBIX_NS_BEGIN

ENDBResult CDBCmd::ExecImp( CDBConnection *pcDBConnection, CDbResultDataBase* &pcResultData )
{
    string strSql;
    GenerateExecProcSql(strSql, m_strCmdName, m_tParmValues, pcDBConnection->ConnectCfg().enDbType);

    pcResultData = NULL;
    return pcDBConnection->ExecSQL(strSql, CDBConnection::enExecNoRet)? enDBResultSuccess: enDBResultFail;
}

DBIX_NS_END
