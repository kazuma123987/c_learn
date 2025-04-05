#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
void myscanf(int *num, int size)
{
    char str[20];
    fgets(str, size + 1, stdin); // 只读取指定长度的字符串
    if (str[strcspn(str, "\n")] == '\n')
        str[strcspn(str, "\n")] = '\0'; // 将读取字符串的换行符去掉
    else
        while (getchar() != '\n')
            ; // 读取缓冲区剩余字符串
    *num = atoi(str);
}
int find_line(int array_len, int **array, bool x)
{
    int ret = 0;
    char str[2];
    if (x)
        strcpy(str, "O");
    else
        strcpy(str, "X");
    // 行遍历
    for (int i = 0; i < array_len; i++)
    {
        int t = x;
        for (int j = 0; j < array_len; j++)
        {
            if (array[i][j] == 2)
            {
                t = !x;
                break;
            }
            if (x)
                t = (t && array[i][j]);
            else
                t = (t || array[i][j]);
            if (t != x)
                break;
        }
        if (t == x)
        {
            wprintf(L"第%d行的%s连成一线\n", i + 1, str);
            ret = 1;
            goto Ret;
        }
    }
    // 列遍历
    for (int i = 0; i < array_len; i++)
    {
        int t = x;
        for (int j = 0; j < array_len; j++)
        {
            if (array[j][i] == 2)
            {
                t = !x;
                break;
            }
            if (x)
                t = (t && array[j][i]);
            else
                t = (t || array[j][i]);
            if (t == !x)
                break;
        }
        if (t == x)
        {
            wprintf(L"第%d列的%s连成一线\n", i + 1, str);
            ret = 1;
            goto Ret;
        }
    }
    // 左对角线遍历
    for (int i = 0, t = x; i < array_len; i++)
    {
        if (array[i][array_len - 1 - i] == 2)
        {
            t = !x;
            break;
        }
        if (x)
            t = (t && array[i][array_len - 1 - i]);
        else
            t = (t || array[i][array_len - 1 - i]);
        if (t == !x)
            break;
        if ((i == array_len - 1) && t == x)
        {
            wprintf(L"左对角线的%s连成一线\n", str);
            ret = 1;
            goto Ret;
        }
    }
    // 右对角线遍历
    for (int i = 0, t = x; i < array_len; i++)
    {
        if (array[i][i] == 2)
        {
            t = !x;
            break;
        }
        if (x)
            t = (t && array[i][i]);
        else
            t = (t || array[i][i]);
        if (t == !x)
            break;
        if (i == array_len - 1 && t == x)
        {
            wprintf(L"右对角线的%s连成一线\n", str);
            ret = 1;
            goto Ret;
        }
    }
Ret:
    if (ret == 1)
        wprintf(L"%s方获胜!\n", str);
    return ret;
}
void print_chess(int N, int **array)
{
    wprintf(L"棋盘如下图所示：\n");
    wprintf(L"--------------------------begin--------------------------\n");
    wprintf(L"  ");
    for (int i = 1; i <= N; i++)
        wprintf(L"%d ", i);
    wprintf(L"\n");
    for (int i = 0; i < N; i++)
    {
        wprintf(L"%d ", i + 1);
        for (int j = 0; j < N; j++)
        {
            if (array[i][j] == 1)
                wprintf(L"O ");
            else if (array[i][j] == 0)
                wprintf(L"X ");
            else
                wprintf(L"- ");
        }
        wprintf(L"\n");
    }
    wprintf(L"---------------------------end---------------------------\n\n\n");
}
void game()
{
    // 棋盘数组大小定义和初始化
    int N = 1;
    wprintf(L"请输入棋盘的行数（或列数）N:(1<N<15)\n");
    while (N <= 1 || N >= 15)
    {
        myscanf(&N, 1);
        if (N <= 1 || N >= 15)
            wprintf(L"invalid number.\n");
    }
    int **array = (int **)malloc(sizeof(int *) * N);
    for (int i = 0; i < N; i++)
        array[i] = (int *)malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            array[i][j] = 2;
        }
    }
    // 先手决定
    int chess_order = 2;
    wprintf(L"请输入1表示O方为先手,或输入0表示X方为先手:\n");
    while (chess_order != 0 && chess_order != 1)
    {
        myscanf(&chess_order, 1);
        if (chess_order != 0 && chess_order != 1)
            wprintf(L"无效的输入,请输入0或1\n");
    }
    // 下棋阶段
    int chess_line, chess_row;
    int chess_count = 0;
    print_chess(N, array);
    while (chess_count != N * N)
    {
        if (chess_order)
            wprintf(L"请输入O方要下在几行几列\n");
        else
            wprintf(L"请输入X方要下在几行几列\n");
        myscanf(&chess_line, 1);
        myscanf(&chess_row, 1);
        if (chess_line > 0 && chess_line <= N && chess_row > 0 && chess_row <= N)
        {
            if (array[chess_line - 1][chess_row - 1] == 2)
            {
                if (chess_order)
                {
                    array[chess_line - 1][chess_row - 1] = 1;
                    chess_order = !chess_order;
                    chess_count++;
                }
                else
                {
                    array[chess_line - 1][chess_row - 1] = 0;
                    chess_order = !chess_order;
                    chess_count++;
                }
            }
            else
            {
                wprintf(L"\n\n此处已存在棋子了!\n\n\n");
                continue;
            }
        }
        else
        {
            wprintf(L"\n\nerror number!\n\n\n");
            continue;
        }
        print_chess(N, array);
        if (find_line(N, array, !chess_order))
            break;
    }
    for (int i = 0; i < N; i++)
        free(array[i]);
    free(array);
}

int main()
{
    setlocale(LC_CTYPE, "");
    game();
    getchar();
    return 0;
}