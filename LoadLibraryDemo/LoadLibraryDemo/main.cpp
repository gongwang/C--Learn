#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "uniplay.h"

using namespace std;
typedef BOOL (*StartUpFun)();

const double MAX_LENTH = 3.14159;
#define  MAX 3.14159


size_t coco( void )
{
    return 0;
}

StartUpFun g_pStartUpFun = NULL;
void main()
{
    HMODULE hModule = LoadLibrary("D:/VS2012_Project/LoadLibraryDemo/uniplay.dll");

    g_pStartUpFun = (StartUpFun)GetProcAddress(hModule, "PLAYKD_Startup");

    g_pStartUpFun();

    int i = coco();

    int nNum = 1;
    switch(nNum)
    {
    case 0:
        printf("00000000000000\n");
    case 1:
        printf("1111111111111111\n");
        break;
    }
    system("pause");
}


