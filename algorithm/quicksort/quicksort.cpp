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
void swap(int * nLeft, int *nRight)
{
	int temp = *nLeft;
	*nLeft = *nRight;
	*nRight = temp;
}

void swap(int arry[], int i , int j)
{
	arry[i] = arry[i] +arry[j] - (arry[i] = arry[j]);
}

void printarry(int * arry, DWORD dwLen)
{
	printf("array len:%d content: ", dwLen);
	for (int i = 0; i<dwLen; i++)
	{
		printf("%d  ",arry[i]);
	}
	printf("\n");
}

//快速排序
/*初始值 : 5,1,9,3,7,4,8,6,2
*/
//错误示范
//void sort_quik(int *arry, DWORD dwLen)
//{
//    printarry(arry, dwLen);
//
//    if (NULL == arry || dwLen < 2)
//    {
//        return;
//    }
//
//	int nPivot = arry[0];
//	int low = 0;
//	int hight = dwLen -1;
//
//	while(low<hight)
//	{
//		if (arry[hight] <= nPivot)
//		{
//			swap(&arry[low],&arry[hight]);
//
//			printf("hight < pivot swap: low:%d hight:%d ", low,hight);
//			printarry(arry, dwLen);
//			
//			while(low<hight)
//			{
//				if (arry[low]>= nPivot)
//				{
//					printf("low >pivot swap: low:%d hight:%d ", low,hight);
//					swap(&arry[low], &arry[hight]);
//					printarry(arry, dwLen);
//					break;
//				} 
//				else
//				{
//					low++;
//				}
//			}
//		} 
//		else
//		{
//			hight--;
//		}
//	}
//
//	printf("After sort  source len:%d content: ", dwLen);
//	for (int i = 0; i<dwLen; i++)
//	{
//		printf("%d  ",arry[i]);
//	}
//	printf("\n");
//
//	//一半砍 肯定不对的，会漏掉
//	//sort_quik(arry,  dwLen/2 );
//	//sort_quik(&arry[dwLen/2], dwLen/2);
//
//	sort_quik(arry,  dwLen/2 );
//	sort_quik(&arry[dwLen/2], dwLen/2);
//}

//题解：
/*
初始值 : 5,1,9,3,7,4,8,6,2
速排序的思想是，通过一趟排序将待排记录分割成独立的两部分，
其中一部分记录的关键字均比另一部分记录的关键字小，则可分别对这两部分记录继续进行排序，以达到整个序列有序的目的。

思路：

选择5作为一个基准元素，然后从右向左移动hight下标，进行基准元素和下标为hight的元素进行比较。

如果基准元素要大，则进行hight的左移一位；如果基准元素要小，则进行元素的交换。

在hight下标左移的过程中，我们目的是找出比基准元素小的，然后进行交换。

交换完之后，进行left的右移，找出比基准元素大的，找到则进行交换。

*/

//记录标杆位置
void sort_quik(int *arry, DWORD dwLen)
{
	printarry(arry, dwLen);

	if (NULL == arry || dwLen < 2)
	{
		return;
	}

	int nPivot = 0;
	int low = 0;
	int hight = dwLen -1;

	while(low<hight)
	{
		if (arry[hight] < arry[nPivot])
		{
			swap(&arry[nPivot],&arry[hight]);
			nPivot = hight;
			printf("hight < pivot swap: low:%d hight:%d nPivot:%d \n", low,hight,nPivot);
			printarry(arry, dwLen);

			while(low<hight)
			{
				if (arry[low]> arry[nPivot])
				{					
					swap(&arry[low], &arry[nPivot]);
					nPivot = low;
					printf("low >pivot swap: low:%d hight:%d nPivot:%d \n", low,hight,nPivot);
					printarry(arry, dwLen);
					break;
				} 
				else
				{
					low++;
				}
			}
		} 
		else
		{
			hight--;
		}
	}

	printf("After sort  source len:%d npvoit: %d content: ", dwLen, nPivot);
	for (int i = 0; i<dwLen; i++)
	{
		printf("%d  ",arry[i]);
	}
	printf("\n");

	//一半砍 肯定不对的，会漏掉
	//sort_quik(arry,  dwLen/2 );
	//sort_quik(&arry[dwLen/2], dwLen/2);

	sort_quik(arry,  nPivot );
	sort_quik(&arry[nPivot + 1], dwLen - nPivot - 1);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int arry[] = {5,2,1,9,3,7,4,8,6,2};
    //int arry[] = {7,2,5,3,9,4,8,2,6};
	
	//
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