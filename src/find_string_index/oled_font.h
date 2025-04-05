#ifndef __OLED__FONT__H
#define __OLED__FONT__H
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define Encode_Type_UTF8 // 定义编码类型!!!(Encode_Type_UTF8或者Encode_Type_GBK)
#define CHS_BYTES 3      // 中文字宽(bytes),UTF-8编码格式给3，GB2312编码格式给2

    /*字模基本单元*/
    typedef struct
    {
        uint8_t Data[32];              // 字模数据
        char chs_bytes[CHS_BYTES + 1]; // 汉字索引
    } ChineseCell_t;

    /*ASCII字模数据声明*/
    extern const uint8_t OLED_F8x16[][16];
    extern const uint8_t OLED_F6x8[][6];

    /*汉字字模数据声明*/
    extern const ChineseCell_t OLED_CF16x16[];
    extern const uint16_t Oled_CF_Size;
    extern uint16_t OLED_CF_Tree[34];

    /*图像数据声明*/
    extern const uint8_t Diode[];
    extern const unsigned char gImage_Icon32x32[][128];
    extern const unsigned char gImage_Icon16x16[][32];

    void oled_font_init(void);
    int32_t oled_find_chs_index(uint8_t *string);

#ifdef __cplusplus
}
#endif
#endif
