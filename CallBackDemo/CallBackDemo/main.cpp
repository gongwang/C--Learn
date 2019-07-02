#include "windows.h"
#include "stdlib.h"
#include "stdio.h"

class CMyClassA;
class CMyClassB;

typedef void (*DemoCallBack)(int nNum, void* pContext);

class CMyClassA
{
public:
    CMyClassA(){};

    void Printf()
    {
        printf("AAAAAA\n");
        if(m_pFunCB)
        {
            m_pFunCB(10, m_pContext);
        }
    }
    void SetDataCB(DemoCallBack pFun, void* pContext)
    {
        m_pFunCB = pFun;
        m_pContext = pContext;
    }

    DemoCallBack m_pFunCB;
    void* m_pContext;
};

class CMyClassB
{
public:
    CMyClassB(){};
    void SetDataType(int nType)
    {
        m_nType = nType;
    }
    void Printf()
    {
        if(m_nType == 0)
        {
            printf("0000000\n");
        }
        else
        {
            printf("BBBBBBB\n");
        }
    }
    int m_nType;
};

void DemoCallBackFun(int nNum, void* pContext)
{
    CMyClassB* pClassB = (CMyClassB*)pContext;
    if(pClassB)
    {
        pClassB->Printf();
    }
}

void main()
{
    CMyClassA cMyClassA;
    CMyClassB cMyClassB1;
    CMyClassB cMyClassB2;
    cMyClassB1.SetDataType(1);
    cMyClassB2.SetDataType(0);

    cMyClassA.SetDataCB(DemoCallBackFun, (void*)&cMyClassB1);
    cMyClassA.Printf();

    cMyClassA.SetDataCB(DemoCallBackFun, (void*)&cMyClassB2);
    cMyClassA.Printf();

    Sleep(10000);
    system("pause");
}



