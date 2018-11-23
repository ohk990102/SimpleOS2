#include <stdint.h>
#include <stdio.h>
#include <arch/i386/interrupt.h>

void default_isr_handler(int _r) {
    registers_t * r = GET_REGISTER(_r);
    printf("[!] Default ISR called : %d, %d\n", r->int_no, r->err_code);
    return;
}
