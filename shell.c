#include "shell.h"
#include <stdint.h>
#include <stddef.h>

#define BUFFER_SIZE 256

extern void terminal_putchar(char c);
extern void terminal_print(const char* str);
extern void terminal_init(void);

static char buffer[BUFFER_SIZE];
static size_t buffer_pos = 0;

/* iki stringi müqayisə et */
static int streq(const char* a, const char* b) {
    size_t i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

static void buffer_clear(void) {
    for (size_t i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = 0;
    buffer_pos = 0;
}

static void shell_prompt(void) {
    terminal_print("\nMyOS> ");
}

static void cmd_help(void) {
    terminal_print("\n");
    terminal_print("  help   - komandalar siyahisi\n");
    terminal_print("  clear  - ekrani temizle\n");
    terminal_print("  about  - kernel haqqinda\n");
    terminal_print("  reboot - yeniden bashlat\n");
}

static void cmd_about(void) {
    terminal_print("\n");
    terminal_print("  MyOS v0.1\n");
    terminal_print("  Yazan: sen!\n");
    terminal_print("  C ve Assembly ile yazilmish mini kernel\n");
}

static void cmd_reboot(void) {
    terminal_print("\nYeniden bashlanir...\n");
    __asm__ volatile (
        "movb $0xFE, %al \n"
        "outb %al, $0x64 \n"
    );
}

static void cmd_clear(void) {
    terminal_init();
}

static void execute(void) {
    terminal_print("\n");

    if (buffer_pos == 0) {
        /* boş enter — heç nə etmə */
    } else if (streq(buffer, "help")) {
        cmd_help();
    } else if (streq(buffer, "clear")) {
        cmd_clear();
    } else if (streq(buffer, "about")) {
        cmd_about();
    } else if (streq(buffer, "reboot")) {
        cmd_reboot();
    } else {
        terminal_print("  bilinmeyen komanda: ");
        terminal_print(buffer);
        terminal_print("\n  'help' yaz - komandalar ucun");
    }

    buffer_clear();
    shell_prompt();
}

void shell_process_char(char c) {
    if (c == '\n') {
        execute();
        return;
    }

    /* backspace */
    if (c == '\b') {
        if (buffer_pos > 0) {
            buffer_pos--;
            buffer[buffer_pos] = 0;
            terminal_putchar('\b');
        }
        return;
    }

    if (buffer_pos < BUFFER_SIZE - 1) {
        buffer[buffer_pos++] = c;
        terminal_putchar(c);
    }
}

void shell_init(void) {
    buffer_clear();
    terminal_print("MyOS v0.1 - hazirdir!\n");
    terminal_print("'help' yaz - komandalar ucun\n");
    shell_prompt();
}