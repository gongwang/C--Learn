#include<iostream>
#include<string>
#include<sstream> 
#include <cstring>
using namespace std;

//
//����һ�����򼴽���������ת��Ϊchar*���ͣ���Ϊchar*�ǲ�ͬ����֮�������.

//////////////////////////////////////////////////

/*=============================================================================
    ������        ��CP1
    ����          ��char*  char[] ��string��ת����
    �㷨ʵ��      ������ѡ�
    ����ȫ�ֱ���  ��
    �������˵��  ��                
    ����ֵ˵��    ����
    ��ע          ��
=============================================================================*/
void cp1()
{
    string strTemp, strTemp1;
    char *cTemp = "hello world!";
    char acTmep[] = "My coco!";
    strTemp = cTemp;
    strTemp1 = acTmep;
    printf("cp1 char*  char[] ��string��ת�� Before Change:%s %s After Change: %s  %s\n", cTemp, acTmep, strTemp.c_str(), strTemp1.c_str());
};


/*=============================================================================
    ������        ��CP2
    ����          ��string �� char * ��ת��
    �㷨ʵ��      ������ѡ�
    ����ȫ�ֱ���  ��
    �������˵��  ��                
    ����ֵ˵��    ����
    ��ע          ��
=============================================================================*/
void cp2()
{
    string strTemp = "hello world!";
    char * cTemp = new char[strTemp.length() + 1];
    strcpy(cTemp,strTemp.c_str());
    printf("cp2 string �� char * ��ת�� Before Change:%s  After Change: %s \n", strTemp.c_str(), cTemp);
};

/*=============================================================================
    ������        ��CP3
    ����          ��string �� const char * ��ת��
    �㷨ʵ��      ������ѡ�
    ����ȫ�ֱ���  ��
    �������˵��  ��                
    ����ֵ˵��    ����
    ��ע          ��
=============================================================================*/
void cp3()
{
    string strTemp = "hello world!";
    const char * cTemp = strTemp.c_str();
    printf("cp3 string �� const char * ��ת�� Before Change:%s  After Change: %s \n", strTemp.c_str(), cTemp);
};

/*=============================================================================
    ������        ��CP4
    ����          ��int �� string��ת��
    �㷨ʵ��      ������ѡ�
    ����ȫ�ֱ���  ��
    �������˵��  ��                
    ����ֵ˵��    ����
    ��ע          ��
=============================================================================*/
void cp4()
{
    string strTemp = "";
    char c[8] = {0};
    int i = 99;
    itoa(i,c,10);
    strTemp = i;
    
    printf("cp4 0 int �� string��ת�� Before Change:%d  After Change: %s \n", i, strTemp.c_str()); // 99 -> c

    sprintf(c,"%d",i);
    strTemp = c;
    printf("cp4 1 int �� string��ת�� Before Change:%d  After Change: %s \n", i, strTemp.c_str()); // 99 -> "99"

};

/*=============================================================================
    ������        ��CP5
    ����          ��string �� int ��ת��
    �㷨ʵ��      ������ѡ�
    ����ȫ�ֱ���  ��
    �������˵��  ��                
    ����ֵ˵��    ����
    ��ע          ��
=============================================================================*/
void cp5()
{
    string strTemp = "98";

    int i = atoi(strTemp.c_str());
    printf("cp5 0 string �� int ��ת�� Before Change:%s  After Change: %d \n", strTemp.c_str(), i); // "98" -> 98

    int j = 0;
    j = (char)i;
    printf("cp5 1 string �� int ��ת�� Before Change:%s  After Change: %c \n",strTemp.c_str(), j); // "98" -> b

};

/*=============================================================================
    ������        ��CP6
    ����          ��string �� duuble ��ת��
    �㷨ʵ��      ������ѡ�
    ����ȫ�ֱ���  ��
    �������˵��  ��                
    ����ֵ˵��    ����
    ��ע          ��
=============================================================================*/
void cp6()
{
    string strTemp = "98";

    double i = atof(strTemp.c_str());
    printf("cp5 0 tring �� duuble ��ת�� Before Change:%s  After Change: %f \n", strTemp.c_str(), i); // "98" -> 98


};

/*=============================================================================
    ������        ��CP8
    ����          ��double ��  string��ת��
    �㷨ʵ��      ������ѡ�
    ����ȫ�ֱ���  ��
    �������˵��  ��                
    ����ֵ˵��    ����
    ��ע          ��
=============================================================================*/
void cp8()
{
    string strTemp = "";
    char cTemp[128] = {0};
    double i = 97.6878;
    //sprintf(cTemp,"%s",i);
    strTemp = cTemp;
    printf("cp8 0 tring �� duuble ��ת�� Before Change:%s  After Change: %f \n", strTemp.c_str(), i); // "98" -> 98
};

/*=============================================================================
    ������        ��CP7
    ����          ��string �� cstring ��ת��
    �㷨ʵ��      ������ѡ�
    ����ȫ�ֱ���  ��
    �������˵��  ��                
    ����ֵ˵��    ����
    ��ע          ��
=============================================================================*/
void cp7()
{
    string strTemp = "98";
    //CString cstrTemp = "";
    //cstrTemp.Format("%s",strTemp);
    //printf("cp7  string �� cstring ��ת�� Before Change:%s  After Change: %s \n", strTemp.c_str(), cstrTemp); // "98" -> 98
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


