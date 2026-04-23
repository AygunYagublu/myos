#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdint.h>

void speaker_beep(uint32_t frequency, uint32_t duration_ms);
void speaker_off(void);
void sleep_ms(uint32_t ms);

#endif