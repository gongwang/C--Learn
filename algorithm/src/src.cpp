// src.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////
#include "reverseList.h"

//////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
    printf("hello world!\n");
   
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
    system("pause");

	return 0;
}

