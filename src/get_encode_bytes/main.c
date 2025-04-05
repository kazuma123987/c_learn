#include <stdint.h>
#include <stdio.h>
#include <string.h>

static inline uint8_t get_utf8_bytes(char *utf8_char)
{
    uint8_t *tmp = (uint8_t *)utf8_char;
    // 只要是10xxxxxx这种二进制数据位,则一直自增
    while (tmp[0] >> 6 == 2)
    {
        tmp++;
    }
    // 单字节(ascii码)
    if (tmp[0] >> 7 == 0)
        return 1;
    // 11111110
    uint8_t mask = 0xfe;
    // 二、三、四字节(7-ret)
    for (uint8_t ret = 5; ret >= 3; ret--)
    {
        if (tmp[0] >> ret == ((mask << ret) & 0xff) >> ret)
            return (7 - ret);
    }
    return 0;
}

static inline uint8_t get_gb2312_bytes(char *gb2312_char)
{
    uint8_t *tmp = (uint8_t *)gb2312_char;
    if (gb2312_char[0] >> 7 == 0)
        return 1;
    // GB2312的第一个字节和第二个字节都是0xA0-0xFE,分别表示0到94个数
    if (tmp[0] >= 0xA0 && tmp[0] <= 0xFE && tmp[1] >= 0xA0 && tmp[2] <= 0xFE)
        return 2;
    return 0;
}

int main(int argc, char *argv[])
{
    char tmp_string[6] = {0};
    char *string = "你好001我是到安徽到达好神奇大侠3.~";
    while (string[0] != '\0')
    {
        uint8_t size = get_gb2312_bytes(string);
        memcpy(tmp_string, string, size);
        tmp_string[size] = '\0';
        string += size;
        printf("%s", tmp_string);
        // printf("%u ", size);
    }
    return 0;
}