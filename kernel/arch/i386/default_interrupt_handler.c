#include <stdio.h>
#include <arch/i386/interrupt.h>
#include <arch/i386/utility.h>

void default_exception_handler(struct registers_t * r) {
    printf("Exception : %d\nErrorCode : %d\n", r->int_no, r->err_code);
}

void default_interrupt_handler(struct registers_t * r) {
    send_EOI_to_PIC(r->int_no);
}