#include <stdint.h>
#include <stddef.h>
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

static uint16_t* vga_buffer;
static size_t terminal_row;
static size_t terminal_col;
static uint8_t terminal_color;

static uint8_t make_color(uint8_t fg, uint8_t bg) {
    return fg | (bg << 4);
}

static uint16_t make_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void terminal_init(void) {
    terminal_row   = 0;
    terminal_col   = 0;
    terminal_color = make_color(15, 0); /* ağ mətn, qara fon */
    vga_buffer     = (uint16_t*)VGA_ADDRESS;

    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            vga_buffer[y * VGA_WIDTH + x] = make_entry(' ', terminal_color);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_col = 0;
        terminal_row++;
        return;
    }

    if (c == '\b') {
        if (terminal_col > 0) {
            terminal_col--;
            vga_buffer[terminal_row * VGA_WIDTH + terminal_col] = make_entry(' ', terminal_color);
        }
        return;
    }

    vga_buffer[terminal_row * VGA_WIDTH + terminal_col] = make_entry(c, terminal_color);
    if (++terminal_col >= VGA_WIDTH) {
        terminal_col = 0;
        terminal_row++;
    }
}

void terminal_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        terminal_putchar(str[i]);
}

void kernel_main(void) {
    gdt_init();
    terminal_init();
    idt_init();
    keyboard_init();
    shell_init();
    while (1) {}
}