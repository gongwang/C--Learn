#if defined(WIN32) && (_MSC_VER <= 1200)
#pragma warning(disable : 4786)
#endif

#include "dbi_layer.h"

DBIX_NS_BEGIN

static void DBGlobalCBProc(u16 wErrNum, LPCSTR lpSrvMsg, void *pContext)
{
    if (NULL == lpSrvMsg)
    {
        return;
    }

    OspPrintf(TRUE, FALSE, "\n\nDBGlobalCBProc Start*******************************************\n");
    OspPrintf(TRUE, FALSE, "ErrNum:%d\n", wErrNum);
    OspPrintf(TRUE, FALSE, "SrvMsg:%s\n", lpSrvMsg);
    OspPrintf(TRUE, FALSE, "DBGlobalCBProc End*******************************************\n\n");
}

bool DBIInit( ENDBType enDBType )
{
    do 
    {
        if (!DBInit(static_cast<u8>(enDBType)))
        {
            log(enLOGLVL_ERROR, "DBInit failed");
            break;
        }
	#ifdef _LINUX_
		if (!SetDBLocale(DB_LANG_CHINESE, "utf8")) // 周忠敏修改 试用于linux版本
		{
			log(enLOGLVL_ERROR, "SetDbLocale failed\n");
			break;
		}	
	#else
		if (!SetDBLocale(DB_LANG_CHINESE, "gbk")) // 周忠敏修改 试用于windows版本
		{
			log(enLOGLVL_ERROR, "SetDbLocale failed\n");
			break;
		}
	#endif
        if (!SetupLibErrFuc(DBGlobalCBProc, NULL))
        {
            log(enLOGLVL_ERROR, "SetupLibErrFunc failed");
            break;
        }
        if (!SetDBTimeOut(20)) // 设置20秒的超时时间
        {
            log(enLOGLVL_ERROR, "SetDbTimeout failed");
            break;
        }

        return true;
    } while (false);

    return false;
}

void DBIExit()
{
    DBExit();
}

void CDBILayer::DBClientCBProc( u16 wErrNum, LPCSTR lpSrvMsg, void *pContext )
{
    if (NULL != pContext && 0 == wErrNum)
    {
        //断链检测
        CDBILayer* pcDBILayer = (CDBILayer*)pContext;
        pcDBILayer->m_bConnected = false;
        OspPrintf(TRUE, FALSE, "DBDisconnect detected\n");
    }
}

void CDBILayer::DBServerCBProc( u16 wErrNum, LPCSTR lpSrvMsg, void *pContext )
{
    if (NULL == lpSrvMsg)
    {
        return;
    }

    OspPrintf(TRUE, FALSE, "\n\nDBServerCBProc Start*******************************************\n");
    OspPrintf(TRUE, FALSE, "ErrNum:%d\n", wErrNum);
    OspPrintf(TRUE, FALSE, "SrvMsg:%s\n", lpSrvMsg);
    OspPrintf(TRUE, FALSE, "DBServerCBProc End*******************************************\n\n");
}

CDBILayer::CDBILayer()
{
    __szFieldVal[0] = '\0';
    m_bConnected = false;
}

CDBILayer::~CDBILayer()
{
    if (IsConnect())
    {
        Disconnect();
    }
}

bool CDBILayer::Connect(const TDBConnectCfg &tCfg)
{
    if (m_bConnected)
    {
        log(enLOGLVL_ERROR, "connect twice");
        return true;
    }

    m_bConnected = (TRUE == m_cDBOprer.ConnecttoSrv(tCfg.strDbUserName.c_str(), tCfg.strDbUserPassword.c_str(), 
        tCfg.strDbName.c_str(), tCfg.strDbServerIP.c_str(), tCfg.wDbServerPort));
    if (!m_bConnected)
    {
        log(enLOGLVL_WARN, "CDBILayer Connect database server failed, server ip:%s, server port:%d",
            tCfg.strDbServerIP.c_str(), (u32)tCfg.wDbServerPort);
    }

    return m_bConnected;
}

void CDBILayer::Disconnect()
{
    if (m_bConnected)
    {
        if (m_cDBOprer.CloseConnection())
        {
            m_bConnected = false;
        }
        else
        {
            log(enLOGLVL_ERROR, "connect disconnect failed");
        }
    }
}

