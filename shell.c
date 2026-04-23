#include "shell.h"
#include "kmalloc.h"
#include "timer.h"
#include "terminal.h"
#include <stdint.h>
#include <stddef.h>

#define BUFFER_SIZE 256

static char buffer[BUFFER_SIZE];
static size_t buffer_pos = 0;

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

static void print_num(uint32_t n) {
    char buf[16];
    int i = 14;
    buf[15] = '\0';
    if (n == 0) { terminal_print("0"); return; }
    while (n > 0 && i >= 0) {
        buf[i--] = '0' + (n % 10);
        n /= 10;
    }
    terminal_print(&buf[i + 1]);
}

static void shell_prompt(void) {
    terminal_print("\n");
    terminal_set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    terminal_print("MyOS");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("> ");
}

static void cmd_help(void) {
    terminal_print("\n");
    terminal_set_color(COLOR_YELLOW, COLOR_BLACK);
    terminal_print("  Movcud komandalar:\n");
    terminal_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    terminal_print("  help    ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("- komandalar siyahisi\n");
    terminal_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    terminal_print("  clear   ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("- ekrani temizle\n");
    terminal_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    terminal_print("  about   ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("- kernel haqqinda\n");
    terminal_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    terminal_print("  reboot  ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("- yeniden bashlat\n");
    terminal_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    terminal_print("  memstat ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("- yaddash statistikasi\n");
    terminal_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    terminal_print("  memtest ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("- yaddash testi\n");
    terminal_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    terminal_print("  uptime  ");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("- nece saniye ishleyir\n");
}

static void cmd_about(void) {
    terminal_print("\n");
    terminal_set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    terminal_print("  MyOS v0.1\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print("  C ve Assembly ile yazilmish mini kernel\n");
    terminal_print("  Ozellikleri:\n");
    terminal_set_color(COLOR_GRAY, COLOR_BLACK);
    terminal_print("  - GDT, IDT, PIC, PIT\n");
    terminal_print("  - PS/2 klaviatura driver\n");
    terminal_print("  - kmalloc/kfree\n");
    terminal_print("  - Rengli shell\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
}

static void cmd_reboot(void) {
    terminal_set_color(COLOR_LIGHT_RED, COLOR_BLACK);
    terminal_print("\nYeniden bashlanir...\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    __asm__ volatile (
        "movb $0xFE, %al \n"
        "outb %al, $0x64 \n"
    );
}

static void cmd_clear(void) {
    terminal_init();
}

static void cmd_memtest(void) {
    terminal_print("\n");
    terminal_set_color(COLOR_YELLOW, COLOR_BLACK);
    terminal_print("  kmalloc testi bashlayir...\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    void* a = kmalloc(64);
    void* b = kmalloc(128);
    void* c = kmalloc(32);
    terminal_print("  3 blok ayrildi\n");
    kfree(b);
    terminal_print("  ortadaki blok serbest buraxildi\n");
    void* d = kmalloc(64);
    terminal_print("  yeni blok ayrildi\n");
    kfree(a);
    kfree(c);
    kfree(d);
    terminal_set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    terminal_print("  test ugurlu!\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
}

static void cmd_uptime(void) {
    terminal_print("\n  Kernel ishleme vaxti: ");
    terminal_set_color(COLOR_YELLOW, COLOR_BLACK);
    print_num(timer_get_seconds());
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    terminal_print(" saniye\n");
}

static void execute(void) {
    terminal_print("\n");

    if (buffer_pos == 0) {
        /* boş enter */
    } else if (streq(buffer, "help")) {
        cmd_help();
    } else if (streq(buffer, "clear")) {
        cmd_clear();
    } else if (streq(buffer, "about")) {
        cmd_about();
    } else if (streq(buffer, "reboot")) {
        cmd_reboot();
    } else if (streq(buffer, "memstat")) {
        kmalloc_stats();
    } else if (streq(buffer, "memtest")) {
        cmd_memtest();
    } else if (streq(buffer, "uptime")) {
        cmd_uptime();
    } else {
        terminal_set_color(COLOR_LIGHT_RED, COLOR_BLACK);
        terminal_print("  bilinmeyen komanda: ");
        terminal_print(buffer);
        terminal_print("\n");
        terminal_set_color(COLOR_GRAY, COLOR_BLACK);
        terminal_print("  'help' yaz - komandalar ucun");
        terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    }

    buffer_clear();
    shell_prompt();
}

void shell_process_char(char c) {
    if (c == '\n') {
        execute();
        return;
    }

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
    terminal_set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    terminal_print("MyOS v0.1\n");
    terminal_set_color(COLOR_GRAY, COLOR_BLACK);
    terminal_print("'help' yaz - komandalar ucun\n");
    terminal_set_color(COLOR_WHITE, COLOR_BLACK);
    shell_prompt();
}