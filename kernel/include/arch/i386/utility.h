#ifndef UTILITY_HEADER
#define UTILITY_HEADER 1
#include <stdint.h>
#include <stdbool.h>

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
static inline void initialize_PIC() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);
}

static inline void send_EOI_to_PIC(int int_no) {
    if (int_no >= 40) outb(0xA0, 0x20); /* slave */
    outb(0x20, 0x20); /* master */
}
static inline void enable_interrupt() {
    __asm__("sti");
}
static inline void disable_interrupt() {
    __asm__("cli");
}
static inline uint32_t read_rflag() {
    uint32_t a;
    __asm__(".intel_syntax noprefix;"
            "pushfd;"
            "pop eax;"
            "mov %0, eax;"
            ".att_syntax;"
            : "=r"(a)
            : 
            :"eax");
    return a;
    
}
static inline bool set_interrupt_flag(bool set_interrupt) {

    uint16_t rflag = read_rflag();
    if(set_interrupt)
        __asm__("sti");
    else
        __asm__("cli");
    
    if(rflag & 0x0200)
        return true;
    return false;
}
extern void set_idt(void * idt_base_addr, unsigned short idt_length);
extern void set_gdt(void * gdt_base_addr, unsigned short gdt_length);

#endif