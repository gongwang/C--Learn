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

//��������
/*��ʼֵ : 5,1,9,3,7,4,8,6,2
*/
//����ʾ��
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
//	//һ�뿳 �϶����Եģ���©��
//	//sort_quik(arry,  dwLen/2 );
//	//sort_quik(&arry[dwLen/2], dwLen/2);
//
//	sort_quik(arry,  dwLen/2 );
//	sort_quik(&arry[dwLen/2], dwLen/2);
//}

//��⣺
/*
��ʼֵ : 5,1,9,3,7,4,8,6,2
�������˼���ǣ�ͨ��һ�����򽫴��ż�¼�ָ�ɶ����������֣�
����һ���ּ�¼�Ĺؼ��־�����һ���ּ�¼�Ĺؼ���С����ɷֱ���������ּ�¼�������������Դﵽ�������������Ŀ�ġ�

˼·��

ѡ��5��Ϊһ����׼Ԫ�أ�Ȼ����������ƶ�hight�±꣬���л�׼Ԫ�غ��±�Ϊhight��Ԫ�ؽ��бȽϡ�

�����׼Ԫ��Ҫ�������hight������һλ�������׼Ԫ��ҪС�������Ԫ�صĽ�����

��hight�±����ƵĹ����У�����Ŀ�����ҳ��Ȼ�׼Ԫ��С�ģ�Ȼ����н�����

������֮�󣬽���left�����ƣ��ҳ��Ȼ�׼Ԫ�ش�ģ��ҵ�����н�����

*/

//��¼���λ��
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

	//һ�뿳 �϶����Եģ���©��
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