#include "io.h"
#include "framebuffer.h"
#include "types.h"

static u16int fb_calculate_position(u16int x, u16int y);
static void fb_write_cell(u16int position, char c, u8int fg, u8int bg);

static u16int* fb = (u16int*) FB_MEMORY_ADDRESS;

static u16int cursor_x = 0;
static u16int cursor_y = 0;

static u8int current_fg = FB_LIGHT_GREY;
static u8int current_bg = FB_BLACK;

static u16int fb_calculate_position(u16int x, u16int y) {
    return y * 80 + x;
}

static void fb_write_cell(u16int position, char c, u8int fg, u8int bg) {
    fb[position] = (c & 0xFF) | ((fg & 0x0F) << 8) | ((bg & 0x0F) << 12);
}

void fb_clear(void) {
    for (u16int i = 0; i < 80 * 25; i++) {
        fb_write_cell(i, ' ', current_fg, current_bg);
    }
    cursor_x = 0;
    cursor_y = 0;
    fb_move_cursor(0);
}

void fb_move_cursor(unsigned short pos) {
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, (pos >> 8) & 0xFF);
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0xFF);
    
    cursor_x = pos % 80;
    cursor_y = pos / 80;
}

unsigned short fb_get_cursor_position(void) {
    unsigned short pos = 0;
    
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    pos = inb(FB_DATA_PORT) << 8;
    
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    pos |= inb(FB_DATA_PORT);
    
    return pos;
}

void fb_move(unsigned short x, unsigned short y) {
    cursor_x = x;
    cursor_y = y;
    fb_move_cursor(fb_calculate_position(x, y));
}

void fb_write_char(char c, unsigned char fg, unsigned char bg) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 8) & ~(8 - 1);
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
        }
    } else {
        u16int position = fb_calculate_position(cursor_x, cursor_y);
        fb_write_cell(position, c, fg, bg);
        cursor_x++;
    }
    
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= 25) {
        fb_clear();
        cursor_x = 0;
        cursor_y = 0;
    }
    
    fb_move_cursor(fb_calculate_position(cursor_x, cursor_y));
}

void fb_write(char* text) {
    for (u32int i = 0; text[i] != '\0'; i++) {
        fb_write_char(text[i], current_fg, current_bg);
    }
}

void fb_write_dec(int num) {
    if (num == 0) {
        fb_write_char('0', current_fg, current_bg);
        return;
    }
    
    if (num < 0) {
        fb_write_char('-', current_fg, current_bg);
        num = -num;
    }
    
    char buffer[32];
    int i = 0;
    
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    for (int j = i - 1; j >= 0; j--) {
        fb_write_char(buffer[j], current_fg, current_bg);
    }
}

void fb_write_hex(unsigned int num) {
    fb_write("0x");
    
    if (num == 0) {
        fb_write_char('0', current_fg, current_bg);
        return;
    }
    
    char buffer[32];
    int i = 0;
    
    while (num > 0) {
        int digit = num % 16;
        if (digit < 10) {
            buffer[i++] = '0' + digit;
        } else {
            buffer[i++] = 'A' + (digit - 10);
        }
        num /= 16;
    }
    
    for (int j = i - 1; j >= 0; j--) {
        fb_write_char(buffer[j], current_fg, current_bg);
    }
}

void fb_set_color(unsigned char fg, unsigned char bg) {
    current_fg = fg;
    current_bg = bg;
}

void fb_backspace(void) {
    unsigned short pos = fb_get_cursor_position();
    
    if (pos > 0) {
        pos--;
        fb_move_cursor(pos);
        
        fb_write_char(' ', current_fg, current_bg);
        
        fb_move_cursor(pos);
        
        cursor_x = pos % 80;
        cursor_y = pos / 80;
    }
}

void fb_newline(void) {
    fb_write_char('\n', current_fg, current_bg);
}

