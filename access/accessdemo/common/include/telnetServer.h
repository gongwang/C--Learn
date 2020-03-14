/*========================================================================================
    ģ����    ��telnetServer
    �ļ���    ��telnetServer.h
    ����ļ�  ��
    ʵ�ֹ���  ������osp��telnet�������浯��cmd�����ֱ��ʹ��API����
    ����      ��zhaojunxiang
    ��Ȩ      ��<Copyright(C) 2003-2020 Suzhou Keda Technology Co., Ltd. All rights reserved.>
    -------------------------------------------------------------------------------------------
    �޸ļ�¼��
    ����               �汾              �޸���             �߶���              �޸ļ�¼
    2019/12/26         V1.0        zhaojunxiang                         �½��ļ�
    =========================================================================================*/
#ifndef  _TELNETSERVER_H
#define _TELNETSERVER_H

#include "ospext/ospext.h"
#include <vector>
#include "gsserrcode.h"

//����ȫ��vector��������ָ��
extern std::vector<void*> g_vecDevObject;
extern CRITICAL_SECTION g_csDevObjectLock;
extern BOOL g_bDevObjectLockInit;
///////////////////////////////////////////////////////////////////////  
static BOOL initOspServer(const std::string &strDllName)
{
    //telnet ����
    std::string strWholeDllName = strDllName;
    strWholeDllName.append(".dll");

    OspInit( TRUE, 2500);
    OspSetPrompt(strDllName.c_str());
    OspRegistModule(strWholeDllName.c_str()); //�����ע��dll�����ܻᵼ���޷��ҵ�API����

    if (OSPEXT_OK != OspExtInit())
    {
        OspPrintf(TRUE, FALSE, "ospext��ʼ��ʧ��\n");

        printf("ospext��ʼ��ʧ��\n");
        return GSS_ERROR_DLL_DEV_INIT_ERROR;
    }
    OspPrintf( TRUE, FALSE, "Creat  access OSP success\n" );
    return TRUE;
}

//���������ָ��

static BOOL insertObject2Vector(void *objectPtr)
{
    BOOL bRet = TRUE;

    if (g_bDevObjectLockInit == FALSE)
    {
        g_bDevObjectLockInit = TRUE;
        InitializeCriticalSection(&g_csDevObjectLock);
        printf("InitializeCriticalSection g_csDevObjectLock. Init:%d(0x%X) cs:0x%X\n", g_bDevObjectLockInit, &g_bDevObjectLockInit, &g_csDevObjectLock);
    }

    EnterCriticalSection(&g_csDevObjectLock);
    g_vecDevObject.push_back(objectPtr);
    LeaveCriticalSection(&g_csDevObjectLock);

    return bRet;
}

//ɾ�������ָ��

static BOOL deleteObject2Vector(void *objectPtr)
{
    BOOL bRet = FALSE;

    EnterCriticalSection(&g_csDevObjectLock);

    std::vector<void*>::iterator it = g_vecDevObject.begin();

    for (;it != g_vecDevObject.end(); it++)
    {
        if (*it == objectPtr)
        {
            g_vecDevObject.erase(it);
            bRet =FALSE;
        }
    }

    LeaveCriticalSection(&g_csDevObjectLock);

    return bRet;
}

static int GetDevObjectSize()
{
    printf("GetDevObjectSize before...Init:%d(0x%X) cs:0x%X\n", g_bDevObjectLockInit, &g_bDevObjectLockInit, &g_csDevObjectLock);
    EnterCriticalSection(&g_csDevObjectLock);
    printf("GetDevObjectSize end ...\n");

    int nCount = g_vecDevObject.size();

    LeaveCriticalSection(&g_csDevObjectLock);

    return nCount;
}

static void* GetDevObject(int nIndex)
{
    EnterCriticalSection(&g_csDevObjectLock);

    std::vector<void*>::iterator it = g_vecDevObject.begin();
    
    void* pObject = it[nIndex];

    printf("GetDevObject nindex: %d ptr: %p\n", nIndex, it[nIndex]);

    LeaveCriticalSection(&g_csDevObjectLock);

    return pObject;
}

/*********************************/
#endif