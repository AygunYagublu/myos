#include "timer.h"
#include "idt.h"

extern void terminal_print(const char* str);
extern void irq0_handler(void);

#define PIC1_COMMAND  0x20
#define PIC_EOI       0x20
#define PIT_CHANNEL0  0x40
#define PIT_COMMAND   0x43
#define PIT_BASE_FREQ 1193180

static volatile uint32_t ticks = 0;
static uint32_t frequency = 0;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void timer_handler(void) {
    ticks++;
    outb(PIC1_COMMAND, PIC_EOI);
}

uint32_t timer_get_ticks(void) {
    return ticks;
}

uint32_t timer_get_seconds(void) {
    return ticks / frequency;
}

void timer_init(uint32_t freq) {
    frequency = freq;

    uint32_t divisor = PIT_BASE_FREQ / freq;

    outb(PIT_COMMAND,  0x36);
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));

    extern void idt_set_entry(uint8_t, uint32_t, uint16_t, uint8_t);
    idt_set_entry(32, (uint32_t)irq0_handler, 0x08, 0x8E);

    terminal_print("timer: hazirdir\n");
}