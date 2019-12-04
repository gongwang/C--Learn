/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };

 方法一：迭代
 假设存在链表 1 → 2 → 3 → Ø，我们想要把它改成 Ø ← 1 ← 2 ← 3。

 在遍历列表时，将当前节点的 next 指针改为指向前一个元素。由于节点没有引用其上一个节点，因此必须事先存储其前一个元素。在更改引用之前，还需要另一个指针来存储下一个节点。不要忘记在最后返回新的头引用！

 作者：LeetCode
 链接：https://leetcode-cn.com/problems/reverse-linked-list/solution/fan-zhuan-lian-biao-by-leetcode/
 来源：力扣（LeetCode）
 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

 复杂度分析

 时间复杂度：O(n)O(n)，假设 nn 是列表的长度，时间复杂度是 O(n)O(n)。
 空间复杂度：O(1)O(1)。

 */
#include <assert.h>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};


class Solution {
public:
    ListNode* reverseList(ListNode* head) {
            
        ListNode *pre = NULL;
        ListNode *curr = head;

        while(curr != NULL)
        {
            ListNode *temp = curr->next;
            
            curr->next = pre; //先将current node的前节点赋值
            
            pre = curr; //保存 当前节点的值 给 pre节点
             
            curr = temp; //最后将下一节点 赋值到当前节点
        }
        return pre;
    }
};

//双向链表反转

struct DoubleListNode {
    int val;
    DoubleListNode *pre;
    DoubleListNode *next;
};

typedef struct DoubleListNode* pElem;
typedef struct DoubleListNode eElem;

class DoubleListSolution {
public:
    //1.创建双向链表
    pElem CreateList()
    {
        pElem head = (pElem)malloc(sizeof(eElem));
        assert(head != NULL);

        head->next = head->pre = NULL;

        return head;
    }
    //2.插入数据
    void InsertElem(pElem head, int date)
    {
        if (head == NULL)
        {
            printf("head is null\n");
            return;
        }
        
        pElem tempHead = head;//临时头

        if (tempHead->next == NULL)
        {
            /*  当双向链表只有一个头结点时 */
            pElem addition = (pElem)malloc(sizeof(eElem));

            addition->val = date;
            addition->next = tempHead->next;
            addition->pre = tempHead;
            tempHead->next = addition;         
        }else
        {
            /* 当双向链表不只一个头结点时 超过1个节点*/
            pElem addition = (pElem)malloc(sizeof(eElem));

            addition->val = date;

            addition->next = head->next;
            head->next->pre = addition;
            head->next = addition;
            addition->pre = head;                  
        }
    }

    //3.遍历链表 输出打印
    void printfElem(pElem head)
    {
        if (head == NULL)
        {
            printf("head is null\n");
            return;
        }

        pElem tempNode = head;

        while(tempNode->next != NULL)
        {                 
            tempNode = tempNode->next;
            printf("list[%p]:[%d]\n",tempNode, tempNode->val);
        }
      
    }

    //4.删除
    BOOL DeleteElem(pElem head, int data)
    {
        BOOL bRet = FALSE;

        if (head == NULL)
        {
            printf("head is null\n");
            return bRet;
        }

        pElem tempHead = head;

        while(tempHead->next != NULL)
        {
            tempHead = tempHead->next;

            if (tempHead->val == data)
            {
                tempHead->pre->next = tempHead->next;
                tempHead->next->pre = tempHead->pre; 
                printf("delete succ: %d \n", data);
                bRet = TRUE;
                break;
            }
        }
        free(tempHead);
        return bRet;
    }
    
    //5.反转双向列表
    //取出每个值，然后按照插入，从头开始插，倒序
    DoubleListNode* reverseList(DoubleListNode* head) 
    {
        if (head == NULL)
        {
            printf("head is null\n");
            return NULL;
        }

        pElem tempHead = head;

        pElem temp = CreateList();

        while(tempHead->next != NULL)
        {
            tempHead = tempHead->next;

            //重新插入
            InsertElem(temp, tempHead->val);
        }
        return temp;
    }
    
};

//class Solution {
//public:
//    ListNode* reverseList(ListNode* head) {
//            ListNode *prev = NULL;
//            ListNode *curr = head;
//            while (curr != NULL) 
//            {
//                ListNode *nextTemp = curr->next;
//                curr->next = prev;
//                prev = curr;
//                curr = nextTemp;
//            }
//            return prev;
//    }
//};

