#ifndef GDT_H
#define GDT_H

#include <stdint.h>

/* Hər GDT entry 8 baytdır */
typedef struct {
    uint16_t limit_low;    /* seqmentin ölçüsü (aşağı 16 bit) */
    uint16_t base_low;     /* başlanğıc ünvan (aşağı 16 bit)  */
    uint8_t  base_mid;     /* başlanğıc ünvan (orta 8 bit)    */
    uint8_t  access;       /* icazə baytı                     */
    uint8_t  granularity;  /* ölçü + flags                    */
    uint8_t  base_high;    /* başlanğıc ünvan (yuxarı 8 bit)  */
} __attribute__((packed)) gdt_entry_t;

/* CPU-ya GDT-nin yerini göstərmək üçün */
typedef struct {
    uint16_t limit;   /* cədvəlin ölçüsü - 1 */
    uint32_t base;    /* cədvəlin ünvanı      */
} __attribute__((packed)) gdt_ptr_t;

void gdt_init(void);

#endif