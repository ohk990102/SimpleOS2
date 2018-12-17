#ifndef INTERRUPT_HEADER
#define INTERRUPT_HEADER 1

#include <stdint.h>

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void isr_pic0();
extern void isr_pic1();
extern void isr_pic2();
extern void isr_pic3();
extern void isr_pic4();
extern void isr_pic5();
extern void isr_pic6();
extern void isr_pic7();
extern void isr_pic8();
extern void isr_pic9();
extern void isr_pic10();
extern void isr_pic11();
extern void isr_pic12();
extern void isr_pic13();
extern void isr_pic14();
extern void isr_pic15();

struct __attribute__((__packed__)) registers_t{
   uint32_t ds; /* Data segment selector */
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
   uint32_t int_no, err_code; /* Interrupt number and error code (if applicable) */
   uint32_t eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
};

//void keyboard_handler(registers_t r);
void default_exception_handler(struct registers_t * r);
void default_interrupt_handler(struct registers_t * r);

//#define GET_REGISTER(r) ((registers_t *) &(r))
#endif