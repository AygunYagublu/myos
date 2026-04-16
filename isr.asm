global irq1_handler
extern keyboard_handler

section .text

irq1_handler:
    cli
    pusha

    call keyboard_handler

    popa
    sti
    iret