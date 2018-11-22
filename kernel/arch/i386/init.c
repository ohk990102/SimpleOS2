#include <stdint.h>
#include <kernel/init.h>
#include <arch/i386/gdt.h>
#include <arch/i386/idt.h>

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
    for(i = 0; i < IDT_SIZE; i++) {
        idt[i] = create_interrupt_descriptor(default_isr, 0x08, IDT_INTERRUPT_GATE);
    }
    set_idt(idt, sizeof(idt));
}

void init(void * addr) {
    init_gdt();
    init_idt();
}
