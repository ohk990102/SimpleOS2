#include <stdint.h>
#include <stdio.h>
#include <arch/i386/idt.h>

struct interrupt_frame;

__attribute__((interrupt)) void default_isr(struct interrupt_frame *frame) {
    printf("[!] Default ISR called\n");
}
__attribute__((interrupt)) void keyboard_isr(struct interrupt_frame *frame) {
    
}