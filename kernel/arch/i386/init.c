#include <stdint.h>
#include <kernel/init.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/isr.h>

uint64_t gdt[5];
uint64_t idt[256];

static void init_gdt() {
    gdt[0] = create_global_descriptor(0, 0, 0);
    gdt[1] = create_global_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    gdt[2] = create_global_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    gdt[3] = create_global_descriptor(0, 0x000FFFFF, (GDT_CODE_PL3));
    gdt[4] = create_global_descriptor(0, 0x000FFFFF, (GDT_DATA_PL3));

    set_gdt(gdt, sizeof(gdt) - 1);
}
static void init_idt() {
    int i;
    idt[0] = create_interrupt_descriptor(isr0, 0x08, IDT_INTERRUPT_GATE);
    idt[1] = create_interrupt_descriptor(isr1, 0x08, IDT_INTERRUPT_GATE);
    idt[2] = create_interrupt_descriptor(isr2, 0x08, IDT_INTERRUPT_GATE);
    idt[3] = create_interrupt_descriptor(isr3, 0x08, IDT_INTERRUPT_GATE);
    idt[4] = create_interrupt_descriptor(isr4, 0x08, IDT_INTERRUPT_GATE);
    idt[5] = create_interrupt_descriptor(isr5, 0x08, IDT_INTERRUPT_GATE);
    idt[6] = create_interrupt_descriptor(isr6, 0x08, IDT_INTERRUPT_GATE);
    idt[7] = create_interrupt_descriptor(isr7, 0x08, IDT_INTERRUPT_GATE);
    idt[8] = create_interrupt_descriptor(isr8, 0x08, IDT_INTERRUPT_GATE);
    idt[9] = create_interrupt_descriptor(isr9, 0x08, IDT_INTERRUPT_GATE);
    idt[10] = create_interrupt_descriptor(isr10, 0x08, IDT_INTERRUPT_GATE);
    idt[11] = create_interrupt_descriptor(isr11, 0x08, IDT_INTERRUPT_GATE);
    idt[12] = create_interrupt_descriptor(isr12, 0x08, IDT_INTERRUPT_GATE);
    idt[13] = create_interrupt_descriptor(isr13, 0x08, IDT_INTERRUPT_GATE);
    idt[14] = create_interrupt_descriptor(isr14, 0x08, IDT_INTERRUPT_GATE);
    idt[15] = create_interrupt_descriptor(isr15, 0x08, IDT_INTERRUPT_GATE);
    idt[16] = create_interrupt_descriptor(isr16, 0x08, IDT_INTERRUPT_GATE);
    idt[17] = create_interrupt_descriptor(isr17, 0x08, IDT_INTERRUPT_GATE);
    idt[18] = create_interrupt_descriptor(isr18, 0x08, IDT_INTERRUPT_GATE);
    idt[19] = create_interrupt_descriptor(isr19, 0x08, IDT_INTERRUPT_GATE);
    idt[20] = create_interrupt_descriptor(isr20, 0x08, IDT_INTERRUPT_GATE);
    idt[21] = create_interrupt_descriptor(isr21, 0x08, IDT_INTERRUPT_GATE);
    idt[22] = create_interrupt_descriptor(isr22, 0x08, IDT_INTERRUPT_GATE);
    idt[23] = create_interrupt_descriptor(isr23, 0x08, IDT_INTERRUPT_GATE);
    idt[24] = create_interrupt_descriptor(isr24, 0x08, IDT_INTERRUPT_GATE);
    idt[25] = create_interrupt_descriptor(isr25, 0x08, IDT_INTERRUPT_GATE);
    idt[26] = create_interrupt_descriptor(isr26, 0x08, IDT_INTERRUPT_GATE);
    idt[27] = create_interrupt_descriptor(isr27, 0x08, IDT_INTERRUPT_GATE);
    idt[28] = create_interrupt_descriptor(isr28, 0x08, IDT_INTERRUPT_GATE);
    idt[29] = create_interrupt_descriptor(isr29, 0x08, IDT_INTERRUPT_GATE);
    idt[30] = create_interrupt_descriptor(isr30, 0x08, IDT_INTERRUPT_GATE);
    idt[31] = create_interrupt_descriptor(isr31, 0x08, IDT_INTERRUPT_GATE);
    set_idt(idt, 256 * 8 - 1);
}

void init(void * addr) {
    init_gdt();
    init_idt();
}
