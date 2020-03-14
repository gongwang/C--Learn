#if WIN32 && (_MSC_VER <= 1200)
#pragma warning(disable : 4786)
#endif

#include "sql_layer.h"

DBIX_NS_BEGIN

std::string DBData( bool val )
{
    return val? "1":"0"; 
}

std::string DBData( s32 val )
{
    s8 szBuf[40];
    sprintf(szBuf, "%d", val);
    return szBuf; 
}

std::string DBData( double val )
{
    s8 szBuf[40];
    sprintf(szBuf, "%.15f", val);
    return szBuf; 
}

std::string DBData( const std::string &val )
{
    string strtmp(val);
    string strSrc="'";
    string strDst="\\'";
	if (enDBTypeSqlite == g_tDBIXConf.tDBConnectCfg.enDbType)
	{
		strDst="''";
	}
    
	_replace_str(strtmp, strSrc, strDst);

    strtmp.insert(strtmp.begin(), 1, '\'');
    strtmp.append(1, '\'');

    return strtmp;
}

std::string DBData( const s8* val )
{
    string strtmp;
    strtmp = val;
    return DBData(strtmp);
}

std::string DBData( const CCMSTime &val )
{
#ifdef WIN32
    s8 szBuf[21] = { 0 };
    return string(_i64toa(static_cast<u64>(val.GetTime()), szBuf, 10));
#else
    std::stringstream ssNum;
    ssNum << static_cast<u64>(val.GetTime());
    return ssNum.str();
#endif
}

// like语境下额外特殊字符的支持，不同数据库可能不同；此函数对除%,_,\外的特殊字符自动转义
std::string DBLikeString(const std::string &val, ENDBType enDBType)
{
    string strtmp = val;
    
    if (enDBTypeMysql == enDBType)
    {
        // none
    }
    else if (enDBTypeSybase == enDBType)
    {
        // ?
    }
    else if (enDBTypeOracle == enDBType)
    {
        // ?
    }
    else
    {
        log(enLOGLVL_ERROR, "DBLikeString detected not supported database type");
    }

    strtmp = DBData(strtmp);

    return strtmp; // hzytodo 不同数据库特殊符的自动转义
}

// 相比上面的函数，它使用初始化库时连接的数据库类型作为enDBType
std::string DBLikeString(const std::string &val)
{
    return DBLikeString(val, g_tDBIXConf.tDBConnectCfg.enDbType);
}

void GenerateExecProcSql(string &strSql, const string &strProcName, const vector<TVariantWrapper> &tParms, ENDBType enDbType)
{
    TSqlString tSql;

    if (enDBTypeMysql == enDbType)
    {
        tSql.Format("call %s(", strProcName.c_str());
        for (u32 i = 0; i < tParms.size(); ++i)
        {
            tSql.AppendString("%s", DBData(tParms[i].DataVal(), tParms[i].DataType()).c_str());
            tSql.AppendString(",");
        }

        tSql.EraseLastRedundantComma();
        tSql.AppendString(")");
    }
    else if (enDBTypeOracle == enDbType)
    {
        tSql.Format("exec %s(", strProcName.c_str());
        for (u32 i = 0; i < tParms.size(); ++i)
        {
            tSql.AppendString("%s", DBData(tParms[i].DataVal(), tParms[i].DataType()).c_str());
            tSql.AppendString(",");
        }

        tSql.EraseLastRedundantComma();
        tSql.AppendString(")");
    }
    else if (enDBTypeSybase == enDbType)
    {
        tSql.Format("exec %s ", strProcName.c_str());
        for (u32 i = 0; i < tParms.size(); ++i)
        {
            tSql.AppendString("%s", DBData(tParms[i].DataVal(), tParms[i].DataType()).c_str());
            tSql.AppendString(",");
        }

        tSql.EraseLastRedundantComma();
    }
    else
    {
        log(enLOGLVL_ERROR, "GenerateExecProcSql detected not supportted database type");
    }

    strSql = tSql.GetBuf();
}

DBIX_NS_END
