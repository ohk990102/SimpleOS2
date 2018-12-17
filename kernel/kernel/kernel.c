#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/multiboot2.h>
#include <kernel/init.h>
#include <kernel/keyboard.h>
#include <arch/i386/task_cfs.h>
#include <arch/i386/vga.h>
#include <arch/i386/pit.h>
#include <kernel/shell.h>


void kernel_main(unsigned long magic, void * addr) {
    terminal_initialize();
    printf("[*] SimpleOS2 Loading...\n");
    if(magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        printf("[!] Multiboot2 Bootloader Magic invalid\n");
        abort();
    }    
    if(((unsigned long) addr) & 7) {
        printf("[!] Multiboot2 Bootloader Information Addr not aligned\n");
        abort();
    }
    /**
    unsigned size = *(unsigned *) addr;
    struct multiboot_tag *tag;
    for(tag = (struct multiboot_tag *)(addr + 8); 
        tag->type != MULTIBOOT_TAG_TYPE_END; 
        tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))) {
    }
    **/

    init_descriptor(addr);
    printf("[+] system init complete\n");
    if(!initialize_keyboard()) {
        printf("[!] Error occured while initializing keyboard\n");
    }
    
    initializeScheduler();
    initialize_pit(MSTOCOUNT(1), 1);

    __asm__("sti");

    startShell();

}
