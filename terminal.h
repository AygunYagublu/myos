#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>

/* rəng kodları */
#define COLOR_BLACK        0
#define COLOR_BLUE         1
#define COLOR_GREEN        2
#define COLOR_CYAN         3
#define COLOR_RED          4
#define COLOR_MAGENTA      5
#define COLOR_ORANGE       6
#define COLOR_GRAY         7
#define COLOR_DARK_GRAY    8
#define COLOR_LIGHT_BLUE   9
#define COLOR_LIGHT_GREEN  10
#define COLOR_LIGHT_CYAN   11
#define COLOR_LIGHT_RED    12
#define COLOR_PINK         13
#define COLOR_YELLOW       14
#define COLOR_WHITE        15

void terminal_set_color(uint8_t fg, uint8_t bg);
uint8_t terminal_get_color(void);
void terminal_putchar(char c);
void terminal_print(const char* str);
void terminal_init(void);

#endif