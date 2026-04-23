CC = i686-linux-gnu-gcc
AS = nasm
LD = i686-linux-gnu-gcc

CFLAGS  = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32
LDFLAGS = -ffreestanding -O2 -nostdlib -m32 -lgcc

OBJS = boot.o gdt_flush.o gdt.o idt_flush.o idt.o isr.o timer.o keyboard.o kmalloc.o shell.o kernel.o

myos.bin: $(OBJS) linker.ld
	$(LD) -T linker.ld -o $@ $(LDFLAGS) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) -f elf32 $< -o $@

iso: myos.bin
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/
	printf 'set timeout=0\nset default=0\nmenuentry "MyOS" {\n  multiboot /boot/myos.bin\n}\n' > isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

run: iso
	qemu-system-i386 -cdrom myos.iso

clean:
	rm -f *.o myos.bin myos.iso
	rm -rf isodir