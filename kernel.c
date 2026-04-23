#include <stdint.h>
#include <stddef.h>
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "shell.h"
#include "kmalloc.h"  
#include "timer.h"
#include "terminal.h"
#include "speaker.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

static uint16_t* vga_buffer;
static size_t terminal_row;
static size_t terminal_col;
static uint8_t terminal_color;

static void print_centered(const char* str) {
    /* 80 sütunlu ekranda ortala */
    size_t len = 0;
    while (str[len]) len++;
    size_t pad = (VGA_WIDTH - len) / 2;
    for (size_t i = 0; i < pad; i++)
        terminal_putchar(' ');
    terminal_print(str);
    terminal_putchar('\n');
}

static void boot_screen(void) {
    terminal_init();

    /* ASCII art — MyOS */
    terminal_set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    print_centered(" __  __       ____  ____  ");
    print_centered("|  \\/  |_   _/ __ \\/ ___| ");
    print_centered("| |\\/| | | | | |  | |     ");
    print_centered("| |  | | |_| | |__| |___  ");
    print_centered("|_|  |_|\\__, |\\____/\\____| ");
    print_centered("        |___/              ");

    terminal_putchar('\n');

    terminal_set_color(COLOR_YELLOW, COLOR_BLACK);
    print_centered("v0.1 -- Mini x86 OS Kernel");

    terminal_putchar('\n');

    terminal_set_color(COLOR_GRAY, COLOR_BLACK);
    print_centered("C ve Assembly ile sifirdan yazilmishdir");

    terminal_putchar('\n');
    terminal_putchar('\n');

    /* sistem bilgisi */
    terminal_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    print_centered("[ GDT OK ] [ IDT OK ] [ PIT OK ] [ MEM OK ]");

    terminal_putchar('\n');
    terminal_putchar('\n');

    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    print_centered("'help' yaz - komandalar ucun");

    terminal_putchar('\n');
    /* boot səsi — iki qısa bip */
speaker_beep(800, 100);
sleep_ms(50);
speaker_beep(1200, 150);
}

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

static void terminal_scroll(void) {
    /* bütün sətirləri bir yuxarı köçür */
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];

    /* son sətiri təmizlə */
    for (size_t x = 0; x < VGA_WIDTH; x++)
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = make_entry(' ', terminal_color);

    terminal_row = VGA_HEIGHT - 1;
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_col = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT)
            terminal_scroll();
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
        if (terminal_row >= VGA_HEIGHT)
            terminal_scroll();
    }
}

void terminal_set_color(uint8_t fg, uint8_t bg) {
    terminal_color = make_color(fg, bg);
}

uint8_t terminal_get_color(void) {
    return terminal_color;
}

void terminal_print(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        terminal_putchar(str[i]);
}

void kernel_main(void) {
    gdt_init();
    idt_init();
    kmalloc_init();
    timer_init(100);
    keyboard_init();
    boot_screen();
    shell_init();
    while (1) {}
}