#if defined(WIN32) && (_MSC_VER <= 1200)
#pragma warning(disable : 4786)
#endif

#include "connect_pool.h"
#include "dbi_layer.h"

DBIX_NS_BEGIN
extern CConnectPool g_cConnectPool;

CDBConnection* AllocateDBConnection(const TDBConnectCfg* ptCfg)
{
    return g_cConnectPool.AllocateDBConnection(ptCfg);
}

void DeallocateDBConnection(const CDBConnection* pcDBConn)
{
    g_cConnectPool.DeallocateDBConnection(pcDBConn);
}

bool CConnectPool::Init( u32 dwDBConnectMaxNum, const TDBConnectCfg &tDBConnDefaultCfg )
{
    if (NULL != m_hDBConnsLock)
    {
        log(enLOGLVL_ERROR, "CConnectPool reinit");
        return false;
    }

    do 
    {
        if (!::OspSemBCreate(&m_hDBConnsLock))
            break;
        m_tDBConnectDefaultCfg = tDBConnDefaultCfg;
        m_dwDBConnectMaxNum = dwDBConnectMaxNum;

        return true;
    } while (false);
    
    Quit();
    return false;
}

void CConnectPool::Quit()
{
    if (m_hDBConnsLock != NULL)
    {
        for (u32 i = 0; i < m_cDBConns.size(); ++i)
        {
            if (!m_cDBConns[i].bAssigned)
            {
                m_cDBConns[i].pcDBConn->Disconnect();
            }
            else
            {
                // ���ӳ��˳�ʱδ��ȷ�ͷ�����
                log(enLOGLVL_DEBUG, "CConnectPool detected connection unreleased correctly");
            }
        }

        ::OspSemDelete(m_hDBConnsLock);
    }
}

CDBConnection* CConnectPool::AllocateDBConnection( const TDBConnectCfg* ptCfg /*= NULL*/ )
{
    CDBConnection* pcDBConn;

    OspSemTake(m_hDBConnsLock);

    do 
    {
        TDBConnectCfg tCfg = (ptCfg == NULL)? m_tDBConnectDefaultCfg : *ptCfg;
        u32 i;
        for (i = 0; i < m_cDBConns.size(); ++i)
        {
            if (!m_cDBConns[i].bAssigned && (m_cDBConns[i].pcDBConn->ConnectCfg() == tCfg))
            {
                break;
            }
        }

        if (i < m_cDBConns.size())
        {
            m_cDBConns[i].bAssigned = true;
            pcDBConn = m_cDBConns[i].pcDBConn;
            break;
        }

        if (m_cDBConns.size() < m_dwDBConnectMaxNum)
        {
            pcDBConn = new CDBConnection(tCfg);
            m_cDBConns.push_back(TDBConnInfo(pcDBConn, true));
        }
        else
        {
            pcDBConn = NULL;
            log(enLOGLVL_ERROR, "CConnectPool exceed connection number limitation");
        }
        
    } while (false);

    OspSemGive(m_hDBConnsLock);

    return pcDBConn;
}

void CConnectPool::DeallocateDBConnection( const CDBConnection *pcDBConn )
{
    OspSemTake(m_hDBConnsLock);

    u32 i;
    for (i = 0; i < m_cDBConns.size(); ++i)
    {
        if (m_cDBConns[i].bAssigned && (m_cDBConns[i].pcDBConn == pcDBConn))
        {
            break;
        }
    }

    if (i < m_cDBConns.size())
    {
        m_cDBConns[i].bAssigned = false;
    }
    else
    {
        log(enLOGLVL_ERROR, "CConnectPool::DeallocateDBConnection detected incorrect deallocation");
    }

    OspSemGive(m_hDBConnsLock);
}

void CConnectPool::Dump()
{
    vector<TDBConnInfo> tDBConns;
    OspSemTake(m_hDBConnsLock);
    tDBConns = m_cDBConns;
    OspSemGive(m_hDBConnsLock);

    OspPrintf(TRUE, FALSE, " ���ӳ�״̬��Ϣ================================================\n");
    OspPrintf(TRUE, FALSE, " ȱʡ����������Ϣ\n");
    OspPrintf(TRUE, FALSE, " \t���ݿ�����(1-sybase 2-mysql 3-oracle)��%d\n", (s32)m_tDBConnectDefaultCfg.enDbType);
    OspPrintf(TRUE, FALSE, " \t���ݿ������IP��%s\n", m_tDBConnectDefaultCfg.strDbServerIP.c_str());
    OspPrintf(TRUE, FALSE, " \t���ݿ�������˿ڣ�%u\n", (u32)m_tDBConnectDefaultCfg.wDbServerPort);
    OspPrintf(TRUE, FALSE, " \t���ݿ��������¼���ƣ�%s\n", m_tDBConnectDefaultCfg.strDbUserName.c_str());
    OspPrintf(TRUE, FALSE, " \t���ݿ��������¼���룺%s\n", m_tDBConnectDefaultCfg.strDbUserPassword.c_str());
    OspPrintf(TRUE, FALSE, " \t���ݿ�������������ݿ⣺%s\n", m_tDBConnectDefaultCfg.strDbName.c_str());

    u32 dwAssignedConnNum = 0;
    for (u32 i = 0; i < tDBConns.size(); ++i)
    {
        if (tDBConns[i].bAssigned)
        {
            ++dwAssignedConnNum;
        }
    }
    OspPrintf(TRUE, FALSE, "\n ��ǰ������Ϣ, ��������Ŀ:%u���Ѿ�������Ŀ:%u\n", (u32)tDBConns.size(), dwAssignedConnNum);
}


