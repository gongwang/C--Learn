#if WIN32 && (_MSC_VER <= 1200)
#pragma warning(disable : 4786)
#endif

#include "task_queue.h"

DBIX_NS_BEGIN

extern CTaskQueue g_cTaskQueue;

bool PostDBTask(CDBTask *pcTask)
{
    return g_cTaskQueue.Enqueue(pcTask);
}


CTaskQueue::CTaskQueue()
{
    m_hDbTaskListLock = NULL;
    m_hDbTaskSemCount = NULL;
}

CTaskQueue::~CTaskQueue()
{
    Quit();
}

bool CTaskQueue::Enqueue( const TDbTaskItem &tItem )
{
    OspSemTake(m_hDbTaskListLock);
    m_tDbTaskList.push_back(tItem);
    OspSemGive(m_hDbTaskListLock);

    OspSemGive(m_hDbTaskSemCount);

    return true;
}

bool CTaskQueue::Dequeue( TDbTaskItem &tItem, u32 dwTimeout )
{
    if (FALSE == OspSemTakeByTime(m_hDbTaskSemCount, dwTimeout))
        return false;

    OspSemTake(m_hDbTaskListLock);
    tItem = *m_tDbTaskList.begin();
    m_tDbTaskList.pop_front();
    OspSemGive(m_hDbTaskListLock);

    return true;
}

bool CTaskQueue::Init()
{
    if (NULL != m_hDbTaskListLock)
    {
        log(enLOGLVL_ERROR, "CTaskQueue reinited");
        return true;
    }

    do 
    {
        if (!OspSemBCreate(&m_hDbTaskListLock))
        {
            break;
        }
        if (!OspSemCCreate(&m_hDbTaskSemCount, 0, 0xfffffff))
        {
            break;
        }

        return true;
    } while (false);

    Quit();
    log(enLOGLVL_ERROR, "CTaskQueue init fail");
    return false;
}

void CTaskQueue::Quit()
{
    while(!m_tDbTaskList.empty())
    {
        delete m_tDbTaskList.front();
        m_tDbTaskList.pop_front();
    }

    if (NULL != m_hDbTaskSemCount)
    {
        OspSemDelete(m_hDbTaskSemCount);
        m_hDbTaskSemCount = NULL;
    }
    if (NULL != m_hDbTaskListLock)
    {
        OspSemDelete(m_hDbTaskListLock);
        m_hDbTaskListLock = NULL;
    }
}

void CTaskQueue::Dump()
{
    OspSemTake(m_hDbTaskListLock);
    u32 dwTaskNum = (u32)m_tDbTaskList.size();
    OspSemGive(m_hDbTaskListLock);
    OspPrintf(TRUE, FALSE, " 任务队列状态信息===========================================================\n");
    OspPrintf(TRUE, FALSE, " 队列中剩余任务数目:%u\n", dwTaskNum);
}

DBIX_NS_END
