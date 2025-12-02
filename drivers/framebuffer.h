#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

#define FB_MEMORY_ADDRESS 0x000B8000

typedef enum {
    FB_BLACK = 0,
    FB_BLUE = 1,
    FB_GREEN = 2,
    FB_CYAN = 3,
    FB_RED = 4,
    FB_MAGENTA = 5,
    FB_BROWN = 6,
    FB_LIGHT_GREY = 7,
    FB_DARK_GREY = 8,
    FB_LIGHT_BLUE = 9,
    FB_LIGHT_GREEN = 10,
    FB_LIGHT_CYAN = 11,
    FB_LIGHT_RED = 12,
    FB_LIGHT_MAGENTA = 13,
    FB_YELLOW = 14,
    FB_WHITE = 15
} fb_color;

void fb_clear(void);
void fb_move_cursor(unsigned short pos);
unsigned short fb_get_cursor_position(void);
void fb_move(unsigned short x, unsigned short y);
void fb_write_char(char c, unsigned char fg, unsigned char bg);
void fb_write(char* text);
void fb_write_dec(int num);
void fb_write_hex(unsigned int num);
void fb_set_color(unsigned char fg, unsigned char bg);
void fb_backspace(void);
void fb_newline(void);
#endif
