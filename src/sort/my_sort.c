#include "my_sort.h"
//*###########################################################################*//
//*################################辅助函数部分################################*//
//*###########################################################################*//
// 交换索引
static inline void swap(int *array, int index1, int index2)
{
    int tmp_val = array[index1];
    array[index1] = array[index2];
    array[index2] = tmp_val;
}
// 取三个数的中位数
static inline int mid_of_three(int *array, int index1, int index2, int index3)
{
    // 此处使用异或运算来简化代码(一真一假则为真,两真或两假都为假)
    // 异或规则为 0 ^ 0 = 1 ^ 1 = 0, 0 ^ 1 = 1 ^ 0 = 1
    if ((array[index1] < array[index2]) ^ (array[index1] < array[index3]))
        return index1;
    else if ((array[index2] < array[index1]) ^ (array[index2] < array[index3]))
        return index2;
    else
        return index3;
}
// 堆排序辅助函数(从指定索引开始向下堆化)
static inline void Vector_SiftDown(int *array, int arrayNum, int index)
{
    while (1)
    {
        int left = index * 2 + 1;
        int right = left + 1;
        int max_index = index;
        if (left < arrayNum && array[max_index] < array[left])
            max_index = left;
        if (right < arrayNum && array[max_index] < array[right])
            max_index = right;
        if (max_index == index)
            break;
        int tmp_val = array[index];
        array[index] = array[max_index];
        array[max_index] = tmp_val;
        index = max_index;
    }
}
// 快速排序辅助函数
static inline int partition(int *array, int left, int right)
{
    int i = left;
    int j = right;
    int mid = mid_of_three(array, left, (left + right) / 2, right); // 把数组左边中间和右边的数的中位数作为基准
    swap(array, left, mid);                                         // 把基准交换到最左边
    while (i < j)
    {
        while (i < j && array[j] >= array[left]) // 从右到左找第一个小于基准值的索引
            j--;
        while (i < j && array[i] <= array[left]) // 从左到右找第一个大于基准值的索引
            i++;
        swap(array, i, j); // 交换两者
    }
    swap(array, i, left); // 交换基准值与i(接近但小于等于基准值)对应的值
    return i;             // 返回基准值索引
}
static inline void mergeArray(int *array, int left, int mid, int right)
{
    int tmpNum = right - left + 1;                  // 数组的元素数量
    int *tmp = (int *)malloc(sizeof(int) * tmpNum); // 分配临时数组
    int i = left, j = mid + 1, k = 0;               // 数组范围是[left,mid],[mid+1,right]
    while (i <= mid && j <= right)                  // tmp数组存放两数组最小值(能这样做是因为两数组都按从小到大排序好了)
        tmp[k++] = (array[i] <= array[j]) ? array[i++] : array[j++];
    while (i <= mid) // 如果j在while (i <= mid && j <= right)循环中存放完了则存放i剩余元素
        tmp[k++] = array[i++];
    while (j <= right) // 如果i在while (i <= mid && j <= right)循环中存放完了则存放j剩余元素
        tmp[k++] = array[j++];
    memcpy(array + left, tmp, tmpNum * sizeof(int)); // 把排序好的数组复原回去
    free(tmp);
}
int compare_int(const void *a, const void *b)
{
    return (*(int *)a > *(int *)b) ? 1 : -1;
}
// 获取数data的第k位,这里exp=10^(k-1),data为10进制
static inline int getBit(int data, int exp)
{
    return (data / exp) % 10;
}
void counting_sort_radix(int *array, int arrayNum, int exp)
{
    //! 一定要用calloc把counter置零
    int *counter = (int *)calloc(10, sizeof(int)); // 0-9十个数字
    // 统计第k位0-9出现的次数,exp=10^(k-1)
    for (int i = 0; i < arrayNum; i++)
    {
        int bit = getBit(array[i], exp);
        counter[bit]++;
    }
    // 计算前缀和
    for (int i = 0; i < 10 - 1; i++)
        counter[i + 1] += counter[i];
    // 按前缀倒序排序
    int *tmp = (int *)malloc(sizeof(int) * arrayNum);
    for (int i = arrayNum - 1; i >= 0; i--)
    {
        int index = getBit(array[i], exp);
        tmp[--counter[index]] = array[i];
    }
    // 复制排序结构到原数组
    memcpy(array, tmp, sizeof(int) * arrayNum);
    // 清除空间
    free(counter);
    free(tmp);
}
//*###########################################################################*//
//*################################排序函数部分################################*//
//*###########################################################################*//