bool CDBILayer::IsConnect()
{
    return m_bConnected;
}

bool CDBILayer::ExecSql( const s8* cstrSql, bool bNeedRs )
{
    if (!m_bConnected)
    {
        return false;
    }
    if (NULL == cstrSql)
    {
        log(enLOGLVL_WARN, "invalid argument for CDBILayer::ExecSql");
        return false;
    }

    log(enLOGLVL_DEBUG, "ExecSql begin, sql=%s\n", cstrSql);
    
    BOOL32 bRet = FALSE;
    if (bNeedRs)
    {
        bRet = m_cDBOprer.ExecSql(cstrSql, &m_cDBRs);
        if (bRet)
        {
            if (!m_cDBRs.MoveNext())
            {
                log(enLOGLVL_ERROR, "MoveNext failed in CDBILayer::ExecSql");
                bRet = FALSE;
            }
        }
    }
    else
    {
        bRet = m_cDBOprer.ExecSql(cstrSql);
    }

    log(enLOGLVL_DEBUG, "ExecSql end, sql=%s\n", cstrSql);

    return (bRet != FALSE);
}

u32 CDBILayer::GetAffectedRowCount()
{
    return m_cDBOprer.GetAffectedRowCount();
}

bool CDBILayer::IsNotEof()
{
    return !m_cDBRs.IsEof();
}

const s8* CDBILayer::GetString( const s8* field )
{
    BOOL32 bRet = m_cDBRs.GetFieldValue(field, __szFieldVal, sizeof(__szFieldVal));
    if (!bRet)
    {
        log(enLOGLVL_WARN, "CDBILayer::GetString failed");
        __szFieldVal[0] = '\0';
    }

    return __szFieldVal;
}

CCMSTime CDBILayer::GetCmsDate( const s8* field )
{
    CCMSTime cCmsTime;
    u64 qwVal;
    if (m_cDBRs.GetFieldValue(field, qwVal))
    {
        cCmsTime.SetTime(static_cast<time_t>(qwVal));
    }
    else
    {
        log(enLOGLVL_WARN, "CDBILayer::GetCmsDate failed");
    }

    return cCmsTime;
}

s32 CDBILayer::GetInt( const s8* field )
{
    s32 nVal;
    if (m_cDBRs.GetFieldValue(field, nVal))
    {
        return nVal;
    }
    else
    {
        log(enLOGLVL_WARN, "CDBILayer::GetInt failed");
        return 0;
    }
}

bool CDBILayer::GetBool( const s8* field )
{
    u8 byVal;
    if (m_cDBRs.GetFieldValue(field, byVal))
    {
        return (0 != byVal);
    }
    else
    {
        log(enLOGLVL_WARN, "CDBILayer::GetBool failed");
        return true;
    }
}

double CDBILayer::GetDouble( const s8* field )
{
    double dblVal;
    if (m_cDBRs.GetFieldValue(field, dblVal))
    {
        return dblVal;
    }
    else
    {
        log(enLOGLVL_WARN, "CDBILayer::GetDouble failed");
        return 0.0;
    }
}

bool CDBILayer::MoveNext()
{
    bool bRet = (TRUE == m_cDBRs.MoveNext());
    if (bRet)
    {
        return true;
    }
    else
    {
        log(enLOGLVL_WARN, "CDBILayer::MoveNext failed");
        return false;
    }
}

CDBConnection::ENMultiRsProcCode CDBILayer::NextResultset()
{
    CDBConnection::ENMultiRsProcCode enResult;

    EOpCode enRet = m_cDBOprer.GoOnProcRes();
    if (EOpCode_FindRs == enRet)
    {
        if (MoveNext())
        {
            enResult = CDBConnection::enMultiRsProcFoundNewResultset;
        }
        else
        {
            log(enLOGLVL_WARN, "CDBILayer::NextResultset movenext failed");
            enResult = CDBConnection::enMultiRsProcFailed;
        }
    }
    else if (EOpCode_Finish == enRet)
    {
        enResult = CDBConnection::enMultiRsProcNoNewResultset;
    }
    else
    {
        log(enLOGLVL_WARN, "CDBILayer::NextResultset failed");
        enResult = CDBConnection::enMultiRsProcFailed;
    }

    return enResult;
}

DBIX_NS_END
