#include <stdint.h>
#include <string.h>
#include <kernel/init.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>
#include <arch/i386/interrupt.h>
#include <arch/i386/utility.h>
#include <kernel/keyboard.h>

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

    idt[32] = create_interrupt_descriptor(isr_pic0, 0x08, IDT_INTERRUPT_GATE);
    idt[33] = create_interrupt_descriptor(isr_pic1, 0x08, IDT_INTERRUPT_GATE);
    idt[34] = create_interrupt_descriptor(isr_pic2, 0x08, IDT_INTERRUPT_GATE);
    idt[35] = create_interrupt_descriptor(isr_pic3, 0x08, IDT_INTERRUPT_GATE);
    idt[36] = create_interrupt_descriptor(isr_pic4, 0x08, IDT_INTERRUPT_GATE);
    idt[37] = create_interrupt_descriptor(isr_pic5, 0x08, IDT_INTERRUPT_GATE);
    idt[38] = create_interrupt_descriptor(isr_pic6, 0x08, IDT_INTERRUPT_GATE);
    idt[39] = create_interrupt_descriptor(isr_pic7, 0x08, IDT_INTERRUPT_GATE);
    idt[40] = create_interrupt_descriptor(isr_pic8, 0x08, IDT_INTERRUPT_GATE);
    idt[41] = create_interrupt_descriptor(isr_pic9, 0x08, IDT_INTERRUPT_GATE);
    idt[42] = create_interrupt_descriptor(isr_pic10, 0x08, IDT_INTERRUPT_GATE);
    idt[43] = create_interrupt_descriptor(isr_pic11, 0x08, IDT_INTERRUPT_GATE);
    idt[44] = create_interrupt_descriptor(isr_pic12, 0x08, IDT_INTERRUPT_GATE);
    idt[45] = create_interrupt_descriptor(isr_pic13, 0x08, IDT_INTERRUPT_GATE);
    idt[46] = create_interrupt_descriptor(isr_pic14, 0x08, IDT_INTERRUPT_GATE);
    idt[47] = create_interrupt_descriptor(isr_pic15, 0x08, IDT_INTERRUPT_GATE);

    initialize_PIC();

    set_idt(idt, sizeof(idt) - 1);
}

void init_descriptor(void * addr) {
    init_gdt();
    init_idt();
}
