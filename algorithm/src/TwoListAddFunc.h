/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };

 1.给出两个 非空 的链表用来表示两个非负的整数。其中，它们各自的位数是按照 逆序 的方式存储的，并且它们的每个节点只能存储 一位 数字。

 如果，我们将这两个数相加起来，则会返回一个新的链表来表示它们的和。

 您可以假设除了数字 0 之外，这两个数都不会以 0 开头。

 示例：

 输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)
 输出：7 -> 0 -> 8
 原因：342 + 465 = 807

 来源：力扣（LeetCode）
 链接：https://leetcode-cn.com/problems/add-two-numbers
 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

 */
#pragma once

class Solution_addtwolist {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        
        //循环终止条件，所有都为null

        //特殊条件 1的长度 大于2 或者1小于2
        if (l1 == NULL || l2 == NULL)
        {
            return NULL;
        }
        ListNode* line1 = l1;
        ListNode* line2 = l2;

        ListNode *retNode = (ListNode*)malloc(sizeof(ListNode));
        retNode->val = NULL;
        retNode->next = NULL;

        ListNode *retNodeOld = (ListNode*)malloc(sizeof(ListNode));
        retNodeOld->val  = NULL;
        retNodeOld->next = NULL;

        while (line1 != NULL || line2 != NULL)
        {
            ListNode *tempNode = (ListNode*)malloc(sizeof(ListNode));
            tempNode->val = NULL;
            tempNode->next = NULL;

            if (line1 == NULL)
            {
                tempNode->val = line2->val;
                line2 = line2->next;
            }else if(line2 == NULL)
            {
                tempNode->val = line1->val;
                line1 = line1->next;
            }else
            {
                //如果上一个 相加大于10 则本次应多+1
                if (retNodeOld->val == 1)
                {
                     tempNode->val = line1->val + line2->val + retNodeOld->val;
                }else
                {
                     tempNode->val = line1->val + line2->val;
                }
               
                line1 = line1->next;
                line2 = line2->next;
            }

            if (retNodeOld->next != NULL)
            {
                ListNode* temp = (ListNode*)retNodeOld->next; //解指针 将next赋值
                temp->next = tempNode;
            }
            
            retNodeOld->next = tempNode;

            if (tempNode->val >9)
            {
                tempNode->val = 0;
                retNodeOld->val = 1; //赋值
            }

            if(retNode->next == NULL)retNode->next = tempNode;

            //
        }
        return retNode->next;

        free(retNode);
        free(retNodeOld);
    }
};