
#define ASM_FILE        1
.intel_syntax noprefix

.section .bss
gdtr:
    .short 0
    .long 0
idtr:
    .short 0
    .long 0
.section .text
.global set_gdt, set_idt
.type set_gdt, @function
.type set_idt, @function

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


set_idt:
    mov eax, [esp+4]
    mov [idtr+2], eax
    mov ax, [esp+8]
    mov [idtr], ax
    lidt [idtr]
    sti
    ret