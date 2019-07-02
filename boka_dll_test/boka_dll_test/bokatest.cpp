#include "StdAfx.h"
#include "bokatest.h"

#define EventLoopID 8888
bokatest::bokatest(void)
{
    if (!mIOMain.CreateDispatch(_T("FCDrive8800.DriveMain"), mpe))
    {
        printf(("初始化错误！"));
        return ;
    }
    else
    {
        //设置事件监听模式
        mIOMain.SetShowEvent(0);//表示使用手动获取方式得到事件
        printf("初始化成功！ver: %s",  mIOMain.GetVer());
        SetTimer(EventLoopID, 100, 0);//自定义事件定时器
    }
}


bokatest::~bokatest(void)
{
}

void bokatest::EventLoop()
{

}
