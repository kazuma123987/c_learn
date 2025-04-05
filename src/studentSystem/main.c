#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "student.h"
#include <locale.h>
#define ADRESS_LENTH 100
int main(void)
{
    setlocale(LC_CTYPE, "");
    FILE *fp = NULL;
    char addr_str[ADRESS_LENTH];
    wprintf(L"请输入数据文件所在地址:");
    myscanf_char(addr_str, sizeof(addr_str));
    fp = fopen(addr_str, "r+");
    if (fp)
    {
        wprintf(L"文件打开成功！\n");
        int8_t student_operand;
        while (1)
        {
            wprintf(L"欢迎进入学生数据管理系统,请输入你想进行的操作:\n1.写入信息 2.查找信息 3.退出系统\n");
            myscanf_int(&student_operand, 1);
            if (student_operand == 1)
            {
                if (student_write(&fp) == 0)
                    wprintf(L"文件写入成功!\n");
                else
                    wprintf(L"文件写入失败！\n");
            }
            else if (student_operand == 2)
            {
                if (student_read(&fp) == 0)
                    wprintf(L"文件读取成功!\n");
                else
                    wprintf(L"文件读取失败！\n");
            }
            else if (student_operand == 3)
            {
                wprintf(L"系统退出成功!\n");
                break;
            }
            else
                wprintf(L"输入的数字无效,请重新输入！\n");
        }
        fclose(fp);
    }
    else
        wprintf(L"文件打开错误！\n");
    getchar();
    return 0;
}