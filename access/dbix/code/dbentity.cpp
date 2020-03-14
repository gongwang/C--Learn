#if defined(WIN32) && (_MSC_VER <= 1200)
#pragma warning(disable : 4786)
#endif

#include "inner_common.h"

DBIX_NS_BEGIN

static ENDBResult GenerateEntityQrySql(TSqlString &tSql, const CDBEntitySchema &cSchema, const CDBCriteria &cCond, 
                                       const TOrderby &tOrderby, u32 dwEntityIndex, u32 dwEntityNum, ENDBType enDbType)
{
    u32 i;
    tSql.Format("select ");
    for (i = 0; i < cSchema.ColumnInfos().size(); ++i)  // 相比直接用select *, 可以检测cSchema中列名的正确性；另外有资料说相对select *性能会提升
    {
        tSql.AppendString("%s,", cSchema.ColumnInfos()[i].colName.c_str());
    }
    tSql.EraseLastRedundantComma();

    tSql.AppendString(" from %s", cSchema.TableName().c_str());
    if (!cCond.empty())
    {
        tSql.AppendString(" where %s", cCond.c_str());
    }
    if (!tOrderby.empty())
    {
        tSql.AppendString(" order by ");

        for (i = 0; i < tOrderby.size(); ++i)
        {
            tSql.AppendString(" %s %s,", tOrderby[i].colName.c_str(), (tOrderby[i].asc? "asc":"desc"));
        }

        tSql.EraseLastRedundantComma();
    }

    if (dwEntityIndex != (u32)-1)
    {
        if (enDBTypeMysql == enDbType || enDBTypeSqlite == enDbType)
        {
            tSql.AppendString(" limit %u,%u", (unsigned int)dwEntityIndex, (unsigned int)dwEntityNum);
        }
        else
        {
            assert(false && "not support currently");
            return enDBResultFail; // hzytodo
        }
    }

    return enDBResultSuccess;
}

ENDBResult AutoFetchResultset(CDBConnection *pcDBConnection, const CDBEntitySchema &cSchema, CDBEntityValue* &pResultData)
{
    ENDBResult enDbResult = enDBResultSuccess;

    CDBEntityValue *pcEntityValue = new CDBEntityValue(cSchema);

    const vector<TColInfo>& tColInfos = cSchema.ColumnInfos();
    while (pcDBConnection->IsNotEof())
    {
        pcEntityValue->AppendRow();

        for (u32 i = 0; i < tColInfos.size(); ++i)
        {
            if (enDBDataTypeString == tColInfos[i].colDataType)
            {
                pcEntityValue->SetColumnVal(pcDBConnection->GetString(tColInfos[i].colName.c_str()));
            }
            else if (enDBDataTypeS32 == tColInfos[i].colDataType)
            {
                pcEntityValue->SetColumnVal(pcDBConnection->GetInt(tColInfos[i].colName.c_str()));
            }
            else if (enDBDataTypeDate == tColInfos[i].colDataType)
            {
                pcEntityValue->SetColumnVal(pcDBConnection->GetCmsDate(tColInfos[i].colName.c_str()));
            }
            else if (enDBDataTypeBool == tColInfos[i].colDataType)
            {
                pcEntityValue->SetColumnVal(pcDBConnection->GetBool(tColInfos[i].colName.c_str()));
            }
            else if (enDBDataTypeDouble == tColInfos[i].colDataType)
            {
                pcEntityValue->SetColumnVal(pcDBConnection->GetDouble(tColInfos[i].colName.c_str()));
            }
            else
            {
                log(enLOGLVL_ERROR, "not supported data type");
                enDbResult = enDBResultFail;
                break;
            }
        }
        if (enDBResultSuccess != enDbResult)
        {
            break;
        }

        if (!pcDBConnection->MoveNext())
        {
            enDbResult = enDBResultFail;
            break;
        }
    }
    if (enDBResultSuccess == enDbResult)
    {
        pResultData = pcEntityValue;
        return enDBResultSuccess;
    }
    else
    {
        delete pcEntityValue;
        pResultData = NULL;
        return enDbResult;
    }
}

static ENDBResult GenerateEntityAddSql(TSqlString &tSql, const CDBEntitySchema &cSchema, const map<TColName, Variant> &tColValues)
{
    ENDBResult enDbResult = enDBResultSuccess;

    tSql.Format("insert into %s(", cSchema.TableName().c_str());

    CMapColVal::const_iterator it;

    const vector<TColInfo>& tColInfos = cSchema.ColumnInfos();
    u32 dwIdx = 0;
    for (dwIdx = 0; dwIdx < tColInfos.size(); ++dwIdx)
    {
        it = tColValues.find(tColInfos[dwIdx].colName);
        if (it != tColValues.end())
        {
            tSql.AppendString("%s,", tColInfos[dwIdx].colName.c_str());
        }
    }
    tSql.EraseLastRedundantComma();
    tSql.AppendString(") values(");

    for (dwIdx = 0; dwIdx < tColInfos.size(); ++dwIdx)
    {
        it = tColValues.find(tColInfos[dwIdx].colName);
        if (it != tColValues.end())
        {
            tSql.AppendString("%s,", DBData(it->second, tColInfos[dwIdx].colDataType).c_str());
        }
    }
    tSql.EraseLastRedundantComma();
    tSql.AppendString(")");

    return enDbResult;
}

