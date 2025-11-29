#include "../drivers/framebuffer.h"

static inline unsigned char inb(unsigned short port) {
    unsigned char data;
    asm volatile ("inb %1, %0" : "=a" (data) : "dN" (port));
    return data;
}

static inline void outb(unsigned short port, unsigned char data) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

void serial_putc(char c) {
    while ((inb(0x3F8 + 5) & 0x20) == 0);
    outb(0x3F8, c);
}

void serial_puts(const char* str) {
    while (*str) {
        serial_putc(*str);
        str++;
    }
}

void serial_put_dec(int num) {
    if (num < 0) {
        serial_putc('-');
        num = -num;
    }
    
    if (num >= 10) {
        serial_put_dec(num / 10);
    }
    serial_putc('0' + (num % 10));
}

int sum_of_three(int a, int b, int c) {
    return a + b + c;
}

int multiply(int a, int b) {
    return a * b;
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

void kernel_main(void) {
    serial_puts("Kernel started successfully!\r\n");
    serial_puts("Testing framebuffer...\r\n");
    
    int result1 = sum_of_three(1, 2, 3);
    serial_puts("sum_of_three(1,2,3) = ");
    serial_put_dec(result1);
    serial_puts("\r\n");
    
    int result2 = multiply(6, 7);
    serial_puts("multiply(6,7) = ");
    serial_put_dec(result2);
    serial_puts("\r\n");
    
    int result3 = factorial(5);
    serial_puts("factorial(5) = ");
    serial_put_dec(result3);
    serial_puts("\r\n");
    
    serial_puts("Initializing framebuffer...\r\n");
    
    fb_clear();
    serial_puts("Framebuffer cleared\r\n");
    
    fb_set_color(FB_WHITE, FB_BLUE);
    serial_puts("Color set to white on blue\r\n");
    
    fb_write("=== Tiny OS Booted ===\n\n");
    serial_puts("First line written to framebuffer\r\n");
    
    fb_set_color(FB_GREEN, FB_BLACK);
    fb_write("Framebuffer driver working!\n");
    serial_puts("Second line written to framebuffer\r\n");
    
    fb_set_color(FB_RED, FB_BLACK);
    fb_write("Text output successful\n");
    serial_puts("Third line written to framebuffer\r\n");
    
    fb_set_color(FB_CYAN, FB_BLACK);
    fb_write("Cursor control active\n\n");
    serial_puts("Fourth line written to framebuffer\r\n");
    
    fb_move(10, 15);
    serial_puts("Cursor moved to (10,15)\r\n");
    fb_write("Cursor moved to (10,15)");
    
    serial_puts("All framebuffer tests completed\r\n");
    
    while(1) {
    }
}
