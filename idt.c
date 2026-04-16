#include "idt.h"

#define IDT_ENTRIES 256

static idt_entry_t idt[IDT_ENTRIES];
static idt_ptr_t   idt_ptr;

extern void idt_flush(uint32_t);

void idt_set_entry(uint8_t i, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[i].base_low  = base & 0xFFFF;
    idt[i].base_high = (base >> 16) & 0xFFFF;
    idt[i].selector  = sel;
    idt[i].zero      = 0;
    idt[i].flags     = flags;
}

void idt_init(void) {
    idt_ptr.limit = (sizeof(idt_entry_t) * IDT_ENTRIES) - 1;
    idt_ptr.base  = (uint32_t)&idt;

    /* bütün entry-ləri sıfırla */
    for (int i = 0; i < IDT_ENTRIES; i++)
        idt_set_entry(i, 0, 0, 0);

    idt_flush((uint32_t)&idt_ptr);
}