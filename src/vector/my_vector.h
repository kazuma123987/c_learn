#ifndef MY__VECTOR__H
#define MY__VECTOR__H
#include <malloc.h>
#include "my_compiler.h"
typedef struct Vector
{
    int *data;
    unsigned int size;
    unsigned int capacity;
} Vector;
#ifdef __cplusplus
extern "C"
{
#endif
    // 创建向量(可变数组),capacity:向量的容量
    static inline Vector *vector_create(unsigned int capacity)
    {
        Vector *vector = (Vector *)malloc(sizeof(Vector));
        vector->capacity = capacity;
        vector->size = 0;
        vector->data = (int *)malloc(capacity * sizeof(int));
        if (vector->data != NULL)
            return vector;
        free(vector);
        return NULL;
    }
    // 向量尾部追加元素
    static inline void vector_pushback(Vector *v, int data)
    {
        while (v->size >= v->capacity)
        {
            int *oldData = v->data;
            v->capacity *= 2;
            v->data = (int *)realloc(v->data, sizeof(int) * v->capacity);
            if (v->data == NULL)
            {
                v->data = oldData;
                return;
            }
        }
        v->data[v->size++] = data;
    }
    // 向量尾部弹出元素
    static inline int vector_pop(Vector *v)
    {
        if (v)
            return v->data[--v->size];
        return 0x7fffffff;
    }
    // 清空向量
    static inline void vector_free(Vector **v)
    {
        if (*v)
        {
            free((*v)->data);
            (*v)->data = NULL;
            free(*v);
            *v = NULL;
        }
    }
#ifdef __cplusplus
}
#endif
#endif