#define ASM_FILE        1
#include <kernel/multiboot2.h>
.intel_syntax noprefix
/*  C symbol format. HAVE_ASM_USCORE is defined by configure. */
#ifdef HAVE_ASM_USCORE
#define EXT_C(sym)                     _ ## sym
#else
#define EXT_C(sym)                     sym
#endif

/*  The size of our stack (16KB). */
#define STACK_SIZE                      0x4000

.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096

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
    cli
    /* This is physical address */
    /* 4KB paging */
    /* Base address is 0xC0000000 */
    mov     edi, offset (boot_page_table1 - 0xC0000000)
    mov     esi, 0
    mov     ecx, 1023
1:
    cmp     esi, offset (_kernel_start - 0xC0000000)
    jl      2f
    cmp     esi, offset (_kernel_end - 0xC0000000)
    jge     3f

    mov     edx, esi
    or      edx, 0x0003
    mov     [edi], edx
2:
    add     esi, 4096
    add     edi, 4
    loop    1b
3:
    mov     dword ptr (boot_page_table1 - 0xC0000000 + 1023 * 4), (0x000B8000 | 0x003)
    mov     dword ptr (boot_page_directory - 0xC0000000 + 0), offset (boot_page_table1 - 0xC0000000 + 0x003)
    mov     dword ptr (boot_page_directory - 0xC0000000 + 768 * 4), offset (boot_page_table1 - 0xC0000000 + 0x003)

    # set cr3 to boot_page_directory
    mov     ecx, offset (boot_page_directory - 0xC0000000)
    mov     cr3, ecx
    # enable paging and write-protect bit
    mov     ecx, cr0
    or      ecx, 0x80010000
    mov     cr0, ecx

    # paging is enabled, but still in 0x00000000 base address
    # move to higher address(0xC0000000)
    lea     ecx, 4f
    jmp     ecx
4:
    # now in higher address, feeling good
	# Unmap the identity mapping as it is now unnecessary. 
	mov     dword ptr boot_page_directory, 0

	# Reload crc3 to force a TLB flush so the changes to take effect.
	mov     ecx, cr3
    mov     cr3, ecx

    mov     esp, offset (stack + STACK_SIZE)
    push    ebx
    push    eax
    call    _init
    call    kernel_main
loop:
    hlt
    jmp loop

.section .bootstrap_stack, "aw", @nobits
stack:
.skip STACK_SIZE


