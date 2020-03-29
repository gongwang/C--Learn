// SplitINT.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

//����1
/************************************************************************/
/* 
ͨ���۲죬���ȿ϶�������ȷ��2��3��û�취���в�ֵ���С���ӡ�ͬʱ�����Ǻ����ܿ�������
ֻҪ��n�����ܵĲ�ֳɰ���3����ϣ��Ϳ��Եõ����ֵ��
���û�취���3����ϣ�����һ�����2����ϡ�
����3��2��û�취�ٽ��в�֡�

���ݷ��������ǳ���ʹ��̰�Ľ�����⡣��Ϊһ����������Ϊn��������һ���������ǰ����������֣�x�����������֣�y�����Ǹղ�Ҳ�õ��ˣ�����������3������������Ҫ�� n/3�������Ļ�������������1,2 ���ֿ����ԡ�
���������1���ղ�����Ҳ������������1�Ĳ����û������ģ�����������һ���������һ�ε�3��1�Ĳ�֣���2��2���档
���������2���ǲ�����˵��ֱ�ӳ�������2���ɡ�                                                                     */
/************************************************************************/
static int integerBreak(int n) 
{
	if (n== 1) return n;
	if (n <= 3) return n - 1;
	int x = n / 3, y = n % 3;
	//ǡ��������ֱ��Ϊ3^x
	    if (y == 0) return (int)pow((double)3, x);
	//����Ϊ1����һ�� 3^(x-1)*2*2
	    if (y == 1) return (int)pow((double)3, x - 1) * 4;
	//����Ϊ2��ֱ�ӳ���2
	   return (int)pow((double)3, x) * 2;
}

//����2 ��̬�滮�㷨
//dynamic programming
static int integerBreakDynamic(unsigned int n)
{
	if (NULL == n)
	{
		return 0;
	}	 
	vector<int> dp(n + 1,0);
	dp[1] = 1;
	for (int i = 2; i <= n; i++)
	{
		for (int j = 1; j < i; j++)
		{
			dp[i] = max(dp[i], max(dp[j], j) * (i - j));
		}
	}
	return(dp[n]);
}

int _tmain(int argc, _TCHAR* argv[])
{
	while(1)
	{
		int n = 0;
		cout<<"Input your test int:";
		cin>>n;

		printf("integerBreak max: %d\n", integerBreak(n));

		printf("integerBreakDynamic max: %d\n", integerBreakDynamic(n));

	}

	return 0;
}