static ENDBResult GenerateEntityDelSql(TSqlString &tSql, const CDBEntitySchema &cSchema, const CDBCriteria &cConditions)
{
    tSql.Format("delete from %s", cSchema.TableName().c_str());
    if (!cConditions.empty())
    {
        tSql.AppendString(" where %s", cConditions.c_str());
    }

    return enDBResultSuccess;
}

static ENDBResult GenerateEntityModSql(TSqlString &tSql, const CDBEntitySchema &cSchema, const map<TColName, Variant> &tColValues, 
                                       const CDBCriteria &cConditions)
{
    ENDBResult enDbResult = enDBResultSuccess;

    tSql.Format("update %s set ", cSchema.TableName().c_str());

    ENDBDataType enDbDataType;

    for (CMapColVal::const_iterator it = tColValues.begin(); it != tColValues.end(); ++it)
    {
        if (cSchema.ColumnDBDataType(it->first, enDbDataType))
        {
            tSql.AppendString("%s=%s,", it->first.c_str(), DBData(it->second, enDbDataType).c_str());
        }
        else
        {
            log(enLOGLVL_ERROR, "GenerateEntityModSql detected not existted column's value");
            enDbResult = enDBResultFail;
            break;
        }
    }
    tSql.EraseLastRedundantComma();

    if (!cConditions.empty())
    {
        tSql.AppendString(" where %s", cConditions.c_str());
    }

    return enDbResult;
}

ENDBResult CDBEntity::ExecImp( CDBConnection *pcDBConnection, CDbResultDataBase* &pcResultData )
{
    ENDBResult enDbResult = enDBResultFail;
    pcResultData = NULL;

    do 
    {
        TSqlString tSql;

        if (enOperateQry == m_enOperate)
        {
            enDbResult = GenerateEntityQrySql(tSql, m_cSchema, m_cConditions, m_tOrderby, m_dwEntityIndex, m_dwEntityNum, pcDBConnection->ConnectCfg().enDbType);
            if (enDBResultSuccess != enDbResult)
            {
                break;
            }

            if (!pcDBConnection->ExecSQL(tSql, CDBConnection::enExecMayRet))
            {
                enDbResult = enDBResultFail;
                break;
            }

            CDBEntityValue *pcResultDataTmp = NULL;
            enDbResult = AutoFetchResultset(pcDBConnection, m_cSchema, pcResultDataTmp);
            if (enDBResultSuccess == enDbResult)
            {
                pcResultData = pcResultDataTmp;
            }
            else
            {
                break;
            }
        }
        else if (enOperateAdd == m_enOperate)
        {
            if (!m_cConditions.empty())
            {
                enDbResult = GenerateEntityDelSql(tSql, m_cSchema, m_cConditions);
                if (enDBResultSuccess != enDbResult)
                {
                    break;
                }
                if (!pcDBConnection->ExecSQL(tSql, CDBConnection::enExecNoRet))
                {
                    enDbResult = enDBResultFail;
                    break;
                }
            }

            enDbResult = GenerateEntityAddSql(tSql, m_cSchema, m_tColValues);
            if (enDBResultSuccess != enDbResult)
            {
                break;
            }

            if (!pcDBConnection->ExecSQL(tSql, CDBConnection::enExecNoRet))
            {
                enDbResult = enDBResultFail;
                break;
            }
        }
        else if (enOperateDel == m_enOperate)
        {
            enDbResult = GenerateEntityDelSql(tSql, m_cSchema, m_cConditions);
            if (enDBResultSuccess != enDbResult)
            {
                break;
            }
            if (!pcDBConnection->ExecSQL(tSql, CDBConnection::enExecNoRet))
            {
                enDbResult = enDBResultFail;
                break;
            }
        }
        else if (enOperateMod == m_enOperate)
        {
            enDbResult = GenerateEntityModSql(tSql, m_cSchema, m_tColValues, m_cConditions);
            if (enDBResultSuccess != enDbResult)
            {
                break;
            }
            if (!pcDBConnection->ExecSQL(tSql, CDBConnection::enExecNoRet))
            {
                enDbResult = enDBResultFail;
                break;
            }
        }
        else
        {
            log(enLOGLVL_ERROR, "CDBEntity::ExecImp detected invalid operate type");
            break;
        }

        enDbResult = enDBResultSuccess;
    } while (false);

    return enDbResult;
}

DBIX_NS_END
