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
   
    //·´×ªlist
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

    system("pause");

	return 0;
}

