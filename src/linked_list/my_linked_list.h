#ifndef __MY__LINKED__LIST__H
#define __MY__LINKED__LIST__H
#ifdef __GNUC__
#include <malloc.h>
#include <stdio.h>
typedef struct __attribute__((__packed__)) ListNode
{
    int data;
    struct ListNode *next;
} ListNode;
typedef struct LinkedList
{
    ListNode *head;
    ListNode *tail;
} LinkedList;
#ifdef __cplusplus
extern "C"
{
#endif
    // 创建节点
    static inline ListNode *linked_list_node_create(int data)
    {
        ListNode *node = (ListNode *)malloc(sizeof(ListNode));
        node->data = data;
        node->next = NULL;
        return node;
    }
    // 在dst之后插入节点src
    static inline void linked_list_node_insert(ListNode *dst, ListNode *src)
    {
        ListNode *tmp = dst->next;
        src->next = tmp;
        dst->next = src;
    }
    // 删除node后的首个节点
    static inline void linked_list_delete(ListNode *node)
    {
        if (node && node->next)
        {
            ListNode *tmp = node->next;
            node->next = tmp->next;
            free(tmp);
        }
    }
    // 访问索引为index的节点
    static inline ListNode *linked_list_access(ListNode *node, int index)
    {
        for (int i = 0; i < index; i++)
        {
            if (node == NULL)
                break;
            node = node->next;
        }
        return node;
    }
    // 向链表尾部加入数据
    static inline void linked_list_push(LinkedList *list, int data)
    {
        ListNode *node = linked_list_node_create(data);
        if (list->tail == NULL)
            list->head = list->tail = node;
        else
        {
            list->tail->next = node;
            list->tail = list->tail->next;
        }
    }
    // 链表节点个数
    static inline int linked_list_getnum(LinkedList *list)
    {
        int num = 0;
        ListNode *node = list->head;
        while (node)
        {
            node = node->next;
            num++;
        }
        return num;
    }
    // 查找值为target的节点
    static inline ListNode *linked_list_find_by_data(ListNode *head, int target)
    {
        while (head)
        {
            if (head->data == target)
                return head;
            head = head->next;
        }
        return NULL;
    }
    // 创建链表
    LinkedList *linked_list_create();
    // 根据数组创建链表
    LinkedList *linked_list_create_from_array(int *array, int arrayNum);
    // 删除链表
    void linked_list_free(LinkedList *list);
    // 打印链表
    void linked_list_print(ListNode *head);
    // 节点归并排序
    ListNode *linked_list_node_merge_sort(ListNode *head);
    // 链表归并排序
    void linked_list_merge_sort(LinkedList *list);
#ifdef __cplusplus
}
#endif
#endif
#endif