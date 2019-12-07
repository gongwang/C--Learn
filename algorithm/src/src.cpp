// src.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "reverseList.h"
#include "TwoListAddFunc.h"
#include "myAtoiFunc.h"
#include "OnceNum.h"
#include "ValidParentheses.h"
//////////////////////////////////////////////////////////////////////////
//单向链表操作
void SingleListTest()
{
    //反转list
    ListNode temp1(1);
    ListNode temp2(2);
    temp2.next = &temp1;
    ListNode temp3(3);
    temp3.next = &temp2;
    printf("temp1: value: %d ptr:%p next: %p \n temp2: value: %d ptr:%p next: %p \n temp3: value: %d ptr:%p next: %p \n", temp1.val, &temp1, temp1.next, 
        temp2.val, &temp2,temp2.next, temp3.val, &temp3, temp3.next);

    Solution slu;

    ListNode *revc = slu.reverseList(&temp3);

    printf("recv value: %d ptr:%p next: %p\n", revc->val, revc, revc->next);
}

//单向链表操作 判断是否有环
void SingleList_HasCycle()
{
    printf("====单向链表操作 判断是否有环===\n");
    ListNode temp1(1);
    ListNode temp2(2);
    temp2.next = &temp1;
    ListNode temp3(3);
    temp3.next = &temp2;
    
    //环
    temp1.next = &temp2;

    printf("temp1: value: %d ptr:%p next: %p \n temp2: value: %d ptr:%p next: %p \n temp3: value: %d ptr:%p next: %p \n", temp1.val, &temp1, temp1.next, 
        temp2.val, &temp2,temp2.next, temp3.val, &temp3, temp3.next);

    Solution slu;

    bool bRet = slu.hasCycle_hash(&temp3);

    printf("hascycle_hash ret: %d \n", bRet);

    bool bRet1 = slu.hasCycle_Fast_Slow_ptr(&temp3);

     printf("hascycle_hash ret: %d \n", bRet1);
}

void freeMemory(ListNode* listNode)
{
    while(listNode->next != NULL)
    {
        freeMemory(listNode->next);
        break;
    }
    delete listNode;
    listNode = NULL;
}

void addTwoList()
{
    ListNode temp1(3);
    ListNode temp2(4);
    temp2.next = &temp1;
    ListNode temp3(2);
    temp3.next = &temp2;

    ListNode temp4(4);
    ListNode temp5(6);
    temp5.next = &temp4;
    ListNode temp6(5);
    temp6.next = &temp5;

    Solution_addtwolist addSlu;
    ListNode * ret = (ListNode*)addSlu.addTwoNumbers(&temp3, &temp6);
    ListNode* freeRet = ret;
    //7 0 9
   while(ret != NULL)
   {
       printf("ret : %d\n", ret->val);
       ret = ret->next;
   }
    
   //释放free 内存 倒序释放内存
   freeMemory(freeRet);

}

//双向链表操作
void DoubleListTest()
{
    //双向链表测试
    printf("========================\n");
    //1.创建
    printf("=========双向链表测试=======\n");
    DoubleListSolution DoubleList;

    pElem head = DoubleList.CreateList();
    //2.插入
    DoubleList.InsertElem(head, 1);
    DoubleList.InsertElem(head, 2);
    DoubleList.InsertElem(head, 3);
    DoubleList.InsertElem(head, 4);
    DoubleList.InsertElem(head, 5);
    //3.轮询
    DoubleList.printfElem(head);

    ////4.查找并删除
    DoubleList.DeleteElem(head, 4);

    DoubleList.printfElem(head);

    //5.反转双向列表

    printf("========反转链表==========\n");
    pElem tempList = DoubleList.reverseList(head);

    DoubleList.printfElem(tempList);

    //释放内存
    free(head);
    free(tempList);

    printf("========================\n");
}

//原型
/*
char *mytest = "-42";
int nIndex = 0;
sscanf(mytest, "%d", &nIndex);
printf("myTest str: %s int: %d\n", mytest, nIndex);
*/

void myAtoi()
{
    SolutionAtoi suatoi;
    //suatoi.myAtoi("0-1");
    //suatoi.myAtoi("42");
    suatoi.myAtoi(" -42");
    //suatoi.myAtoi(" -5-");
}

//判断字符串包含有效括号

bool isValid(string s) {

    if (s.empty())
    {
        return true;
    }

    int nLength = s.length();

    printf("input : %s \n", s.c_str());

    //三个二维数组
    int nAt0 = -999;//(
    int nAt1= -999;//{
    int nAt2= -999;//[

    for (int i = 0; i< nLength; i++)
    {
        char c = s.at(i);

        //()
        if (c == '(' || c == ')')
        {    
            if (c == '(')
            {
                nAt0 = i;
            }else
            {
                if (nAt0>= 0)
                {       
                    int nTemp = i - nAt0;
                    nAt0 = -999;
                    if (nTemp %2 == 0)
                    {
                        printf("nat0 false\n");
                        return false;
                    }
                }else
                {
                    printf("nat0 false\n");
                    return false;
                }

            }
        }
        //{}
        if (c == '{' || c == '}')
        {
            if (c == '{')
            {
                nAt1 = i;
            }else
            {
                if (nAt1>= 0)
                {       
                    int nTemp = i - nAt1;
                    nAt1 = -999;
                    if (nTemp %2 == 0)
                    {
                        printf("nat1 false\n");
                        return false;
                    }
                }else
                {
                    printf("nat1 false\n");
                    return false;
                }

            }      
        }

        //[]
        if (c == '[' || c == ']')
        {
            if (c == '[')
            {
                nAt2 = i;
            }else
            {
                if (nAt2>= 0)
                {       
                    int nTemp = i - nAt2;
                    nAt2 = -999;
                    if (nTemp %2 == 0)
                    {
                        printf("nat2 false\n");
                        return false;
                    }
                }else
                {
                    printf("nat2 false\n");
                    return false;
                }

            }  
        }
    }
    
    if (nAt0 >= 0 || nAt1 >= 0 || nAt2 >= 0)
    {
        printf("return false\n");
        return false;
    }

    printf("return true\n");
    return true;
   
}

bool isValid_stack(string s)
{
      SolutionValidPar su;

      return su.isValid(s);
}

//main 函数
int _tmain(int argc, _TCHAR* argv[])
{
    printf("hello world!\n");
   
    SingleListTest();

    DoubleListTest();
    
    SingleList_HasCycle();

    addTwoList();

    myAtoi();

    //isValid("([)]");

    //isValid("{[]}");

    //isValid("[{()}]");

    isValid_stack("(([]){})");
    
    isValid_stack("()");

    isValid_stack("([)]");

    isValid_stack("{[]}");

    isValid_stack("[{()}]");

    system("pause");

	return 0;
}

