#ifndef __FUNC__POINTER__H
#define __FUNC__POINTER__H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
typedef int (*CALCULATION)(int a,int b);
static int add(int a,int b)
{
    return a+b;
}
static int dec(int a,int b)
{
    return a-b;
}
static int mul(int a,int b)
{
    return a*b;
}
CALCULATION func[3]={add,dec,mul};
#define myadd func[0]
#define mydec func[1]
#define mymul func[2]
//定义函数指针类型(相当于 typedef int* func1)
typedef int (*func1)(int,int);
//定义函数类型(相当于typedef int func2)
typedef int (func2)(int,int);
//定义一个函数的指针(相当于int *func3)
int (*func3)(int,int);
#endif