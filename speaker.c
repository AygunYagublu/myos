#include "speaker.h"
#include "timer.h"

#define PIT_CHANNEL2  0x42
#define PIT_COMMAND   0x43
#define PC_SPEAKER    0x61
#define PIT_BASE_FREQ 1193180

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static void speaker_on(uint32_t freq) {
    uint32_t divisor = PIT_BASE_FREQ / freq;

    /* PIT channel 2-ni tənzimlə */
    outb(PIT_COMMAND,  0xB6);
    outb(PIT_CHANNEL2, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2, (uint8_t)((divisor >> 8) & 0xFF));

    /* PC speaker-i aktiv et */
    uint8_t tmp = inb(PC_SPEAKER);
    outb(PC_SPEAKER, tmp | 0x03);
}

void speaker_off(void) {
    uint8_t tmp = inb(PC_SPEAKER);
    outb(PC_SPEAKER, tmp & 0xFC);
}

 void sleep_ms(uint32_t ms) {
    /* timer tick-ləri ilə gözlə (100Hz = 10ms per tick) */
    uint32_t ticks_to_wait = ms / 10;
    if (ticks_to_wait == 0) ticks_to_wait = 1;
    uint32_t start = timer_get_ticks();
    while (timer_get_ticks() - start < ticks_to_wait) {
        __asm__ volatile ("hlt");
    }
}

void speaker_beep(uint32_t freq, uint32_t duration_ms) {
    speaker_on(freq);
    sleep_ms(duration_ms);
    speaker_off();
}