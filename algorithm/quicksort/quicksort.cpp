// quicksort.cpp : Defines the entry point for the console application.
//快速排序算法
//初始状态：5, 1, 9, 3, 7, 4, 8, 6, 2
//
//    选择5作为一个基准元素，然后从右向左移动hight下标，进行基准元素和下标为hight的元素进行比较。
//
//    如果基准元素要大，则进行hight的左移一位；如果基准元素要小，则进行元素的交换。
//
//    在hight下标左移的过程中，我们目的是找出比基准元素小的，然后进行交换。
//
//    交换完之后，进行left的右移，找出比基准元素大的，找到则进行交换。

#include "stdafx.h"
#include <Windows.h>

//快速计算1+2+3+...+n，不许使用if，while等
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


//快速排序
/*初始值 : 5,1,9,3,7,4,8,6,2



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