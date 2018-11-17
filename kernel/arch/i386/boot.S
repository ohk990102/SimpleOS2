#define ASM_FILE        1
#include <kernel/multiboot2.h>

/*  C symbol format. HAVE_ASM_USCORE is defined by configure. */
#ifdef HAVE_ASM_USCORE
#define EXT_C(sym)                     _ ## sym
#else
#define EXT_C(sym)                     sym
#endif

/*  The size of our stack (16KB). */
#define STACK_SIZE                      0x4000

.section .multiboot
multiboot_header:
multiboot_magic:
    /*  magic */
    .long   MULTIBOOT2_HEADER_MAGIC
    /*  ISA: i386 */
    .long   MULTIBOOT_ARCHITECTURE_I386
    /*  Header length. */
    .long   multiboot_header_end - multiboot_header
    /*  checksum */
    .long   -(MULTIBOOT2_HEADER_MAGIC + MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))
multiboot_magic_end:
    .short MULTIBOOT_HEADER_TAG_END
    .short 0
    .long 8
multiboot_header_end:
.section .text
.global multiboot_entry
multiboot_entry:
    /*  Initialize the stack pointer. */
    movl    $(stack + STACK_SIZE), %esp
    call    _init
    pushl   %ebx
    pushl   %eax
    call    kernel_main
loop:
    hlt
    jmp loop

.section .bss
.comm stack, STACK_SIZE

