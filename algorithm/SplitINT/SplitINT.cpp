// SplitINT.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

//方法1
/************************************************************************/
/* 
通过观察，首先肯定可以明确，2和3是没办法进行拆分的最小因子。同时，我们好像能看出来：
只要把n尽可能的拆分成包含3的组合，就可以得到最大值。
如果没办法拆成3的组合，就退一步拆成2的组合。
对于3和2，没办法再进行拆分。

根据分析，我们尝试使用贪心进行求解。因为一个数（假设为n）除以另一个数，总是包括整数部分（x）和余数部分（y）。那刚才也得到了，最优因子是3，所以我们需要让 n/3，这样的话，余数可能是1,2 两种可能性。
如果余数是1，刚才我们也分析过，对于1的拆分是没有意义的，所以我们退一步，将最后一次的3和1的拆分，用2和2代替。
如果余数是2，那不消多说，直接乘以最后的2即可。                                                                     */
/************************************************************************/
static int integerBreak(int n) 
{
	if (n== 1) return n;
	if (n <= 3) return n - 1;
	int x = n / 3, y = n % 3;
	//恰好整除，直接为3^x
	    if (y == 0) return (int)pow((double)3, x);
	//余数为1，退一步 3^(x-1)*2*2
	    if (y == 1) return (int)pow((double)3, x - 1) * 4;
	//余数为2，直接乘以2
	   return (int)pow((double)3, x) * 2;
}

//方法2 动态规划算法
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

