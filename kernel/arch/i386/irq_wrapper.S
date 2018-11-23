#define ASM_FILE        1
.intel_syntax noprefix

.extern default_irq_handler
.section .text

.global irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7, irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15
.type irq0, @function
.type irq1, @function
.type irq2, @function
.type irq3, @function
.type irq4, @function
.type irq5, @function
.type irq6, @function
.type irq7, @function
.type irq8, @function
.type irq9, @function
.type irq10, @function
.type irq11, @function
.type irq12, @function
.type irq13, @function
.type irq14, @function
.type irq15, @function

irq_common_stub:
    pusha 
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    call default_irq_handler
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret 

irq0:
	cli
	push  0
	push  32
	jmp irq_common_stub

irq1:
	cli
	push  1
	push  33
	jmp irq_common_stub

irq2:
	cli
	push  2
	push  34
	jmp irq_common_stub

irq3:
	cli
	push  3
	push  35
	jmp irq_common_stub

irq4:
	cli
	push  4
	push  36
	jmp irq_common_stub

irq5:
	cli
	push  5
	push  37
	jmp irq_common_stub

irq6:
	cli
	push  6
	push  38
	jmp irq_common_stub

irq7:
	cli
	push  7
	push  39
	jmp irq_common_stub

irq8:
	cli
	push  8
	push  40
	jmp irq_common_stub

irq9:
	cli
	push  9
	push  41
	jmp irq_common_stub

irq10:
	cli
	push  10
	push  42
	jmp irq_common_stub

irq11:
	cli
	push  11
	push  43
	jmp irq_common_stub

irq12:
	cli
	push  12
	push  44
	jmp irq_common_stub

irq13:
	cli
	push  13
	push  45
	jmp irq_common_stub

irq14:
	cli
	push  14
	push  46
	jmp irq_common_stub

irq15:
	cli
	push  15
	push  47
	jmp irq_common_stub