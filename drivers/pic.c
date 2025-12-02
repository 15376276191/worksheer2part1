#include "pic.h"
#include "io.h"

void pic_remap(s32int offset1, s32int offset2)
{
    unsigned char a1, a2;
    
    // Save masks
    a1 = inb(PIC_1_DATA);
    a2 = inb(PIC_2_DATA);
    
    // Start initialization
    outb(PIC_1_COMMAND, PIC_ICM1_INIT | PIC_ICM1_ICM4);
    outb(PIC_2_COMMAND, PIC_ICM1_INIT | PIC_ICM1_ICM4);
    
    // Set offsets
    outb(PIC_1_DATA, offset1);
    outb(PIC_2_DATA, offset2);
    
    // Tell master PIC about slave at IRQ2
    outb(PIC_1_DATA, 4);
    outb(PIC_2_DATA, 2);
    
    // Set 8086 mode
    outb(PIC_1_DATA, PIC_ICM4_8086);
    outb(PIC_2_DATA, PIC_ICM4_8086);
    
    // Restore masks
    outb(PIC_1_DATA, a1);
    outb(PIC_2_DATA, a2);
}

void pic_acknowledge(u32int interrupt)
{
    if (interrupt >= PIC_2_OFFSET && interrupt <= PIC_2_END) {
        outb(PIC_2_COMMAND, PIC_ACKNOWLEDGE);
    }
    outb(PIC_1_COMMAND, PIC_ACKNOWLEDGE);
}
