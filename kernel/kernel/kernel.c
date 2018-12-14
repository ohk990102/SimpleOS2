#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/multiboot2.h>
#include <kernel/init.h>
#include <kernel/keyboard.h>
#include <arch/i386/task_rr.h>
#include <arch/i386/vga.h>
#include <arch/i386/pit.h>

void testTask1() {
    uint8_t data;
    int i = 0, x = 0, y = 0, margin;
    struct RRTaskControlBlock * runningTask;
    runningTask = getRunningTask();
    margin = (runningTask->link.id & 0x7FFFFFFF) % 10 + 1;
    while(1) {
        switch(i) {
        case 0:
            x++;
            if(x >= VGA_WIDTH - margin)
                i = 1;
            break;
        case 1:
            y++;
            if(y >= VGA_HEIGHT - margin)
                i = 2;
            break;
        case 2:
            x--;
            if(x < margin)
                i = 3;
            break;
        case 3:
            y--;
            if(y < margin)
                i = 0;
            break;
        }
        terminal_putentryat(data, vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK), x, y);
        data++;
        schedule();
    }
}

void createTestTask(count) {
    for(int i = 0; i < count; i++) {
        if(createTask(0, testTask1) == 0) {
            printf("[!] Something Wrong\n");
        }
    }
    printf("[+] Task1 %d Created\n", count);
}

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

    getchar();
    createTestTask(1023);
    int i = 0;
    while(1);

}
