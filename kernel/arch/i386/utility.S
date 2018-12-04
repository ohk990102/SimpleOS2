
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
    ret

.global switch_context
.type switch_context, @function

switch_context:
    push ebp
    mov ebp, esp
    pushfd
    cmp dword ptr [ebp+8], 0
    je load_context
    popfd
    
    push edi
    push eax
    mov edi, [ebp + 8]

    mov ax, ss
    mov dword ptr [edi + 48], ax # ss

    mov eax, ebp
    add eax, 8
    mov dword ptr [edi + 44], eax # useresp
    
    pushfd
    pop eax
    mov dword ptr [edi + 40], eax # eflags

    mov ax, cs
    mov dword ptr [edi + 36], ax # cs

    mov eax, dword ptr [ebp + 4]
    mov dword ptr [edi + 32], eax # eip

    pop eax
    pop edi
    pop ebp

    mov esp, dword ptr [esp + 4]
    add esp, 8 * 4
    pusha

    mov esp, dword ptr [esp + 44]

load_context:
    mov esp, [esp + 4]
    mov eax, esp
    mov esp, [esp + 44]
    
    push [eax+40]
    push [eax+36]
    push [eax+32]

    mov esp, eax
    popa
    mov esp, [esp+12]
    sub esp, 12
    iret




