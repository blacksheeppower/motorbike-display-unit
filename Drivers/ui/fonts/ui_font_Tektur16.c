/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font F:/black_sheep/LCD_BMU/UI/assets/Tektur-Regular.ttf -o F:/black_sheep/LCD_BMU/UI/assets\ui_font_Tektur16.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_TEKTUR16
#define UI_FONT_TEKTUR16 1
#endif

#if UI_FONT_TEKTUR16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfe, 0x20,

    /* U+0022 "\"" */
    0xd9, 0x99,

    /* U+0023 "#" */
    0x21, 0x4, 0x23, 0xff, 0x90, 0x82, 0x10, 0x42,
    0x8, 0x47, 0xff, 0x21, 0x4, 0x20, 0x84, 0x0,

    /* U+0024 "$" */
    0x8, 0x8, 0x7f, 0x80, 0x80, 0x80, 0x80, 0xff,
    0x1, 0x1, 0x1, 0xfe, 0x8, 0x8,

    /* U+0025 "%" */
    0x81, 0x82, 0x6, 0x4, 0x8, 0x18, 0x10, 0x20,
    0x60, 0x41, 0x81,

    /* U+0026 "&" */
    0x3f, 0x8c, 0x0, 0xc0, 0xc, 0xf, 0xc5, 0x9,
    0xa1, 0xe4, 0x18, 0x83, 0x90, 0xd3, 0xf1, 0x0,

    /* U+0027 "'" */
    0xea,

    /* U+0028 "(" */
    0xf8, 0x88, 0x88, 0x88, 0x88, 0x88, 0x8f,

    /* U+0029 ")" */
    0xf1, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1f,

    /* U+002A "*" */
    0x11, 0x25, 0xf1, 0xc7, 0xd2, 0x44, 0x0,

    /* U+002B "+" */
    0x10, 0x20, 0x40, 0x8f, 0xe2, 0x4, 0x8, 0x10,

    /* U+002C "," */
    0x58,

    /* U+002D "-" */
    0xf0,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x6, 0x8, 0x30, 0x41, 0x82, 0xc, 0x10, 0x60,
    0x83, 0x0,

    /* U+0030 "0" */
    0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0031 "1" */
    0x67, 0x8, 0x42, 0x10, 0x84, 0x21, 0x3e,

    /* U+0032 "2" */
    0xff, 0x1, 0x1, 0x1, 0x1, 0xff, 0xc0, 0x80,
    0x80, 0x80, 0xff,

    /* U+0033 "3" */
    0xff, 0x0, 0x80, 0x40, 0x20, 0x13, 0xf8, 0x2,
    0x1, 0x0, 0x80, 0x7f, 0xe0,

    /* U+0034 "4" */
    0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
    0x1, 0x1, 0x1,

    /* U+0035 "5" */
    0xff, 0x80, 0x80, 0x80, 0x80, 0xff, 0x1, 0x1,
    0x1, 0x1, 0xff,

    /* U+0036 "6" */
    0xff, 0x80, 0x80, 0x80, 0xff, 0x81, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0037 "7" */
    0xff, 0x81, 0x81, 0x81, 0x3, 0x6, 0x4, 0xc,
    0x18, 0x30, 0x20,

    /* U+0038 "8" */
    0x7f, 0xa0, 0x50, 0x28, 0x14, 0xb, 0xfa, 0x3,
    0x1, 0x80, 0xc0, 0x7f, 0xe0,

    /* U+0039 "9" */
    0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x1,
    0x1, 0x1, 0xff,

    /* U+003A ":" */
    0x82,

    /* U+003B ";" */
    0x40, 0x5, 0x80,

    /* U+003C "<" */
    0x1, 0x83, 0x87, 0xe, 0xc, 0x3, 0x80, 0x70,
    0xe, 0x1, 0x80,

    /* U+003D "=" */
    0xff, 0x0, 0x0, 0x0, 0xff,

    /* U+003E ">" */
    0x80, 0x70, 0xf, 0x0, 0xe0, 0x18, 0x38, 0xf1,
    0xc0, 0x80, 0x0,

    /* U+003F "?" */
    0xfe, 0x1, 0x1, 0x1, 0x3, 0x1f, 0x30, 0x20,
    0x0, 0x0, 0x20,

    /* U+0040 "@" */
    0xff, 0xd0, 0x6, 0x0, 0xc0, 0x18, 0xff, 0x10,
    0x62, 0xc, 0x41, 0x87, 0xd0, 0x2, 0x0, 0x40,
    0xf, 0xfe,

    /* U+0041 "A" */
    0x1e, 0x4, 0xc1, 0x30, 0xc4, 0x21, 0x8, 0x62,
    0x19, 0xfe, 0x40, 0x90, 0x3c, 0xc,

    /* U+0042 "B" */
    0xff, 0x81, 0x81, 0x81, 0x81, 0xfe, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0043 "C" */
    0xff, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x81,
    0x3, 0xf8,

    /* U+0044 "D" */
    0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0045 "E" */
    0xff, 0x2, 0x4, 0x8, 0x1f, 0xa0, 0x40, 0x81,
    0x3, 0xf8,

    /* U+0046 "F" */
    0xff, 0x2, 0x4, 0x8, 0x10, 0x3f, 0xc0, 0x81,
    0x2, 0x0,

    /* U+0047 "G" */
    0xff, 0x80, 0x80, 0x80, 0x80, 0x9f, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0048 "H" */
    0x80, 0xc0, 0x60, 0x30, 0x18, 0xf, 0xfe, 0x3,
    0x1, 0x80, 0xc0, 0x60, 0x20,

    /* U+0049 "I" */
    0xf9, 0x8, 0x42, 0x10, 0x84, 0x21, 0x3e,

    /* U+004A "J" */
    0xff, 0x1, 0x1, 0x1, 0x1, 0x1, 0x81, 0x81,
    0x81, 0x81, 0x7e,

    /* U+004B "K" */
    0x83, 0x41, 0x21, 0x90, 0x88, 0xc7, 0xc2, 0x31,
    0x8, 0x86, 0x41, 0xa0, 0x40,

    /* U+004C "L" */
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81,
    0x81, 0x81, 0xff,

    /* U+004D "M" */
    0xc0, 0xf0, 0x3e, 0x1e, 0xcd, 0x92, 0x67, 0x98,
    0xc6, 0x31, 0x80, 0x60, 0x18, 0x4,

    /* U+004E "N" */
    0xc0, 0xf0, 0x68, 0x36, 0x19, 0x8c, 0x46, 0x33,
    0xd, 0x82, 0xc1, 0xe0, 0x60,

    /* U+004F "O" */
    0xff, 0xc0, 0x60, 0x30, 0x18, 0xc, 0x6, 0x3,
    0x1, 0x80, 0xc0, 0x7f, 0xe0,

    /* U+0050 "P" */
    0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
    0x80, 0x80, 0x80,

    /* U+0051 "Q" */
    0xff, 0xa0, 0x28, 0xa, 0x2, 0x80, 0xa0, 0x28,
    0xa, 0x2, 0x80, 0xa0, 0x27, 0xc8, 0x3f,

    /* U+0052 "R" */
    0xff, 0x40, 0xa0, 0x50, 0x28, 0x14, 0xb, 0xfd,
    0x6, 0x81, 0x40, 0xa0, 0x60,

    /* U+0053 "S" */
    0xff, 0x80, 0x80, 0x80, 0x80, 0xff, 0x1, 0x1,
    0x1, 0x1, 0xff,

    /* U+0054 "T" */
    0xff, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,
    0x8, 0x8, 0x8,

    /* U+0055 "U" */
    0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0056 "V" */
    0xc0, 0xd0, 0x34, 0x9, 0x82, 0x61, 0x88, 0x62,
    0x10, 0xc4, 0x13, 0x4, 0xc1, 0xe0,

    /* U+0057 "W" */
    0xc0, 0xa, 0x0, 0x50, 0xc2, 0x86, 0x34, 0x71,
    0xb2, 0xcd, 0x96, 0x44, 0x92, 0x24, 0x91, 0x64,
    0x8f, 0x3c,

    /* U+0058 "X" */
    0x40, 0xd8, 0x22, 0x18, 0xc4, 0x13, 0x7, 0x81,
    0x30, 0xc4, 0x21, 0x98, 0x24, 0xc,

    /* U+0059 "Y" */
    0xc1, 0xa0, 0x90, 0x4c, 0x62, 0x21, 0x10, 0xf8,
    0x10, 0x8, 0x4, 0x2, 0x0,

    /* U+005A "Z" */
    0xfe, 0xc, 0x10, 0x41, 0x82, 0xc, 0x10, 0x41,
    0x83, 0xf8,

    /* U+005B "[" */
    0xf8, 0x88, 0x88, 0x88, 0x88, 0x88, 0x8f,

    /* U+005C "\\" */
    0xc0, 0x81, 0x81, 0x3, 0x2, 0x6, 0x4, 0xc,
    0x8, 0x18,

    /* U+005D "]" */
    0xf1, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1f,

    /* U+005E "^" */
    0x18, 0x18, 0x3c, 0x24, 0x66, 0xc3,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0xf8,

    /* U+0061 "a" */
    0xfe, 0x1, 0x1, 0x1, 0xff, 0x81, 0x81, 0x81,
    0x7f,

    /* U+0062 "b" */
    0x80, 0x80, 0xfe, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0xfe,

    /* U+0063 "c" */
    0x7f, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0x7f,

    /* U+0064 "d" */
    0x1, 0x1, 0x7f, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x7f,

    /* U+0065 "e" */
    0x7f, 0x81, 0x81, 0x81, 0xff, 0x80, 0x80, 0x80,
    0x7f,

    /* U+0066 "f" */
    0x7e, 0x8, 0x20, 0xfe, 0x8, 0x20, 0x82, 0x8,
    0x0,

    /* U+0067 "g" */
    0x7f, 0x81, 0x81, 0x81, 0x81, 0x81, 0x7f, 0x1,
    0x1, 0x1, 0xfe,

    /* U+0068 "h" */
    0x80, 0x80, 0xfe, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x81,

    /* U+0069 "i" */
    0xbf, 0xe0,

    /* U+006A "j" */
    0xbf, 0xf8,

    /* U+006B "k" */
    0x80, 0x80, 0x83, 0x86, 0x84, 0x8c, 0xf8, 0x8c,
    0x84, 0x86, 0x83,

    /* U+006C "l" */
    0xff, 0xe0,

    /* U+006D "m" */
    0xff, 0xf4, 0x10, 0x60, 0x83, 0x4, 0x18, 0x20,
    0xc1, 0x6, 0x8, 0x30, 0x41, 0x82, 0x8,

    /* U+006E "n" */
    0xfe, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81,

    /* U+006F "o" */
    0x7e, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x7e,

    /* U+0070 "p" */
    0xfe, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0xfe, 0x80, 0x80,

    /* U+0071 "q" */
    0x7f, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x7f, 0x1, 0x1,

    /* U+0072 "r" */
    0x7f, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80,

    /* U+0073 "s" */
    0x7f, 0x80, 0x80, 0x80, 0xff, 0x1, 0x1, 0x1,
    0xfe,

    /* U+0074 "t" */
    0x2, 0x8, 0x3f, 0x82, 0x8, 0x20, 0x82, 0x8,
    0x1f,

    /* U+0075 "u" */
    0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x7f,

    /* U+0076 "v" */
    0xc0, 0xa0, 0xd0, 0x4c, 0x26, 0x31, 0x18, 0x88,
    0x64, 0x1e, 0x0,

    /* U+0077 "w" */
    0x80, 0x38, 0x63, 0x8e, 0x2c, 0xe2, 0xcb, 0x24,
    0xb2, 0x59, 0x65, 0x96, 0x71, 0xc0,

    /* U+0078 "x" */
    0x60, 0x98, 0x62, 0x10, 0xcc, 0x1e, 0xc, 0xc2,
    0x11, 0x86, 0x40, 0x80,

    /* U+0079 "y" */
    0x81, 0xe0, 0xb0, 0xc8, 0x66, 0x21, 0x30, 0xf8,
    0x8, 0xc, 0x6, 0x3e, 0x0,

    /* U+007A "z" */
    0xfe, 0x8, 0x30, 0xc1, 0x6, 0x18, 0x20, 0xfe,

    /* U+007B "{" */
    0x3c, 0x82, 0x8, 0x20, 0x8c, 0x8, 0x20, 0x82,
    0x8, 0x20, 0xf0,

    /* U+007C "|" */
    0xff, 0xfe,

    /* U+007D "}" */
    0xf0, 0x41, 0x4, 0x10, 0x40, 0xc4, 0x10, 0x41,
    0x4, 0x13, 0xc0,

    /* U+007E "~" */
    0x30, 0x2e, 0x41, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 67, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 69, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 102, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 5, .adv_w = 210, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 21, .adv_w = 169, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 35, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 46, .adv_w = 188, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 56, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 63, .adv_w = 87, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 70, .adv_w = 87, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 77, .adv_w = 133, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 84, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 92, .adv_w = 64, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 93, .adv_w = 87, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 94, .adv_w = 64, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 113, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 166, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 110, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 123, .adv_w = 169, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 156, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 147, .adv_w = 164, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 158, .adv_w = 169, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 166, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 180, .adv_w = 159, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 172, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 166, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 64, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 216, .adv_w = 64, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 219, .adv_w = 154, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 154, .box_w = 8, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 235, .adv_w = 154, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 246, .adv_w = 141, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 205, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 275, .adv_w = 166, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 164, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 300, .adv_w = 141, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 310, .adv_w = 163, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 321, .adv_w = 148, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 146, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 166, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 352, .adv_w = 172, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 365, .adv_w = 110, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 372, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 383, .adv_w = 161, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 396, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 200, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 421, .adv_w = 174, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 174, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 447, .adv_w = 159, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 458, .adv_w = 174, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 473, .adv_w = 166, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 486, .adv_w = 164, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 497, .adv_w = 143, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 508, .adv_w = 166, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 519, .adv_w = 166, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 533, .adv_w = 220, .box_w = 13, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 551, .adv_w = 169, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 565, .adv_w = 164, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 578, .adv_w = 143, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 588, .adv_w = 87, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 595, .adv_w = 113, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 605, .adv_w = 87, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 612, .adv_w = 128, .box_w = 8, .box_h = 6, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 618, .adv_w = 154, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 619, .adv_w = 630, .box_w = 5, .box_h = 1, .ofs_x = 35, .ofs_y = 10},
    {.bitmap_index = 620, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 629, .adv_w = 159, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 640, .adv_w = 140, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 649, .adv_w = 159, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 660, .adv_w = 151, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 669, .adv_w = 105, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 678, .adv_w = 156, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 689, .adv_w = 159, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 700, .adv_w = 54, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 702, .adv_w = 54, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 704, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 715, .adv_w = 54, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 717, .adv_w = 236, .box_w = 13, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 732, .adv_w = 159, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 741, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 750, .adv_w = 159, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 761, .adv_w = 159, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 772, .adv_w = 120, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 780, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 789, .adv_w = 110, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 798, .adv_w = 159, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 807, .adv_w = 148, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 818, .adv_w = 215, .box_w = 12, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 832, .adv_w = 161, .box_w = 10, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 844, .adv_w = 154, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 857, .adv_w = 136, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 865, .adv_w = 102, .box_w = 6, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 876, .adv_w = 54, .box_w = 1, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 878, .adv_w = 102, .box_w = 6, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 889, .adv_w = 154, .box_w = 9, .box_h = 3, .ofs_x = 0, .ofs_y = 4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    13, 21,
    15, 21,
    24, 13,
    24, 15,
    24, 64,
    64, 21
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -8, -8, -13, -13, -13, -8
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 6,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Tektur16 = {
#else
lv_font_t ui_font_Tektur16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_TEKTUR16*/

