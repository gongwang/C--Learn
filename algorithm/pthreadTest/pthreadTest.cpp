// pthreadTest.cpp : Defines the entry point for the console application.
//

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
    printf("Thread over, and recv : %d\n", (*(STRTEMP*)cRcv).i);
    delete cRcv;
    cRcv = NULL;

    system("pause");

	return 0;
}