// 选择排序(selection sort)
void selection_sort(int *array, int arrayNum)
{
    for (int i = 0; i < arrayNum - 1; i++) // 执行n-1轮排序
    {
        int min_index = i;
        for (int j = i + 1; j < arrayNum; j++)
        {
            if (array[j] < array[min_index])
                min_index = j;
        }
        if (min_index != i)
            swap(array, min_index, i);
    }
}
// 堆排序(heap sort)
void heap_sort(int *array, int arrayNum)
{
    // 建立大顶堆
    for (int i = (arrayNum - 2) / 2; i >= 0; i--)
        Vector_SiftDown(array, arrayNum, i);
    for (int i = arrayNum - 1; i > 0; i--)
    {
        // 交换首尾
        swap(array, 0, i);
        // 对数量为i的堆进行堆化(这样堆顶又是最大值)
        Vector_SiftDown(array, i, 0);
    }
}
// 冒泡排序(bubble sort)
void bubble_sort(int *array, int arrayNum)
{
    for (int i = arrayNum - 1; i > 0; i--) // i的范围[n-1,1]
    {
        char flag = 1;              // 定义一个flag,如果本轮冒泡有进行交换则置0
        for (int j = 0; j < i; j++) // 排序区间为[0,i],j的范围[0,i-1]
        {
            if (array[j] > array[j + 1])
            {
                swap(array, j, j + 1);
                flag = 0;
            }
        }
        if (flag)
            break; // 如果本轮冒泡没有进行交换则结束冒泡
    }
}
// 插入排序(insertion sort)
void insertion_sort(int *array, int arrayNum)
{
    for (int i = 1; i < arrayNum; i++) // i代表base的索引,范围[1,n-1]
    {
        int base = array[i];              // 先把要插入的数存在base中
        int j = i - 1;                    // j的范围:base前一个元素~第一个元素
        while (j >= 0 && base < array[j]) // 要插入的索引(j+1)最开始是base的索引,如果base更小,则要插入的索引-1
        {
            array[j + 1] = array[j]; // 如果base更小,则向右赋值,给要插入的base腾出空间
            j--;
        }
        array[j + 1] = base; // 把base插入到正确的位置
    }
}
// 快速排序(quick sort)
void quick_sort(int *array, int left, int right)
{
    while (left < right)
    {
        int baseIndex = partition(array, left, right);
        if (baseIndex - left > right - baseIndex) // 只递归排序长数组
        {
            quick_sort(array, left, baseIndex - 1); // 排序[left,baseIndex-1]
            left = baseIndex + 1;                   // 未排序[baseIndex+1,right],则让left=baseIndex+1,下次循环即可进行排序
        }
        else
        {
            quick_sort(array, baseIndex + 1, right); // 排序[baseIndex+1,right]
            right = baseIndex - 1;                   // 未排序[left,baseIndex-1],则让right = baseIndex - 1,下次循环即可进行排序
        }
    }
}
// 归并排序(merge sort)
void merge_sort(int *array, int left, int right)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2; // 计算中点
    merge_sort(array, left, mid);
    merge_sort(array, mid + 1, right);
    mergeArray(array, left, mid, right); // 后序遍历进行合并
}
// 桶排序(bucket sort)
void bucket_sort(int *array, int arrayNum, int bucketNum, hashfuncInt function)
{
    // 分配空间
    int *caps = (int *)malloc(bucketNum * sizeof(int));
    int *size = (int *)calloc(bucketNum, sizeof(int));
    int **buckets = (int **)malloc(sizeof(int *) * bucketNum);
    for (int i = 0; i < bucketNum; i++)
    {
        caps[i] = 1;
        buckets[i] = (int *)malloc(sizeof(int));
    }
    // 装入桶
    for (int i = 0; i < arrayNum; i++)
    {
        int index = function(array[i]);
        if (caps[index] <= size[index])
        {
            caps[index] *= 2;
            buckets[index] = (int *)realloc(buckets[index], sizeof(int) * caps[index]);
        }
        buckets[index][size[index]++] = array[i];
    }
    // 排序
    for (int i = 0; i < bucketNum; i++)
        qsort(buckets[i], size[i], sizeof(int), compare_int);
    // 合并数据
    int k = 0;
    for (int i = 0; i < bucketNum; i++)
    {
        memcpy(array + k, buckets[i], sizeof(int) * size[i]);
        k += size[i];
    }
    // 清除数据
    for (int i = 0; i < bucketNum; i++)
        free(buckets[i]);
    free(buckets);
    free(size);
    free(caps);
}
// 计数排序(counting sort)
void counting_sort(int *array, int arrayNum)
{
    // 获取数组最大最小值
    int max = array[0], min = max;
    for (int i = 0; i < arrayNum; i++)
    {
        if (array[i] > max)
            max = array[i];
        if (array[i] < min)
            min = array[i];
    }
    // //! 一定要用calloc把counter置零
    unsigned int size = max - min + 1;
    int *counter = (int *)calloc(size, sizeof(int)); // index范围为[min,max]->[0,max-min]
    // 统计元素的值出现的次数,并放入counter中,因为统计时是按array顺序统计的,所以如果index=counter[array[i]-min]-1,那index=0处存放的就是
    // array[i],index=1处存放的就是排列在array[i]后面并且与其值相等的元素
    for (int i = 0; i < arrayNum; i++)
        counter[array[i] - min]++;
    // 计算前i个数的和(包括自身)放在counter中
    for (int i = 0; i < size - 1; i++)
        counter[i + 1] += counter[i];
    // 倒叙遍历array数组,通过前缀索引排序到临时数组中
    int *res = (int *)malloc(sizeof(int) * arrayNum);
    for (int i = size - 1; i >= 0; i--)
    {
        int index = array[i] - min;
        res[--counter[index]] = array[i];
    }
    // 复制排序好的数据到原数组中
    memcpy(array, res, sizeof(int) * arrayNum);
    // 清除空间
    free(counter);
    free(res);
}
// 基数排序(radix sort)
void radix_sort(int *array, int arrayNum)
{
    // 获取最大值,以方便进行移位操作
    int max = array[0];
    for (int i = 1; i < arrayNum; i++)
        if (array[i] > max)
            max = array[i];
    for (int exp = 1; exp <= max; exp *= 10)
        counting_sort_radix(array, arrayNum, exp);
}