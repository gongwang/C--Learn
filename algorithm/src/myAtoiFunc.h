/************************************************************************/
/* 

此题傻逼 不要继续做了！

请你来实现一个 atoi 函数，使其能将字符串转换成整数。

首先，该函数会根据需要丢弃无用的开头空格字符，直到寻找到第一个非空格的字符为止。

当我们寻找到的第一个非空字符为正或者负号时，则将该符号与之后面尽可能多的连续数字组合起来，作为该整数的正负号；假如第一个非空字符是数字，则直接将其与之后连续的数字字符组合起来，形成整数。

该字符串除了有效的整数部分之后也可能会存在多余的字符，这些字符可以被忽略，它们对于函数不应该造成影响。

注意：假如该字符串中的第一个非空格字符不是一个有效整数字符、字符串为空或字符串仅包含空白字符时，则你的函数不需要进行转换。

在任何情况下，若函数不能进行有效的转换时，请返回 0。

说明：

假设我们的环境只能存储 32 位大小的有符号整数，那么其数值范围为 [−231,  231 − 1]。如果数值超过这个范围，请返回  INT_MAX (231 − 1) 或 INT_MIN (−231) 。

示例 1:

输入: "42"
输出: 42
示例 2:

输入: "   -42"
输出: -42
解释: 第一个非空白字符为 '-', 它是一个负号。
     我们尽可能将负号与后面所有连续出现的数字组合起来，最后得到 -42 。
     示例 3:

     输入: "4193 with words"
     输出: 4193
     解释: 转换截止于数字 '3' ，因为它的下一个字符不为数字。
     示例 4:

     输入: "words and 987"
     输出: 0
     解释: 第一个非空字符是 'w', 但它不是数字或正、负号。
     因此无法执行有效的转换。
     示例 5:

     输入: "-91283472332"
     输出: -2147483648
     解释: 数字 "-91283472332" 超过 32 位有符号整数范围。 
          因此返回 INT_MIN (−231) 。
    
    来源：力扣（LeetCode）
    链接：https://leetcode-cn.com/problems/string-to-integer-atoi
    著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

*/
/************************************************************************/
/************************************************************************/
/* 
char *mytest = "-42";
int nIndex = 0;
sscanf(mytest, "%d", &nIndex);
printf("myTest str: %s int: %d\n", mytest, nIndex);           */
/************************************************************************/

/************************************************************************/
/* 特殊情况
input: -when are you read 12345
output: 12345
*/
/************************************************************************/

class SolutionAtoi {
public:
    int myAtoi(string str) {

        if (str.empty())
        {
            printf("input param is empty\n");
            return NULL;
        }

        void* pBegin = NULL; //指针起始位置
        int nBeginIndex = -1;
        int nCount = NULL;

        const char *cStr = str.c_str(); //转成 const char*

        for (int nIndex = 0; nIndex < strlen(cStr); nIndex++)
        {
            //////////////////////////////////////////////////////////////////////////
            //取任意字符串中的数字
            //if ((nIndex< strlen(cStr) -1)&&(cStr[nIndex] == '-' && (cStr[nIndex+1] < '9' +1 && cStr[nIndex+1] > '0' -1) ) )//如果是“-1...”开头 
            //{
            //    pBegin = (void*)&cStr[nIndex];
            //    nBeginIndex = nIndex;
            //}

            //if ((pBegin == NULL) && (cStr[nIndex] < '9' +1 && cStr[nIndex] > '0' -1))//如果“1..." 开头  已经计算开头位置
            //{
            //    pBegin = (void*)&cStr[nIndex];
            //    nBeginIndex = nIndex;
            //}
            //////////////////////////////////////////////////////////////////////////
            //取只有开头位置为 ‘-’‘0-9’前边可能有空格

            if (cStr[nIndex] == ' ' && pBegin == NULL) //如果开头位置为空 并且没有找到头 直接continue；
            {
                continue;
            }

            if ((pBegin == NULL) && ((cStr[nIndex] < '9' +1 && cStr[nIndex] > '0' -1) || cStr[nIndex] == '-' || cStr[nIndex] == '+'))//如果“1..." 开头  已经计算开头位置
            {
                pBegin = (void*)&cStr[nIndex];
                nBeginIndex = nIndex;
            }

            if (pBegin == NULL)
            {
                printf("开头不是'-'或者'0-9' \n");
                return NULL;
            }

            //////////////////////////////////////////////////////////////////////////

            if ((pBegin != NULL && cStr[nIndex] != '-' && cStr[nIndex] != '+')&&(cStr[nIndex]<'0'-1 || cStr[nIndex]>'9'+1 || cStr[nIndex+1] == '\0')) //如果已经找到开头，而之后不是0-9之间的树，则认为已经到结尾
            {
                nCount = nIndex - nBeginIndex + 1; //计算字符串大小
            }
            if (nCount)break; //如果已经找到，则退出
        }

        if(pBegin == NULL || nCount == NULL)
        {
            return 0;
        }

        char* cRet = new char[nCount +1];
        memcpy(cRet, pBegin, nCount);

        printf("cRet : %s\n", cRet);
       
        long long nRet = NULL;
        sscanf(cRet, "%lld", &nRet);
        delete[] cRet;
        cRet = NULL;

        if (nRet <INT_MIN  )
        {
            nRet = INT_MIN; 
        }
        if (nRet > INT_MAX )
        {
            nRet= INT_MAX -1;
        }
        printf("nRet: %d\n", nRet);
        return nRet;
    }
};