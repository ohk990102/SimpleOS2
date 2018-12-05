#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/multiboot2.h>
#include <kernel/init.h>
#include <kernel/keyboard.h>
#include <arch/i386/task.h>

static struct TaskControlBlock tcb[2] = {0, };
static uint32_t stack[1024] = {0, };
void testTask() {
    int i = 0; 
    while(1) {
        printf("[%d] This is from testTask. Press any key to switch\n", i++);
        getchar();
        
        switch_context(&(tcb[1].context), &(tcb[0].context));
    }
}

void createTestTask() {
    int i = 0;
    setup_task(&(tcb[1]), 1, 0, testTask, stack, sizeof(stack));
    while(1) {
        printf("[%d] This is from createTestTask. Press any key to switch\n", i++);
        getchar();
        switch_context(&(tcb[0].context), &(tcb[1].context));
    }

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
        printf("[!] Error occured while initializing keyboard");
    }
    __asm__("sti");

    createTestTask();

    //abort();

}
