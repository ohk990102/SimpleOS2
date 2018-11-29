#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/multiboot2.h>
#include <kernel/init.h>
#include <kernel/keyboard.h>

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
        printf("[!] Error occured while initializing keyboard");
    }
    __asm__("sti");

    struct KeyDataStruct data;
    char a[2];
    while(1) {
        a[0] = getchar();
        a[1] = '\x00';
        printf(a);
    }

    //abort();

}
