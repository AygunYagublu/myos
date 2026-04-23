global irq0_handler
global irq1_handler
extern timer_handler
extern keyboard_handler

section .text

irq0_handler:
    cli
    pusha
    call timer_handler
    popa
    sti
    iret

irq1_handler:
    cli
    pusha
    call keyboard_handler
    popa
    sti
    iret