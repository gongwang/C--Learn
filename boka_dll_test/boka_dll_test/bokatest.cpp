#include "StdAfx.h"
#include "bokatest.h"

#define EventLoopID 8888
bokatest::bokatest(void)
{
    if (!mIOMain.CreateDispatch(_T("FCDrive8800.DriveMain"), mpe))
    {
        printf(("��ʼ������"));
        return ;
    }
    else
    {
        //�����¼�����ģʽ
        mIOMain.SetShowEvent(0);//��ʾʹ���ֶ���ȡ��ʽ�õ��¼�
        printf("��ʼ���ɹ���ver: %s",  mIOMain.GetVer());
        SetTimer(EventLoopID, 100, 0);//�Զ����¼���ʱ��
    }
}


bokatest::~bokatest(void)
{
}

void bokatest::EventLoop()
{

}
