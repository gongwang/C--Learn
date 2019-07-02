#include<iostream>
#include<string>
#include<sstream> 
#include <cstring>
using namespace std;

//
//朝着一个方向即将类型首先转化为char*类型，因为char*是不同类型之间的桥梁.

//////////////////////////////////////////////////

/*=============================================================================
    函数名        ：CP1
    功能          ：char*  char[] 到string的转换：
    算法实现      ：（可选项）
    引用全局变量  ：
    输入参数说明  ：                
    返回值说明    ：无
    备注          ：
=============================================================================*/
void cp1()
{
    string strTemp, strTemp1;
    char *cTemp = "hello world!";
    char acTmep[] = "My coco!";
    strTemp = cTemp;
    strTemp1 = acTmep;
    printf("cp1 char*  char[] 到string的转换 Before Change:%s %s After Change: %s  %s\n", cTemp, acTmep, strTemp.c_str(), strTemp1.c_str());
};


/*=============================================================================
    函数名        ：CP2
    功能          ：string 到 char * 的转换
    算法实现      ：（可选项）
    引用全局变量  ：
    输入参数说明  ：                
    返回值说明    ：无
    备注          ：
=============================================================================*/
void cp2()
{
    string strTemp = "hello world!";
    char * cTemp = new char[strTemp.length() + 1];
    strcpy(cTemp,strTemp.c_str());
    printf("cp2 string 到 char * 的转换 Before Change:%s  After Change: %s \n", strTemp.c_str(), cTemp);
};

/*=============================================================================
    函数名        ：CP3
    功能          ：string 到 const char * 的转换
    算法实现      ：（可选项）
    引用全局变量  ：
    输入参数说明  ：                
    返回值说明    ：无
    备注          ：
=============================================================================*/
void cp3()
{
    string strTemp = "hello world!";
    const char * cTemp = strTemp.c_str();
    printf("cp3 string 到 const char * 的转换 Before Change:%s  After Change: %s \n", strTemp.c_str(), cTemp);
};

/*=============================================================================
    函数名        ：CP4
    功能          ：int 到 string的转换
    算法实现      ：（可选项）
    引用全局变量  ：
    输入参数说明  ：                
    返回值说明    ：无
    备注          ：
=============================================================================*/
void cp4()
{
    string strTemp = "";
    char c[8] = {0};
    int i = 99;
    itoa(i,c,10);
    strTemp = i;
    
    printf("cp4 0 int 到 string的转换 Before Change:%d  After Change: %s \n", i, strTemp.c_str()); // 99 -> c

    sprintf(c,"%d",i);
    strTemp = c;
    printf("cp4 1 int 到 string的转换 Before Change:%d  After Change: %s \n", i, strTemp.c_str()); // 99 -> "99"

};

/*=============================================================================
    函数名        ：CP5
    功能          ：string 到 int 的转换
    算法实现      ：（可选项）
    引用全局变量  ：
    输入参数说明  ：                
    返回值说明    ：无
    备注          ：
=============================================================================*/
void cp5()
{
    string strTemp = "98";

    int i = atoi(strTemp.c_str());
    printf("cp5 0 string 到 int 的转换 Before Change:%s  After Change: %d \n", strTemp.c_str(), i); // "98" -> 98

    int j = 0;
    j = (char)i;
    printf("cp5 1 string 到 int 的转换 Before Change:%s  After Change: %c \n",strTemp.c_str(), j); // "98" -> b

};

/*=============================================================================
    函数名        ：CP6
    功能          ：string 到 duuble 的转换
    算法实现      ：（可选项）
    引用全局变量  ：
    输入参数说明  ：                
    返回值说明    ：无
    备注          ：
=============================================================================*/
void cp6()
{
    string strTemp = "98";

    double i = atof(strTemp.c_str());
    printf("cp5 0 tring 到 duuble 的转换 Before Change:%s  After Change: %f \n", strTemp.c_str(), i); // "98" -> 98


};

/*=============================================================================
    函数名        ：CP8
    功能          ：double 到  string的转换
    算法实现      ：（可选项）
    引用全局变量  ：
    输入参数说明  ：                
    返回值说明    ：无
    备注          ：
=============================================================================*/
void cp8()
{
    string strTemp = "";
    char cTemp[128] = {0};
    double i = 97.6878;
    //sprintf(cTemp,"%s",i);
    strTemp = cTemp;
    printf("cp8 0 tring 到 duuble 的转换 Before Change:%s  After Change: %f \n", strTemp.c_str(), i); // "98" -> 98
};

/*=============================================================================
    函数名        ：CP7
    功能          ：string 到 cstring 的转换
    算法实现      ：（可选项）
    引用全局变量  ：
    输入参数说明  ：                
    返回值说明    ：无
    备注          ：
=============================================================================*/
void cp7()
{
    string strTemp = "98";
    //CString cstrTemp = "";
    //cstrTemp.Format("%s",strTemp);
    //printf("cp7  string 到 cstring 的转换 Before Change:%s  After Change: %s \n", strTemp.c_str(), cstrTemp); // "98" -> 98
};

#if 0
void main()
{
    cp1();
    cp2();
    cp3();
    cp4();
    cp5();
    cp6();
    cp7();
    cp8();
    system("pause");
    return;
};
#endif


