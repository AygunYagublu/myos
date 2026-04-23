# MyOS

A minimal x86 operating system kernel written from scratch in C and Assembly, with no external libraries or existing OS dependencies.

## Features
- Bootable via GRUB (Multiboot specification)
- GDT (Global Descriptor Table) — memory segmentation
- IDT (Interrupt Descriptor Table) — interrupt handling
- PIC (Programmable Interrupt Controller) — hardware interrupts
- PIT (Programmable Interval Timer) — system timer
- PS/2 Keyboard driver (IRQ1) — real-time input
- Memory allocator — kmalloc/kfree with block coalescing
- Interactive shell with commands: help, clear, about, reboot, memtest, memstat, uptime

## Build & Run

### Requirements
```bash
sudo apt install build-essential nasm qemu-system-x86 xorriso grub-pc-bin
```

### Run
```bash
git clone https://github.com/AygunYagublu/myos.git
cd myos
make run
```

## Shell Commands
| Command  | Description                        |
|----------|------------------------------------|
| help     | List all available commands        |
| clear    | Clear the screen                   |
| about    | Show kernel info                   |
| reboot   | Reboot the system                  |
| memtest  | Run memory allocator test          |
| memstat  | Show memory usage statistics       |
| uptime   | Show kernel uptime in seconds      |

## Project Structure
## What I Learned
- How a CPU boots and transitions to protected mode
- Low-level hardware programming (PIC, PIT, PS/2)
- Memory management without standard libraries
- x86 Assembly and C interoperability
- Building a complete software stack from scratch

## Tech Stack
- C (freestanding, no stdlib)
- x86 Assembly (NASM)
- QEMU (emulator)
- GRUB (bootloader)
