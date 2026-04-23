# MyOS

A minimal x86 operating system kernel written from scratch in C and Assembly, with no external libraries or existing OS dependencies.

![Build](https://img.shields.io/badge/build-passing-brightgreen)
![Language](https://img.shields.io/badge/language-C%20%2B%20Assembly-blue)
![Arch](https://img.shields.io/badge/arch-x86__32-orange)
![License](https://img.shields.io/badge/license-MIT-green)

## Features
- Bootable via GRUB (Multiboot specification)
- GDT (Global Descriptor Table) — memory segmentation
- IDT (Interrupt Descriptor Table) — interrupt handling
- PIC (Programmable Interrupt Controller) — hardware interrupts
- PIT (Programmable Interval Timer) — system timer at 100Hz
- PS/2 Keyboard driver (IRQ1) — real-time input
- PC Speaker driver — boot beep and beep command
- Memory allocator — kmalloc/kfree with block coalescing
- Terminal scroll support
- Colorful boot screen with ASCII art
- Interactive shell with 11 commands

## Shell Commands
| Command       | Description                          |
|---------------|--------------------------------------|
| `help`        | List all available commands          |
| `clear`       | Clear the screen                     |
| `about`       | Show kernel info                     |
| `reboot`      | Reboot the system                    |
| `memtest`     | Run memory allocator test            |
| `memstat`     | Show memory usage statistics         |
| `uptime`      | Show kernel uptime in seconds        |
| `date`        | Show time since boot (h/m/s)         |
| `version`     | Show kernel version                  |
| `sysinfo`     | Show full system information         |
| `echo <text>` | Print text to screen                 |
| `beep`        | Play a beep sound                    |
| `color <name>`| Change shell color (red/green/cyan/yellow/pink/white/gray/blue) |

## Build & Run

### Requirements
```bash
## What I Learned
- How a CPU boots and transitions to protected mode
- Low-level hardware programming (PIC, PIT, PS/2, PC Speaker)
- Memory management without standard libraries
- x86 Assembly and C interoperability
- Interrupt-driven I/O
- Building a complete software stack from scratch

## Tech Stack
- C (freestanding, no stdlib)
- x86 Assembly (NASM)
- QEMU (emulator)
- GRUB (bootloader)
- GNU Make
