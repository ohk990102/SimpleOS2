#include <stdint.h>
#include <stdio.h>
#include <arch/i386/interrupt.h>

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}
void default_irq_handler(int _r) {
    registers_t * r = GET_REGISTER(_r);
    if (r->int_no >= 40) outb(0xA0, 0x20); /* slave */
    outb(0x20, 0x20); /* master */
    printf("[!] Default IRQ called : %d, %d\n", r->int_no, r->err_code);
}
