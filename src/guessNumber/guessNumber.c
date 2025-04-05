#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
static inline void _my_getint(int *inputNum)
{
    char tmp_ch;
    scanf("%d", inputNum);
    while ((tmp_ch = getchar()) != '\n' && tmp_ch != EOF)
        ; // 读取缓冲区剩余字符串
}
static inline char _my_getchar()
{
    char tmp_ch;
    char ch = getchar();
    while ((tmp_ch = getchar()) != '\n' && tmp_ch != EOF)
        ;
    return ch;
}
void guessNumber()
{
    int n = rand();
    int inputNum;
    char chance = 5;
    printf("############game start###########\n");
    while (1)
    {
        _my_getint(&inputNum);
        if (inputNum > n)
            printf("the number is too big\n");
        else if (inputNum < n)
            printf("the number is too small\n");
        else
        {
            printf("nice answer!\n");
            _my_getchar();
            break;
        }
        chance--;
        if (chance == 0)
        {
            printf("############game over###########\n");
            _my_getchar();
            break;
        }
        printf("you have %d chances to guess\n", chance);
    }
}
int main()
{
    srand(time(0)); // time函数是返回一个值，即格林尼治时间1970年1月1日00:00:00到当前时刻的时长，时长单位是秒
    bool isFirst = true;

    while (1)
    {
        if (isFirst)
        {
            printf("Want to guess a number? Answer y or n.\n");
            isFirst = false;
        }
        else
            printf("Want to guess a number again? Answer y or n.\n");
        char key = _my_getchar();
        if (key == 'n')
            break;
        else if (key == 'y')
            guessNumber();
        else
            printf("input y or n to choose\n");
    }
    return 0;
}