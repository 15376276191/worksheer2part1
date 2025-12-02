global loader
extern kernel_main

MAGIC_NUMBER equ 0x1BADB002
FLAGS        equ 0x0
CHECKSUM     equ -MAGIC_NUMBER

section .multiboot
align 4
    dd MAGIC_NUMBER
    dd FLAGS
    dd CHECKSUM

section .text
loader:
    mov eax, 0x12345678
    mov esp, kernel_stack
    call kernel_main
    mov eax, 0x87654321

.loop:
    jmp .loop

section .bss
resb 8192
kernel_stack:
