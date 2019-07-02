#pragma once

#include "CDriveMain.h"

class bokatest
{
public:
    bokatest(void);
    ~bokatest(void);

private:
    CDriveMain mIOMain;	//命令操作对象，其之下包含所有的命令函数
    COleException* mpe;
    void EventLoop();

};

