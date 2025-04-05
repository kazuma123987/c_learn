#ifndef __MY__HEAP__H
#define __MY__HEAP__H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifdef __cplusplus
extern "C"
{
#endif
    typedef struct Heap
    {
        size_t capacity;
        size_t size;
        int data[0];
    } Heap;
    // 获取左子节点索引
    static inline int Heap_left(int index)
    {
        return index * 2 + 1;
    }
    // 获取右子节点索引
    static inline int Heap_right(int index)
    {
        return index * 2 + 2;
    }
    // 获取父节点索引
    static inline int Heap_parent(int index)
    {
        return (index - 1) / 2;
    }
    // 获取堆顶元素
    static inline int Heap_peek(Heap *heap)
    {
        return heap->data[0];
    }
    // 交换堆的元素
    static inline void Heap_Swap(Heap *heap, int index1, int index2)
    {
        int tmp_val = heap->data[index1];
        heap->data[index1] = heap->data[index2];
        heap->data[index2] = tmp_val;
    }
    // 打印堆
    void Heap_print(Heap *heap);
    // 创建堆
    Heap *Heap_create(int capacity);
    Heap *MaxHeap_create_from_vector(int *array, int arrayNum);
    Heap *MinHeap_create_from_vector(int *array, int arrayNum);
    void Heap_realloc(Heap **heap, int capacity);
    void Heap_free(Heap *heap);
    /*####################最大堆####################*/
    // 入堆
    void MaxHeap_push(Heap **heap, int val);
    // 出堆,返回值为堆顶元素
    int MaxHeap_pop(Heap *heap);
    // 从指定索引开始向下堆化
    void MaxHeap_siftDown(Heap *heap, int index);
    // 从指定索引开始向上堆化
    void MaxHeap_siftUp(Heap *heap, int index);

    /*####################最小堆####################*/
    // 入堆
    void MinHeap_push(Heap **heap, int val);
    // 出堆,返回值为堆顶元素
    int MinHeap_pop(Heap *heap);
    // 从指定索引开始向下堆化
    void MinHeap_siftDown(Heap *heap, int index);
    // 从指定索引开始向上堆化
    void MinHeap_siftUp(Heap *heap, int index);

    /*####################堆排序相关####################*/
    void Vector_SiftDown(int *array, int arrayNum, int index);
    void Vector_HeapSort(int *array, int arrayNum);
#ifdef __cplusplus
}
#endif
#endif