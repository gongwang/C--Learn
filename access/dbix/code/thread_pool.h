/*========================================================================================
模块名    ：dbix.lib
文件名    ：thread_pool.h
相关文件  ：
实现功能  ：线程池实现类。
作者      ：huzhiyun
版权      ：<Copyright(C) 2003-2008 Suzhou Keda Technology Co., Ltd. All rights reserved.>
-------------------------------------------------------------------------------------------
修改记录：
日期               版本              修改人             走读人              修改记录
2012/06/08         V1.0              huzhiyun                              新建文件
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
    // 根据连接信息自动创建相应的线程，一个连接一个线程
    bool Init(CTaskQueue *cQueue, const vector<CDBConnection*> &cDBConns);
    void Quit();
    void Dump();

private:
    static void* ProcTaskEntry(void* parm);
    void ProcTask(u32 dwThreadIndex, const TDbTaskItem &tDbTaskItem);
private:
    CTaskQueue *m_pcQueue;                    // 任务队列
    vector<CDBConnection*> m_cTaskDbConnects; // 连接对象
    vector<TASKHANDLE> m_cTaskHandles;        // 线程句柄
    bool m_bQuit;                             // 退出标志
};

DBIX_NS_END
#endif // _THREAD_POOL_H_
