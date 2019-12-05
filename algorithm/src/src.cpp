// src.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "reverseList.h"
#include "TwoListAddFunc.h"
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
    free(listNode);
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

//main 函数
int _tmain(int argc, _TCHAR* argv[])
{
    printf("hello world!\n");
   
    SingleListTest();

    DoubleListTest();
    
    SingleList_HasCycle();

    addTwoList();

    system("pause");

	return 0;
}

