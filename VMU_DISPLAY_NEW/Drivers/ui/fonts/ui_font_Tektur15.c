/*******************************************************************************
 * Size: 15 px
 * Bpp: 1
 * Opts: --bpp 1 --size 15 --font F:/black_sheep/LCD_BMU/UI/assets/Tektur-Regular.ttf -o F:/black_sheep/LCD_BMU/UI/assets\ui_font_Tektur15.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_TEKTUR15
#define UI_FONT_TEKTUR15 1
#endif

#if UI_FONT_TEKTUR15

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
    0x99, 0x99,

    /* U+0023 "#" */
    0x21, 0x8, 0x4f, 0xfc, 0x84, 0x21, 0x8, 0x42,
    0x13, 0xff, 0x21, 0x8, 0x42, 0x10,

    /* U+0024 "$" */
    0x8, 0x8, 0x7f, 0x80, 0x80, 0x80, 0x80, 0xff,
    0x1, 0x1, 0x1, 0xfe, 0x10, 0x10,

    /* U+0025 "%" */
    0x81, 0x82, 0x6, 0x4, 0x8, 0x18, 0x10, 0x20,
    0x60, 0x41, 0x81,

    /* U+0026 "&" */
    0x3f, 0x18, 0x3, 0x0, 0x40, 0xf8, 0xe3, 0x28,
    0x72, 0xc, 0x83, 0x21, 0x6f, 0xcc,

    /* U+0027 "'" */
    0xf0,

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
    0x4, 0x30, 0x86, 0x10, 0x43, 0x8, 0x61, 0xc,
    0x0,

    /* U+0030 "0" */
    0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0031 "1" */
    0x67, 0x8, 0x42, 0x10, 0x84, 0x21, 0x3e,

    /* U+0032 "2" */
    0xff, 0x1, 0x1, 0x1, 0x1, 0xff, 0xc0, 0x80,
    0x80, 0x80, 0xff,

    /* U+0033 "3" */
    0xff, 0x1, 0x1, 0x1, 0x1, 0x7e, 0x1, 0x1,
    0x1, 0x1, 0xff,

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
    0xff, 0x81, 0x81, 0x81, 0x81, 0x7e, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0039 "9" */
    0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x1,
    0x1, 0x1, 0xff,

    /* U+003A ":" */
    0x82,

    /* U+003B ";" */
    0x40, 0x5, 0x80,

    /* U+003C "<" */
    0x1, 0x7, 0x1c, 0x70, 0x80, 0x70, 0x1c, 0x7,
    0x1,

    /* U+003D "=" */
    0xff, 0x0, 0x0, 0x0, 0xff,

    /* U+003E ">" */
    0x80, 0xe0, 0x38, 0xe, 0x1, 0xe, 0x38, 0xe0,
    0x80,

    /* U+003F "?" */
    0xfe, 0x1, 0x1, 0x1, 0x3, 0x1e, 0x30, 0x20,
    0x0, 0x0, 0x20,

    /* U+0040 "@" */
    0xff, 0xa0, 0x18, 0x6, 0x1, 0x8f, 0xe2, 0x18,
    0x86, 0x21, 0x87, 0xa0, 0x8, 0x2, 0x0, 0xff,
    0xc0,

    /* U+0041 "A" */
    0x1e, 0x9, 0xc, 0x86, 0x62, 0x31, 0x9, 0x84,
    0xff, 0x41, 0xa0, 0x70, 0x20,

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
    0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x81, 0x81,
    0x81, 0x81, 0x81,

    /* U+0049 "I" */
    0xf9, 0x8, 0x42, 0x10, 0x84, 0x21, 0x3e,

    /* U+004A "J" */
    0xfe, 0x4, 0x8, 0x10, 0x20, 0x60, 0xc1, 0x83,
    0x5, 0xf0,

    /* U+004B "K" */
    0x83, 0x86, 0x84, 0x8c, 0x88, 0xf8, 0x88, 0x84,
    0x86, 0x82, 0x83,

    /* U+004C "L" */
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81,
    0x81, 0x81, 0xff,

    /* U+004D "M" */
    0xc0, 0xf0, 0x3e, 0x1e, 0xcd, 0x92, 0x67, 0x98,
    0xc6, 0x31, 0x80, 0x60, 0x18, 0x4,

    /* U+004E "N" */
    0xc1, 0xc1, 0xe1, 0xb1, 0x91, 0x99, 0x89, 0x8d,
    0x87, 0x83, 0x83,

    /* U+004F "O" */
    0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0050 "P" */
    0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff,
    0x80, 0x80, 0x80,

    /* U+0051 "Q" */
    0xff, 0x40, 0xa0, 0x50, 0x28, 0x14, 0xa, 0x5,
    0x2, 0x81, 0x40, 0x9f, 0x41, 0xf0,

    /* U+0052 "R" */
    0xff, 0x40, 0xa0, 0x50, 0x28, 0x14, 0xb, 0xfd,
    0x6, 0x81, 0x40, 0xa0, 0x60,

    /* U+0053 "S" */
    0xff, 0x2, 0x4, 0x8, 0x1f, 0xc0, 0x81, 0x2,
    0x7, 0xf8,

    /* U+0054 "T" */
    0xff, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10,

    /* U+0055 "U" */
    0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0xff,

    /* U+0056 "V" */
    0xc0, 0xa0, 0x50, 0x6c, 0x36, 0x11, 0x8, 0x84,
    0x46, 0x32, 0x9, 0x7, 0x80,

    /* U+0057 "W" */
    0xc0, 0x1a, 0x0, 0x91, 0xc4, 0x8e, 0x24, 0x51,
    0x22, 0x99, 0x94, 0xcd, 0xa4, 0x2d, 0xa1, 0x45,
    0xe, 0x38,

    /* U+0058 "X" */
    0x40, 0x98, 0x62, 0x10, 0xcc, 0x12, 0x7, 0x83,
    0x20, 0x84, 0x61, 0x10, 0x64, 0x8,

    /* U+0059 "Y" */
    0xc1, 0xa0, 0x90, 0xcc, 0x62, 0x21, 0x10, 0x78,
    0x10, 0x8, 0x4, 0x2, 0x0,

    /* U+005A "Z" */
    0xfe, 0xc, 0x10, 0x41, 0x82, 0xc, 0x10, 0x41,
    0x83, 0xf8,

    /* U+005B "[" */
    0xf8, 0x88, 0x88, 0x88, 0x88, 0x88, 0x8f,

    /* U+005C "\\" */
    0xc1, 0x6, 0x8, 0x30, 0x41, 0x6, 0x8, 0x30,
    0x40,

    /* U+005D "]" */
    0xf1, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1f,

    /* U+005E "^" */
    0x10, 0x30, 0xa3, 0x24, 0x78, 0x40,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0xf8,

    /* U+0061 "a" */
    0xfc, 0x4, 0x8, 0x1f, 0xf0, 0x60, 0xc1, 0x7e,

    /* U+0062 "b" */
    0x81, 0x3, 0xf4, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x7, 0xf0,

    /* U+0063 "c" */
    0x7f, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x7e,

    /* U+0064 "d" */
    0x2, 0x5, 0xfc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x5, 0xf8,

    /* U+0065 "e" */
    0x7f, 0x6, 0xc, 0x1f, 0xf0, 0x20, 0x40, 0x7e,

    /* U+0066 "f" */
    0x7c, 0x21, 0xf, 0xc2, 0x10, 0x84, 0x20,

    /* U+0067 "g" */
    0x7f, 0x6, 0xc, 0x18, 0x30, 0x5f, 0x81, 0x2,
    0x7, 0xf0,

    /* U+0068 "h" */
    0x81, 0x3, 0xf4, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0x8,

    /* U+0069 "i" */
    0xbf, 0xe0,

    /* U+006A "j" */
    0xbf, 0xf8,

    /* U+006B "k" */
    0x80, 0x80, 0x82, 0x86, 0x84, 0x88, 0xf8, 0x8c,
    0x84, 0x86, 0x82,

    /* U+006C "l" */
    0xff, 0xe0,

    /* U+006D "m" */
    0xff, 0xd0, 0x86, 0x10, 0xc2, 0x18, 0x43, 0x8,
    0x61, 0xc, 0x21, 0x84, 0x20,

    /* U+006E "n" */
    0xfd, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x82,

    /* U+006F "o" */
    0x7d, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x7c,

    /* U+0070 "p" */
    0xfd, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0xfd,
    0x2, 0x0,

    /* U+0071 "q" */
    0x7f, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x7e,
    0x4, 0x8,

    /* U+0072 "r" */
    0x7e, 0x8, 0x20, 0x82, 0x8, 0x20, 0x80,

    /* U+0073 "s" */
    0x7f, 0x2, 0x4, 0xf, 0xe0, 0x40, 0x81, 0xfc,

    /* U+0074 "t" */
    0x2, 0x8, 0x3f, 0x82, 0x8, 0x20, 0x82, 0x8,
    0x1f,

    /* U+0075 "u" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x7e,

    /* U+0076 "v" */
    0xc1, 0x20, 0x90, 0x48, 0x66, 0x21, 0x10, 0x88,
    0x4c, 0x3c, 0x0,

    /* U+0077 "w" */
    0x80, 0x31, 0x8e, 0x39, 0xc7, 0x2c, 0xa5, 0xb4,
    0x94, 0x92, 0x9a, 0x71, 0xc0,

    /* U+0078 "x" */
    0x41, 0xb0, 0x88, 0xc6, 0x41, 0xe1, 0x90, 0x8c,
    0xc2, 0x41, 0x80,

    /* U+0079 "y" */
    0x81, 0x83, 0xc2, 0x42, 0x46, 0x66, 0x3c, 0xc,
    0xc, 0x8, 0xf8,

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
    0x30, 0x3e, 0xe1, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 62, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 65, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 96, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 5, .adv_w = 197, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 19, .adv_w = 158, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 33, .adv_w = 144, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 176, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 53, .box_w = 1, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 59, .adv_w = 82, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 66, .adv_w = 82, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 73, .adv_w = 125, .box_w = 7, .box_h = 7, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 80, .adv_w = 144, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 60, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 89, .adv_w = 82, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 90, .adv_w = 60, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 106, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 156, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 111, .adv_w = 103, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 118, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 146, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 156, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 173, .adv_w = 149, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 184, .adv_w = 161, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 156, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 206, .adv_w = 60, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 207, .adv_w = 60, .box_w = 2, .box_h = 9, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 210, .adv_w = 144, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 219, .adv_w = 144, .box_w = 8, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 224, .adv_w = 144, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 132, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 244, .adv_w = 192, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 261, .adv_w = 156, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 274, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 285, .adv_w = 132, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 295, .adv_w = 152, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 306, .adv_w = 139, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 137, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 156, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 337, .adv_w = 161, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 348, .adv_w = 103, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 144, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 365, .adv_w = 151, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 376, .adv_w = 144, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 387, .adv_w = 187, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 401, .adv_w = 163, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 412, .adv_w = 163, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 423, .adv_w = 149, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 163, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 448, .adv_w = 156, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 461, .adv_w = 154, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 471, .adv_w = 134, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 482, .adv_w = 156, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 493, .adv_w = 156, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 506, .adv_w = 206, .box_w = 13, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 524, .adv_w = 158, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 538, .adv_w = 154, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 551, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 561, .adv_w = 82, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 568, .adv_w = 106, .box_w = 6, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 577, .adv_w = 82, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 584, .adv_w = 120, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 590, .adv_w = 144, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 591, .adv_w = 590, .box_w = 5, .box_h = 1, .ofs_x = 33, .ofs_y = 10},
    {.bitmap_index = 592, .adv_w = 144, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 600, .adv_w = 149, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 610, .adv_w = 131, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 618, .adv_w = 149, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 628, .adv_w = 142, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 636, .adv_w = 98, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 643, .adv_w = 146, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 653, .adv_w = 149, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 663, .adv_w = 50, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 665, .adv_w = 50, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 667, .adv_w = 144, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 678, .adv_w = 50, .box_w = 1, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 680, .adv_w = 221, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 693, .adv_w = 149, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 701, .adv_w = 144, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 709, .adv_w = 149, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 719, .adv_w = 149, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 729, .adv_w = 113, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 736, .adv_w = 144, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 744, .adv_w = 103, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 753, .adv_w = 149, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 761, .adv_w = 139, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 772, .adv_w = 202, .box_w = 11, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 785, .adv_w = 151, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 796, .adv_w = 144, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 807, .adv_w = 127, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 815, .adv_w = 96, .box_w = 6, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 826, .adv_w = 50, .box_w = 1, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 828, .adv_w = 96, .box_w = 6, .box_h = 14, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 839, .adv_w = 144, .box_w = 9, .box_h = 3, .ofs_x = 0, .ofs_y = 4}
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
    -7, -7, -12, -12, -12, -7
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
const lv_font_t ui_font_Tektur15 = {
#else
lv_font_t ui_font_Tektur15 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_TEKTUR15*/

