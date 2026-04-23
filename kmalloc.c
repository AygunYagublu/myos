#include "kmalloc.h"

extern void terminal_print(const char* str);

/* heap 1MB yer tutur — linker script-dən sonra başlayır */
#define HEAP_START  0x200000   /* 2MB-dan başla */
#define HEAP_SIZE   0x100000   /* 1MB heap */
#define HEAP_END    (HEAP_START + HEAP_SIZE)

/* hər blokun başında bu header olur */
typedef struct block_header {
    size_t size;              /* blokun ölçüsü (header-siz) */
    uint8_t free;             /* 1 = boş, 0 = istifadədə    */
    struct block_header* next;/* növbəti blok                */
} block_header_t;

#define HEADER_SIZE sizeof(block_header_t)

static block_header_t* heap_start_ptr = 0;
static size_t total_allocated = 0;
static size_t total_freed = 0;

/* sadə rəqəmi string-ə çevir */
static void print_num(size_t n) {
    char buf[32];
    int i = 30;
    buf[31] = '\0';
    if (n == 0) { terminal_print("0"); return; }
    while (n > 0 && i >= 0) {
        buf[i--] = '0' + (n % 10);
        n /= 10;
    }
    terminal_print(&buf[i + 1]);
}

void kmalloc_init(void) {
    heap_start_ptr = (block_header_t*)HEAP_START;
    heap_start_ptr->size = HEAP_SIZE - HEADER_SIZE;
    heap_start_ptr->free = 1;
    heap_start_ptr->next = 0;
    total_allocated = 0;
    total_freed = 0;
    terminal_print("kmalloc: heap hazirdir\n");
}

void* kmalloc(size_t size) {
    if (size == 0) return 0;

    /* 4 bayt alignment */
    if (size % 4 != 0)
        size += 4 - (size % 4);

    block_header_t* curr = heap_start_ptr;

    while (curr) {
        if (curr->free && curr->size >= size) {
            /* bloku böl — əgər kifayət qədər yer varsa */
            if (curr->size >= size + HEADER_SIZE + 4) {
                block_header_t* new_block = (block_header_t*)
                    ((uint8_t*)curr + HEADER_SIZE + size);
                new_block->size = curr->size - size - HEADER_SIZE;
                new_block->free = 1;
                new_block->next = curr->next;
                curr->next = new_block;
                curr->size = size;
            }
            curr->free = 0;
            total_allocated += size;
            return (void*)((uint8_t*)curr + HEADER_SIZE);
        }
        curr = curr->next;
    }

    terminal_print("kmalloc: yaddash yoxdur!\n");
    return 0;
}

void kfree(void* ptr) {
    if (!ptr) return;

    block_header_t* header = (block_header_t*)
        ((uint8_t*)ptr - HEADER_SIZE);
    header->free = 1;
    total_freed += header->size;

    /* qonşu boş blokları birləşdir (coalescing) */
    block_header_t* curr = heap_start_ptr;
    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += HEADER_SIZE + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}

void kmalloc_stats(void) {
    terminal_print("\n  -- Yaddash statistikasi --\n");
    terminal_print("  Ayrilan: ");
    print_num(total_allocated);
    terminal_print(" bayt\n");
    terminal_print("  Serbest buraxilan: ");
    print_num(total_freed);
    terminal_print(" bayt\n");

    /* boş blokları say */
    size_t free_blocks = 0;
    size_t free_bytes = 0;
    block_header_t* curr = heap_start_ptr;
    while (curr) {
        if (curr->free) {
            free_blocks++;
            free_bytes += curr->size;
        }
        curr = curr->next;
    }
    terminal_print("  Bosh blok: ");
    print_num(free_blocks);
    terminal_print("\n  Bosh yaddash: ");
    print_num(free_bytes);
    terminal_print(" bayt\n");
}