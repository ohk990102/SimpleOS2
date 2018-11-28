#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/multiboot2.h>
#include <kernel/init.h>

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

    init(addr);
    printf("[+] system init complete\n");

    __asm__("int $0");
    __asm__("int $1");
    __asm__("int $2");
    __asm__("int $3");
    __asm__("int $4");
    __asm__("int $5");
    printf("[+] wow\n");

    //abort();

}
