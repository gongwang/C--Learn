/*========================================================================================
模块名    ：dbix.lib
文件名    ：task_queue.h
相关文件  ：
实现功能  ：任务容器实现类。
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/06/08         V1.0              huzhiyun                              新建文件
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
    bool Dequeue(TDbTaskItem &tItem, u32 dwTimeout); // 取到任务返回true, dwTimeout单位为毫秒
};

DBIX_NS_END
#endif // _TASK_QUEUE_H_
