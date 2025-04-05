#include "oled_font.h"
#include <stdio.h>

int main(void)
{
    oled_font_init();
    for (uint16_t i = 0, j = 1; i < Oled_CF_Size; i++)
    {
        printf("%x%x%x", (uint8_t)OLED_CF16x16[OLED_CF_Tree[i]].chs_bytes[0], (uint8_t)OLED_CF16x16[OLED_CF_Tree[i]].chs_bytes[1],
               (uint8_t)OLED_CF16x16[OLED_CF_Tree[i]].chs_bytes[2]);
        if (i == j * 2 - 2)
        {
            putchar('\n');
            j *= 2;
            continue;
        }
        putchar(' ');
    }
    // for (uint16_t i = 0, j = 1; i < Oled_CF_Size; i++)
    // {
    //     printf("%d", OLED_CF_Tree[i]);
    //     if (i == j * 2 - 2)
    //     {
    //         putchar('\n');
    //         j *= 2;
    //         continue;
    //     }
    //     putchar(' ');
    // }
    // for (uint16_t i = 0; i < Oled_CF_Size; i++)
    // {
    //     printf("%x%x%x", (uint8_t)OLED_CF16x16[OLED_CF_Tree[i]].chs_bytes[0], (uint8_t)OLED_CF16x16[OLED_CF_Tree[i]].chs_bytes[1],
    //            (uint8_t)OLED_CF16x16[OLED_CF_Tree[i]].chs_bytes[2]);
    //     if (i % 8 == 0)
    //     {
    //         putchar('\n');
    //         continue;
    //     }
    //     putchar(' ');
    // }
    getchar();
    return 0;
}