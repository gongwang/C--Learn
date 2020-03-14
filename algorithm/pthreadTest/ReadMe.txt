========================================================================
    CONSOLE APPLICATION : pthreadTest Project Overview
========================================================================

AppWizard has created this pthreadTest application for you.

This file contains a summary of what you will find in each of the files that
make up your pthreadTest application.


pthreadTest.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

pthreadTest.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

pthreadTest.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named pthreadTest.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

pthread 使用须知
1.头文件： #include "pthread_w32.h"

2.lib: pthreadVC2.lib pthreadVC2.dll

3.创建线程：pthread_create
	pthread_t ptid;
    pthread_create(&ptid,NULL,ConnectClientProc,(void*)cName);

	void * ConnectClientProc(void * args){}
4.pthread_detach (pthread_t tid);
	1.linux线程执行和windows不同，pthread有两种状态joinable状态和unjoinable状态，如果线程是joinable状态，当线程函数自己返回退出时或pthread_exit时都不会释放线程所占用堆栈和线程描述符（总计8K多）。只有当你调用了pthread_join之后这些资源才会被释放。若是unjoinable状态的线程，这些资源在线程函数退出时或pthread_exit时自动会被释放。
	2.unjoinable属性可以在pthread_create时指定，或在线程创建后在线程中pthread_detach自己, 如：pthread_detach(pthread_self())，将状态改为unjoinable状态，确保资源的释放。或者将线程置为 joinable,然后适时调用pthread_join.
	3.其实简单的说就是在线程函数头加上 pthread_detach(pthread_self())的话，线程状态改变，在函数尾部直接 pthread_exit线程就会自动退出。省去了给线程擦屁股的麻烦。
	————————————————
	版权声明：本文为CSDN博主「魏波-」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
	原文链接：https://blog.csdn.net/weibo1230123/article/details/81410241
	
	注意：如果使用了pthread_detach,就不要再使用join，可能会在有返回值的时候，引起崩溃
		
		若是unjoinable状态的线程，这些资源在线程函数退出时或pthread_exit时自动会被释放
	
5.等待线程结束：pthread_join(ptid, void **value_ptr);
	
  pthread_join(ptid, NULL);
  
  pthread_join用于等待一个线程的结束，也就是主线程中要是加了这段代码，就会在加代码的位置卡主，直到这个线程执行完毕才往下走。
  
  pthread_join() 子程序阻碍调用程序，直到指定的threadid 线程终止为止。当创建一个线程时，它的某个属性会定义它是否是可连接的（joinable）或可分离的（detached）。只有创建时定义为可连接的线程才可以被连接。如果线程创建时被定义为可分离的，则它永远也不能被连接。
  
6.线程主动退出：pthread_exit(void *value_ptr)

	pthread_exit用于强制退出一个线程（非执行完毕退出），一般用于线程内部。

7.取消其他线程：pthread_cancel (pthread_t thread);


8.create、exit/return 、join 示例
// pthreadTest.cpp : Defines the entry point for the console application.
//

https://blog.csdn.net/super828/article/details/86653318

#include "stdafx.h"

#include "pthread_w32.h"

#include <Windows.h>
#include <iostream>

typedef struct strTemp{
public:
    strTemp(){i = 1;}
    strTemp(int nTemp){i = nTemp;}
public:
    int i;
}STRTEMP, *pSTRTEMP;

void * ConnectClientProc(void * args)
{
    if (args == NULL)
    {
        printf("传入参数为空\n");
        return NULL;
    }
    char *cName = (char*)args;

    printf("Myname is : %s \n Sleep 1000ms... \n", cName);

    Sleep(1000);

    STRTEMP *cRet = new STRTEMP(10);
    //pthread_exit((void*)cRet);
    return (void*)cRet; //or 
}

int _tmain(int argc, _TCHAR* argv[])
{

    char cName[32] = "cococo";

    pthread_t ptid;
    pthread_create(&ptid,NULL,ConnectClientProc,(void*)cName);
    //pthread_detach(ptid);

    printf("Main is running... \n");

    void *cRcv;
    pthread_join(ptid, &cRcv);
    printf("Thread over, and recv : %d\n", (*(STRTEMP*)cRcv).i); //一定要强转
    delete cRcv;
    cRcv = NULL;

    system("pause");

	return 0;
}


