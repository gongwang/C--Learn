/*========================================================================================
ģ����    ��dbix.lib
�ļ���    ��thread_pool.h
����ļ�  ��
ʵ�ֹ���  ���̳߳�ʵ���ࡣ
����      ��huzhiyun
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/06/08         V1.0              huzhiyun                              �½��ļ�
=========================================================================================*/
#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_
#include "inner_common.h"
#include "connect_pool.h"
#include "task_queue.h"

DBIX_NS_BEGIN

class CTaskQueue;
class CThreadPool
{
public:
    // ����������Ϣ�Զ�������Ӧ���̣߳�һ������һ���߳�
    bool Init(CTaskQueue *cQueue, const vector<CDBConnection*> &cDBConns);
    void Quit();
    void Dump();

private:
    static void* ProcTaskEntry(void* parm);
    void ProcTask(u32 dwThreadIndex, const TDbTaskItem &tDbTaskItem);
private:
    CTaskQueue *m_pcQueue;                    // �������
    vector<CDBConnection*> m_cTaskDbConnects; // ���Ӷ���
    vector<TASKHANDLE> m_cTaskHandles;        // �߳̾��
    bool m_bQuit;                             // �˳���־
};

DBIX_NS_END
#endif // _THREAD_POOL_H_
