#include "my_linked_list.h"
#ifdef __GNUC__
// 归并排序合并函数
static inline ListNode *linked_list_merge(ListNode *left, ListNode *right)
{
    // 对两条子链进行归并
    ListNode *tmp = (ListNode *)malloc(sizeof(ListNode));
    ListNode *p = tmp;
    while (left && right)
    {
        if (right->data < left->data)
        {
            p->next = right;
            right = right->next;
        }
        else
        {
            p->next = left;
            left = left->next;
        }
        p = p->next;
    }
    if (left != NULL) // 这里不用while循环,因为只剩下一个排序好的链表,直接连接即可
        p->next = left;
    if (right != NULL) // left或者right只能有一个为NULL
        p->next = right;
    p = tmp->next; // tmp本身不装数据,其next为排序后的链表
    free(tmp);
    return p;
}
// 创建链表
LinkedList *linked_list_create()
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->head = list->tail = NULL;
    return list;
}
// 从数组创建链表
LinkedList *linked_list_create_from_array(int *array, int arrayNum)
{
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    list->head = list->tail = linked_list_node_create(array[0]);
    for (int i = 1; i < arrayNum; i++)
    {
        list->tail->next = linked_list_node_create(array[i]);
        list->tail = list->tail->next;
    }
    return list;
}
// 删除链表
void linked_list_free(LinkedList *list)
{
    if (list == NULL)
        return;
    ListNode *cur = list->head, *pre = cur;
    while (cur)
    {
        cur = cur->next;
        free(pre);
        pre = cur;
    }
    free(list);
}
// 打印链表
void linked_list_print(ListNode *head)
{
    while (head)
    {
        printf("%d ", head->data);
        head = head->next;
    }
    putchar('\n');
}
// 节点归并排序
ListNode *linked_list_node_merge_sort(ListNode *head)
{
    // 先判断链表长度是否大于1，小于1时无须排序
    if (head && head->next)
    {
        // !注意这里fast指针一定要是head->next
        ListNode *fast = head->next; // 这样让slow为第n/2个(n为偶数)或第(n+1)/2个(n为奇数),如20个里面排第10,21个里面排第11
        ListNode *slow = head;
        while (fast && fast->next)
        {
            fast = fast->next->next;
            slow = slow->next;
        }
        // 将链表分成两部分进行分割
        ListNode *right = linked_list_node_merge_sort(slow->next); // 排序[slow+1,tail]
        slow->next = NULL;                                         // 这儿很重要，仔细想想为什么
        ListNode *left = linked_list_node_merge_sort(head);        // 排序[head,slow]
        return linked_list_merge(left, right);                     // 合并链表
    }
    return head; // 如果head为NULL或head->next为NULL则直接返回
}
// 链表归并排序
void linked_list_merge_sort(LinkedList *list)
{
    if (list == NULL)
        return;
    list->tail = list->head = linked_list_node_merge_sort(list->head);
    while (list->tail && list->tail->next)
    {
        list->tail = list->tail->next;
    }
}
#endif