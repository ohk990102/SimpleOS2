#include <stdint.h>
#include <arch/i386/idt.h>

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
uint64_t create_interrupt_descriptor(uint32_t offset, uint16_t selector, uint8_t flag) {
    uint64_t descriptor;

    // Create the high 32 bit descriptor
    descriptor  = offset & 0xFFFF0000;
    descriptor |= flag << 8;

    // Shift by 32bit
    descriptor <<= 32;

    // Create the low 32 bit descriptor;
    descriptor |= selector << 16;
    descriptor |= offset & 0x0000FFFF;

    return descriptor;
}
void remap_pic() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);
}