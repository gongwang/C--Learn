// quicksort.cpp : Defines the entry point for the console application.
//���������㷨
//��ʼ״̬��5, 1, 9, 3, 7, 4, 8, 6, 2
//
//    ѡ��5��Ϊһ����׼Ԫ�أ�Ȼ����������ƶ�hight�±꣬���л�׼Ԫ�غ��±�Ϊhight��Ԫ�ؽ��бȽϡ�
//
//    �����׼Ԫ��Ҫ�������hight������һλ�������׼Ԫ��ҪС�������Ԫ�صĽ�����
//
//    ��hight�±����ƵĹ����У�����Ŀ�����ҳ��Ȼ�׼Ԫ��С�ģ�Ȼ����н�����
//
//    ������֮�󣬽���left�����ƣ��ҳ��Ȼ�׼Ԫ�ش�ģ��ҵ�����н�����

#include "stdafx.h"
#include <Windows.h>

//���ټ���1+2+3+...+n������ʹ��if��while��
/*
int GetSums(int n)
{
    printf("n = %d\n", n);
    n&&(n+=GetSums(n-1));
    printf("sum = %d \n", n);
    return n;
}

int _tmain(int argc, _TCHAR* argv[])
{

    int sum = GetSums(2);

    printf("Sum is %d\n",sum);

    system("pause");

	return 0;
}
*/


//��������
/*��ʼֵ : 5,1,9,3,7,4,8,6,2



*/
void sort_quik(const int * arry, DWORD dwLen)
{
    printf("arry source len:%d content: ", dwLen);
    for (int i = 0; i<dwLen; i++)
    {
        printf("%d ",arry[i]);
    }
    printf("\n");

    if (NULL == arry)
    {
        return;
    }





}


int _tmain(int argc, _TCHAR* argv[])
{
    int arry[] = {5,1,9,3,7,4,8,6,2};

    sort_quik(arry, sizeof(arry)/sizeof(int));

    printf("After sort_quik arry source content: ");
    for (int i = 0; i<sizeof(arry)/sizeof(int); i++)
    {
        printf("%d ",arry[i]);
    }
    printf("\n");


    system("pause");

    return 0;
}