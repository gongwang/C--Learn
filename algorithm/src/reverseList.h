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

