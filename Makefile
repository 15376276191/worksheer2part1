KERNEL = kernel.elf
ISO = os.iso
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -I./drivers
ASM = nasm
ASMFLAGS = -f elf

all: $(ISO)

$(ISO): iso/boot/$(KERNEL)
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o $(ISO) \
		iso

iso/boot/$(KERNEL): source/loader.o source/kernel.o drivers/framebuffer.o
	ld -T source/link.ld -melf_i386 source/loader.o source/kernel.o drivers/framebuffer.o -o iso/boot/$(KERNEL)

source/loader.o: source/loader.asm
	$(ASM) $(ASMFLAGS) source/loader.asm -o source/loader.o

source/kernel.o: source/kernel.c
	$(CC) $(CFLAGS) -c source/kernel.c -o source/kernel.o

drivers/framebuffer.o: drivers/framebuffer.c drivers/framebuffer.h
	$(CC) $(CFLAGS) -c drivers/framebuffer.c -o drivers/framebuffer.o

run: $(ISO)
	qemu-system-i386 -nographic -serial mon:stdio -boot d -cdrom $(ISO) -m 32

clean:
	rm -f source/*.o drivers/*.o iso/boot/$(KERNEL) $(ISO) logQ.txt

.PHONY: all run clean
