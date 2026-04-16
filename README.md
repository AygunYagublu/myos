cat > README.md << 'EOF'
# MyOS

A minimal operating system kernel written from scratch in C and Assembly.

## Features
- Bootable via GRUB (Multiboot)
- GDT (Global Descriptor Table)
- IDT (Interrupt Descriptor Table)
- Keyboard driver (IRQ1)
- Interactive shell (help, clear, about, reboot)

## Build & Run
```bash
sudo apt install build-essential nasm qemu-system-x86 xorriso grub-pc-bin
make run
```

## Tech Stack
- C (freestanding, no stdlib)
- x86 Assembly (NASM)
- QEMU (emulator)

## Project Structure
- boot.asm      — kernel entry point
- kernel.c      — VGA terminal, kernel main
- gdt.c/asm     — Global Descriptor Table
- idt.c/asm     — Interrupt Descriptor Table
- keyboard.c    — keyboard driver
- shell.c       — interactive shell
- linker.ld     — linker script
EOF
