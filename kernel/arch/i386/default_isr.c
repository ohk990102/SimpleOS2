#include <stdint.h>
#include <stdio.h>
#include <arch/i386/isr.h>

void default_interrupt_handler(int _r) {
    registers_t * r = GET_REGISTER(_r);
    printf("[!] Default ISR called : %d, %d\n", r->int_no, r->err_code);
    return;
}
