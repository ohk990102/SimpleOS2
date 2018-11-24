#ifndef IDT_HEADER
#define IDT_HEADER 1

#include <stdint.h>

#define IDT_SIZE        0x100
// are names correct?
#define IG_PRES(x)      ((x) << 0x07)  // Present
#define IG_PRIV(x)      (((x) & 0x03) << 0x05) // Descriptor Priviledge Level
#define IG_SSEG(x)      ((x) << 0x04)  // Storage Segment
#define IG_TYPE(x)      ((x) & 0x0F)

#define IDT_INTERRUPT_GATE  IG_PRES(1) |  IG_PRIV(0) | IG_SSEG(0) | IG_TYPE(0b1110)
#define IDT_TRAP_GATE       IG_PRES(1) |  IG_PRIV(0) | IG_SSEG(0) | IG_TYPE(0b1111)
// TODO : Implement Task Gate Descriptor

uint64_t create_interrupt_descriptor(uint32_t offset, uint16_t selector, uint8_t flag);
extern void set_idt(void * idt_base_addr, unsigned short idt_length);
void remap_pic();



#endif