CDBConnection::CDBConnection(const TDBConnectCfg &tCfg)
{
    m_tDBConnectCfg = tCfg;

    m_pcDBILayer = new CDBILayer();
}

CDBConnection::~CDBConnection()
{
    delete m_pcDBILayer;
    m_pcDBILayer = NULL;
}

bool CDBConnection::CheckConnection(bool bTryExecTestSql)
{
    // ����ִ��SQL��䣬���Է�����������
    if (bTryExecTestSql && m_pcDBILayer->IsConnect())
    {
        if (enDBTypeSybase != m_tDBConnectCfg.enDbType)
        {
            ExecSQL("select 1 from dual", enExecMustRet);
        }
        else
        {
            ExecSQL("select 1", enExecMustRet);
        }
    }

    if (m_pcDBILayer->IsConnect())
    {
        return true;
    }
    else
    {
        // �Ͽ�����ʱ�Ƿ���Ȼ��Ҫ��ʾ�����Ž���? �鿴��ʵ�ִ��벻��Ҫ.��Ӧ�������о�
        // m_pcDBILayer->Disconnect(); 
        return m_pcDBILayer->Connect(m_tDBConnectCfg);
    }
}

void CDBConnection::Disconnect()
{
    if (m_pcDBILayer->IsConnect())
    {
        m_pcDBILayer->Disconnect();
    }
}

const TDBConnectCfg& CDBConnection::ConnectCfg() const
{
    return m_tDBConnectCfg;
}

bool CDBConnection::ExecSQL( const string &strSQL, ENExecOpt enExecOpt )
{
    return ExecSQL(strSQL.c_str(), enExecOpt);
}

bool CDBConnection::ExecSQL( const TSqlString &strSQL, ENExecOpt enExecOpt )
{
    return ExecSQL(strSQL.GetBuf(), enExecOpt);
}

bool CDBConnection::ExecSQL( const s8 *strSQL, ENExecOpt enExecOpt )
{
    if (!m_pcDBILayer->IsConnect())
    {
        if (!m_pcDBILayer->Connect(m_tDBConnectCfg))
        {
            return false;
        }
    }

	//OspPrintf(TRUE, FALSE, "sql ctrl��%s\n", strSQL);

    if (enExecNoRet == enExecOpt)
    {
        return m_pcDBILayer->ExecSql(strSQL, false);
    }
    else if (enExecMayRet == enExecOpt)
    {
        return m_pcDBILayer->ExecSql(strSQL, true);
    }
    else // enExecMustRet
    {
        bool bRet = m_pcDBILayer->ExecSql(strSQL, true);
        if (bRet)
        {
            if (m_pcDBILayer->IsNotEof())
            {
                return true;
            }
            else
            {
                // δȡ����¼, Ҳ����ʧ��
                log(enLOGLVL_ERROR, "CDBConnection::ExecSQL success, but not found dataset");
                return false;
            }
        }
        else
        {
            return false;
        }
    }
}

u32 CDBConnection::GetAffectedRowCount()
{
    return m_pcDBILayer->GetAffectedRowCount();
}

bool CDBConnection::IsNotEof()
{
    return m_pcDBILayer->IsNotEof();
}

const s8* CDBConnection::GetString( const s8* field )
{
    return m_pcDBILayer->GetString(field);
}

CCMSTime CDBConnection::GetCmsDate( const s8* field )
{
    return m_pcDBILayer->GetCmsDate(field);
}

s32 CDBConnection::GetInt( const s8* field )
{
    return m_pcDBILayer->GetInt(field);
}

bool CDBConnection::GetBool( const s8* field )
{
    return m_pcDBILayer->GetBool(field);
}

double CDBConnection::GetDouble( const s8* field )
{
    return m_pcDBILayer->GetDouble(field);
}

bool CDBConnection::MoveNext()
{
    return m_pcDBILayer->MoveNext();
}

CDBConnection::ENMultiRsProcCode CDBConnection::NextResultset()
{
    return m_pcDBILayer->NextResultset();
}


DBIX_NS_END
