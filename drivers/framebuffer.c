#include "framebuffer.h"

static unsigned short cursor_pos = 0;
static unsigned char current_fg = FB_WHITE;
static unsigned char current_bg = FB_BLACK;

static inline void outb(unsigned short port, unsigned char data) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char data;
    asm volatile ("inb %1, %0" : "=a" (data) : "dN" (port));
    return data;
}

void fb_move_cursor(unsigned short pos) {
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
    cursor_pos = pos;
}

void fb_clear(void) {
    unsigned short* fb = (unsigned short*)FB_MEMORY_ADDRESS;
    unsigned short blank = (current_bg << 12) | (current_fg << 8) | ' ';
    
    for (int i = 0; i < 80 * 25; i++) {
        fb[i] = blank;
    }
    cursor_pos = 0;
    fb_move_cursor(0);
}

void fb_write_char(char c, unsigned char fg, unsigned char bg) {
    unsigned short* fb = (unsigned short*)FB_MEMORY_ADDRESS;
    
    if (c == '\n') {
        cursor_pos = (cursor_pos / 80 + 1) * 80;
    } else if (c == '\t') {
        for (int i = 0; i < 4; i++) {
            fb_write_char(' ', fg, bg);
        }
        return;
    } else {
        fb[cursor_pos] = (bg << 12) | (fg << 8) | c;
        cursor_pos++;
    }
    
    if (cursor_pos >= 80 * 25) {
        for (int i = 0; i < 80 * 24; i++) {
            fb[i] = fb[i + 80];
        }
        for (int i = 80 * 24; i < 80 * 25; i++) {
            fb[i] = (bg << 12) | (fg << 8) | ' ';
        }
        cursor_pos = 80 * 24;
    }
    
    fb_move_cursor(cursor_pos);
}

void fb_move(unsigned short x, unsigned short y) {
    if (x >= 80) x = 79;
    if (y >= 25) y = 24;
    cursor_pos = y * 80 + x;
    fb_move_cursor(cursor_pos);
}

void fb_write(char* text) {
    while (*text) {
        fb_write_char(*text, current_fg, current_bg);
        text++;
    }
}

void fb_set_color(unsigned char fg, unsigned char bg) {
    current_fg = fg;
    current_bg = bg;
}

void fb_write_dec(int num) {
    if (num < 0) {
        fb_write_char('-', current_fg, current_bg);
        num = -num;
    }
    
    char buffer[32];
    int i = 0;
    
    if (num == 0) {
        fb_write_char('0', current_fg, current_bg);
        return;
    }
    
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
        buffer[i++] = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);
        num /= 16;
    }
    
    for (int j = i - 1; j >= 0; j--) {
        fb_write_char(buffer[j], current_fg, current_bg);
    }
}
