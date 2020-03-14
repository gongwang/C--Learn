/*========================================================================================
ģ����    ��dbix.lib
�ļ���    ��task_queue.h
����ļ�  ��
ʵ�ֹ���  ����������ʵ���ࡣ
����      ��huzhiyun
��Ȩ      ��<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
�޸ļ�¼��
����               �汾              �޸���             �߶���              �޸ļ�¼
2012/06/08         V1.0              huzhiyun                              �½��ļ�
=========================================================================================*/

#ifndef _TASK_QUEUE_H_
#define _TASK_QUEUE_H_
#include "inner_common.h"
#include <list>
using std::list;

DBIX_NS_BEGIN

typedef CDBTask* TDbTaskItem;

class CTaskQueue
{
private:
    list<TDbTaskItem> m_tDbTaskList;
    SEMHANDLE m_hDbTaskListLock;
    SEMHANDLE m_hDbTaskSemCount; 

public:
    CTaskQueue();
    ~CTaskQueue();

    bool Init();
    void Quit();
    void Dump();

    bool Enqueue(const TDbTaskItem &tItem);
    bool Dequeue(TDbTaskItem &tItem, u32 dwTimeout); // ȡ�����񷵻�true, dwTimeout��λΪ����
};

DBIX_NS_END
#endif // _TASK_QUEUE_H_
