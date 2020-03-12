## 206. Reverse Linked List
##### 2018-03-30 16:22:31
##### 链表反转
***
## 1.题目
将一个链表反转，返回反转后的头，有递归与迭代两种写法。
## 2.分析
个人认为链表已经稍微掌握些了，但是看到这个题目之后没有什么直接的想法，更不敢说递归。  
看了别人的代码，解释下原理：  
#### 📌迭代：
1. 首先排除掉输入链表为空的情况。
2. 当输入不为空时，新建一个节点指针p，值为head->next,接着将head指向的元素改为空。
3. 用while对p进行判断，非空执行增加头节点的操作，注意保存p->next，每次循环结束之后将p的值改为p->next。
4. 返回头。

原理为遍历链表，不断添加头节点。
#### 📌递归
递归比较难理解，也比较难以叙述，参照代码，将运行过程在纸上写一下。
## 3.代码
```c
struct ListNode * reverseList(struct ListNode *head){
  if(head==NULL)
    return head;
  struct ListNode *p=head->next;  //不用开辟空间
  head->next=NULL;
  while(p){
    struct ListNode*temp=p->next; //暂存
    p->next=head;
    head=p;
    p=temp;
  }
  return head;
}

//递归
struct ListNode *reverseList(struct ListNode *head){
  if(head==NULL || head->next==NULL)
    return head;  //递归写法不能像迭代一样只考虑head=0
  struct ListNode *p=head->next;
  head->next=NULL;
  struct ListNode *newhead=reverseList(p);  //目的是制造最终的头
  p->next=head; //链接,因为p值不变，该语句将节点链接了起来
  return newhead;
}
```

***
2020年3月2日更新

### 笨方法递归

重新做这个题目，最佳的递归做法还是没有立刻想起来，写了下面的做法，执行用了32ms。思路如下：

递归的结果是返回新的链表头，递归基显然是遇到空指针或者当前节点的下一个节点为空（即链表尾部）。剩下的部分是如何对递归过程中经过的结点进行处理，因为递归结果会返回新的链表头，当前节点应该拼接到新的链表结构的最后位置，很容易想到的粗暴方法是直接遍历链表进行拼接。

### 递归优化

但很明显上述方法不能有效利用指针的特点，每处理一个节点都需要一次遍历列表。官方题解给出了更有效的解法，递归基不变，在处理每一个过程节点时，设有两个连续的节点l、r，过程如下（处理l）：

1. 当前节点的下一个节点的next指向自身。因为原链表是递归处理的，当处理l时，r节点一定被提前处理过，而按照反转链表的目的，l节点应该作为r节点的next元素。
2. 将l节点的next置空，因为第一步已经将l放到r的后面了，但原有的关系没变，二者形成了环，需要将l节点的next置空将环破坏。这也使l真正位于了链表的尾部。

### 再优化（雾）

仔细思考，上述步骤2是可以取消执行的，因为下一个递归处理节点的步骤1会覆盖当前节点的步骤2，除了原有头部节点。将步骤简化可以有第三种写法，此时需要一个全局变量，且需要辅助函数。虽然看起来没有第二种方法简单，但是每次处理节点都会少一次幅值。但最后的执行时间还是8ms。

### 其他

最后写了一种while循环遍历一次改变指向的方法，最后执行时间是12ms，更慢了。

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if(head == NULL || head->next == NULL)
            return head;
        ListNode* nextNode = reverseList(head->next);
        ListNode* tmp = head;
        tmp->next = NULL;
        head = nextNode;
        while(nextNode->next){
            nextNode = nextNode->next;
        }
        nextNode->next = tmp;
        return head;
    }

    // 递归
    ListNode* reverseList2(ListNode* head) {
        if(head == NULL || head->next == NULL)
            return head;
        ListNode* newHead = reverseList(head->next);
        
        head->next->next = head;
        head->next = NULL;
        return newHead;
    }

    // 简化
    ListNode* res;
    void helper(ListNode* head){
        if(head->next == NULL){
            res = head;
            return ;
        }
        helper(head->next);
        head->next->next = head;
    }
    ListNode* reverseList3(ListNode* head) {
        if(!head || head->next == NULL)
            return head;
        helper(head);
        head->next = NULL;  //需要特殊处理
        return res;
    }

    // while循环
    ListNode* reverseList4(ListNode* head) {
        if(!head)
            return head;
        ListNode* cur = head->next;
        head->next = NULL;
        while(cur){
            ListNode* tmp = cur->next;
            cur->next = head;
            head = cur;
            cur = tmp;
        }
        return head;
    }
};


```
