#include "interrupts.h"
#include "pic.h"
#include "io.h"
#include "framebuffer.h"
#include "keyboard.h"

#define INTERRUPTS_DESCRIPTOR_COUNT 256
#define INTERRUPTS_KEYBOARD 33
#define INPUT_BUFFER_SIZE 256

u8int input_buffer[INPUT_BUFFER_SIZE];
u8int buffer_index = 0;

struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
struct IDT idt;

void interrupts_init_descriptor(s32int index, u32int address) {
    idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF;
    idt_descriptors[index].offset_low = (address & 0xFFFF);
    idt_descriptors[index].segment_selector = 0x08;
    idt_descriptors[index].reserved = 0x00;
    idt_descriptors[index].type_and_attr = (0x01 << 7) | (0x00 << 6) | (0x00 << 5) | 0xe;
}

void interrupts_install_idt() {
    interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (u32int)interrupt_handler_33);
    idt.address = (s32int) &idt_descriptors;
    idt.size = sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT;
    load_idt((s32int) &idt);
    
    pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
    
    outb(0x21, inb(0x21) & ~(1 << 1));
}

void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, u32int interrupt, __attribute__((unused)) struct stack_state stack) {
    u8int input;
    u8int ascii;

    switch (interrupt) {
        case INTERRUPTS_KEYBOARD:
            while ((inb(0x64) & 1)) {
                input = keyboard_read_scan_code();

                if (!(input & 0x80)) {
                    ascii = keyboard_scan_code_to_ascii(input);

                    if (ascii == '\b') {
                        if (buffer_index > 0) {
                            buffer_index--;
                            fb_backspace();
                        }
                    } else if (ascii == '\n') {
                        if (buffer_index < INPUT_BUFFER_SIZE - 1) {
                            input_buffer[buffer_index++] = ascii;
                            fb_newline();
                            fb_write("> ");
                        }
                    } else if (ascii != 0) {
                        if (buffer_index < INPUT_BUFFER_SIZE - 1) {
                            input_buffer[buffer_index++] = ascii;
                            fb_write_char(ascii, 7, 0);
                        }
                    }
                }
            }
            pic_acknowledge(interrupt);
            break;

        default:
            pic_acknowledge(interrupt);
            break;
    }
}  // 这是 interrupt_handler 函数的结束

static u8int read_index = 0;

u8int getc(void) {
    if (read_index >= buffer_index) {
        return 0;
    }
    return input_buffer[read_index++];
}

u32int readline(char* buffer, u32int max_length) {
    u32int i = 0;
    u8int ch;
    
    while (i < max_length - 1) {
        ch = getc();

        if (ch == 0) {
            continue;
        }

        if (ch == '\n') {
            buffer[i] = '\0';
            return i;
        }

        buffer[i++] = ch;
    }
    
    buffer[max_length - 1] = '\0';
    return max_length - 1;
}
