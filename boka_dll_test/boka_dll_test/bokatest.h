#pragma once

#include "CDriveMain.h"

class bokatest
{
public:
    bokatest(void);
    ~bokatest(void);

private:
    CDriveMain mIOMain;	//�������������֮�°������е������
    COleException* mpe;
    void EventLoop();

};

