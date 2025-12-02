#include "../drivers/framebuffer.h"
#include "../drivers/interrupts.h"
#include "../drivers/hardware_interrupt_enabler.h"
#include "../drivers/io.h"
#include "../drivers/terminal.h"

int sum_of_three(int arg1, int arg2, int arg3) {
    return arg1 + arg2 + arg3;
}

int multiply(int a, int b) {
    return a * b;
}

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int kernel_main() {
    fb_clear();
    fb_set_color(7, 0);

    fb_write("== Operating System ==\n\n");

    fb_write("Basic tests:\n");
    fb_write("sum_of_three(1,2,3) = ");
    fb_write_dec(sum_of_three(1, 2, 3));
    fb_write_char('\n', 7, 0);

    fb_write("multiply(6,7) = ");
    fb_write_dec(multiply(6, 7));
    fb_write_char('\n', 7, 0);

    fb_write("factorial(5) = ");
    fb_write_dec(factorial(5));
    fb_write_char('\n', 7, 0);

    fb_write("\n---\n");

    fb_write("Step 1: Initializing IDT...\n");
    interrupts_install_idt();
    fb_write("Step 2: IDT initialized.\n");

    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
    
    fb_write("Step 3: Keyboard interrupt unmasked\n");
    
    enable_hardware_interrupts();
     fb_write("Step 4: Interrupts enabled\n");

    fb_write("Step 5: Starting terminal system...\n");
    
    terminal_init();
    terminal_run();
    
    while (1) {
    }
}
