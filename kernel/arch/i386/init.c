#include <kernel/init.h>
#include <arch/i386/gdt.h>
#include <arch/i386/utility.h>

uint64_t gdt[3];

void init_gdt() {
    gdt[0] = create_descriptor(0, 0, 0);
    gdt[1] = create_descriptor(0, 0x000FFFFF, (GDT_CODE_PL0));
    gdt[2] = create_descriptor(0, 0x000FFFFF, (GDT_DATA_PL0));
    
    set_gdt(gdt, sizeof(gdt));
}

void init(void * addr) {
    init_gdt();
}
