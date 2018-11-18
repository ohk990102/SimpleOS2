
#define ASM_FILE        1
.intel_syntax noprefix

.section .bss
gdtr:
    .short 0
    .long 0
.section .text
.global set_gdt
.type set_gdt, @function

set_gdt:
    mov eax, [esp+4]
    mov [gdtr+2], eax
    mov ax, [esp+8]
    mov [gdtr], ax
    lgdt [gdtr]
    jmp 0x08:flush_gdt
flush_gdt:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret

