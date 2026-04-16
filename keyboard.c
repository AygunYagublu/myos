#include "keyboard.h"
#include "idt.h"
#include <stdint.h>

#define KEYBOARD_DATA  0x60
#define PIC1_COMMAND   0x20
#define PIC1_DATA      0x21
#define PIC2_COMMAND   0xA0
#define PIC2_DATA      0xA1
#define PIC_EOI        0x20

extern void irq1_handler(void);
extern void idt_set_entry(uint8_t, uint32_t, uint16_t, uint8_t);
extern void terminal_putchar(char c);

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline void io_wait(void) {
    outb(0x80, 0x00);
}

static const char scancode_map[128] = {
    0, '\b', '1','2','3','4','5','6','7','8','9','0','-','=', 0, 0,
    'q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/', 0, 0, 0,' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

extern void shell_process_char(char c);

void keyboard_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA);

    if (!(scancode & 0x80)) {
        char c = scancode_map[scancode & 0x7F];
        if (c) shell_process_char(c);
    }

    outb(PIC1_COMMAND, PIC_EOI);
}

void keyboard_init(void) {
    /* PIC1-i yenidən proqramla */
    outb(PIC1_COMMAND, 0x11); io_wait();
    outb(PIC2_COMMAND, 0x11); io_wait();

    outb(PIC1_DATA, 0x20); io_wait();  /* IRQ0-7 → 32-39 */
    outb(PIC2_DATA, 0x28); io_wait();  /* IRQ8-15 → 40-47 */

    outb(PIC1_DATA, 0x04); io_wait();
    outb(PIC2_DATA, 0x02); io_wait();

    outb(PIC1_DATA, 0x01); io_wait();
    outb(PIC2_DATA, 0x01); io_wait();

    /* yalnız klaviatura IRQ-nu aktiv et, qalanını mask et */
    outb(PIC1_DATA, 0xFD);  /* 11111101 → yalnız IRQ1 açıq */
    outb(PIC2_DATA, 0xFF);  /* hamısı bağlı */

    /* IDT-yə klaviatura handler-ı qeyd et */
    idt_set_entry(33, (uint32_t)irq1_handler, 0x08, 0x8E);

    /* interrupts-ı aktiv et */
    __asm__ volatile ("sti");
}