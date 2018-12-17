#include <stdint.h>
#include <arch/i386/idt.h>
#include <arch/i386/utility.h>


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