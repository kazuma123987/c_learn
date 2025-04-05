#include "my_sort.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <time.h>
static inline void array_print(int *array, int num)
{
    for (int i = 0; i < num; i++)
        printf("%d ", array[i]);
    printf("\n");
}
static inline void sort_checkerr(int *array, int num, int line)
{
    for (int i = 0; i < num; i++)
        if (array[i] != i + 1)
        {
            printf("error sort in line:%d\n", line);
            break;
        }
}
#define DIV_HASH 100
int hashfunc(int input)
{
    return (input - 1) / DIV_HASH;
}
#define sort_check_error(a, b) sort_checkerr((a), (b), (__LINE__))
#define ARRAR_NUM 100
#define ARRAR_PRINT
void my_sort(int *array, int arrayNum)
{
    heap_sort(array, arrayNum);
}
int main(int argc, char *argv[])
{
    setlocale(LC_CTYPE, ".utf8");
    int *array = (int *)malloc(sizeof(int) * ARRAR_NUM);
    clock_t start;

    //*选择排序
    for (int i = 0; i < ARRAR_NUM; i++)
        array[i] = ARRAR_NUM - i;
    start = clock();
    selection_sort(array, ARRAR_NUM);
    wprintf(L"选择排序测试:%dms\n", clock() - start);
    sort_check_error(array, ARRAR_NUM);
#ifdef ARRAR_PRINT
    array_print(array, ARRAR_NUM);
#endif

    //*冒泡排序
    for (int i = 0; i < ARRAR_NUM; i++)
        array[i] = ARRAR_NUM - i;
    start = clock();
    bubble_sort(array, ARRAR_NUM);
    wprintf(L"冒泡排序测试:%dms\n", clock() - start);
    sort_check_error(array, ARRAR_NUM);
#ifdef ARRAR_PRINT
    array_print(array, ARRAR_NUM);
#endif

    //*插入排序
    for (int i = 0; i < ARRAR_NUM; i++)
        array[i] = ARRAR_NUM - i;
    start = clock();
    insertion_sort(array, ARRAR_NUM);
    wprintf(L"插入排序测试:%dms\n", clock() - start);
    sort_check_error(array, ARRAR_NUM);
#ifdef ARRAR_PRINT
    array_print(array, ARRAR_NUM);
#endif

    //*快速排序
    for (int i = 0; i < ARRAR_NUM; i++)
        array[i] = ARRAR_NUM - i;
    start = clock();
    quick_sort(array, 0, ARRAR_NUM - 1);
    wprintf(L"快速排序测试:%dms\n", clock() - start);
    sort_check_error(array, ARRAR_NUM);
#ifdef ARRAR_PRINT
    array_print(array, ARRAR_NUM);
#endif

    //*堆排序
    for (int i = 0; i < ARRAR_NUM; i++)
        array[i] = ARRAR_NUM - i;
    start = clock();
    heap_sort(array, ARRAR_NUM);
    wprintf(L"堆排序测试:%dms\n", clock() - start);
    sort_check_error(array, ARRAR_NUM);
#ifdef ARRAR_PRINT
    array_print(array, ARRAR_NUM);
#endif

    //*归并排序
    for (int i = 0; i < ARRAR_NUM; i++)
        array[i] = ARRAR_NUM - i;
    start = clock();
    merge_sort(array, 0, ARRAR_NUM - 1);
    wprintf(L"归并排序测试:%dms\n", clock() - start);
    sort_check_error(array, ARRAR_NUM);
#ifdef ARRAR_PRINT
    array_print(array, ARRAR_NUM);
#endif

    //*桶排序
    for (int i = 0; i < ARRAR_NUM; i++)
        array[i] = ARRAR_NUM - i;
    start = clock();
    bucket_sort(array, ARRAR_NUM, ARRAR_NUM / DIV_HASH, hashfunc);
    wprintf(L"桶排序测试:%dms\n", clock() - start);
    sort_check_error(array, ARRAR_NUM);
#ifdef ARRAR_PRINT
    array_print(array, ARRAR_NUM);
#endif

    //*计数排序
    for (int i = 0; i < ARRAR_NUM; i++)
        array[i] = ARRAR_NUM - i;
    start = clock();
    counting_sort(array, ARRAR_NUM);
    wprintf(L"计数排序测试:%dms\n", clock() - start);
    sort_check_error(array, ARRAR_NUM);
#ifdef ARRAR_PRINT
    array_print(array, ARRAR_NUM);
#endif

    //*基数排序
    for (int i = 0; i < ARRAR_NUM; i++)
        array[i] = ARRAR_NUM - i;
    start = clock();
    radix_sort(array, ARRAR_NUM);
    wprintf(L"基数排序测试:%dms\n", clock() - start);
    sort_check_error(array, ARRAR_NUM);
#ifdef ARRAR_PRINT
    array_print(array, ARRAR_NUM);
#endif

    free(array);
    return 0;
}
