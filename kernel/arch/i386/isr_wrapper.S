#define ASM_FILE        1
.intel_syntax noprefix

.extern default_interrupt_handler, default_exception_handler, keyboard_handler
.extern timer_handler
.section .text
.macro interrupt_handler_stub handler_function
    pusha # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    lea eax, [esp + 52]
    mov dword ptr[esp + 12], eax
	mov ax, ds # Lower 16-bits of eax = ds.
	push eax # save the data segment descriptor
	mov ax, 0x10  # kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    push esp
    call \handler_function
    pop esp
    pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8 # Cleans up the pushed error code and pushed ISR number
	iret # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
.endm

.global isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31
.type isr0, @function
.type isr1, @function
.type isr2, @function
.type isr3, @function
.type isr4, @function
.type isr5, @function
.type isr6, @function
.type isr7, @function
.type isr8, @function
.type isr9, @function
.type isr10, @function
.type isr11, @function
.type isr12, @function
.type isr13, @function
.type isr14, @function
.type isr15, @function
.type isr16, @function
.type isr17, @function
.type isr18, @function
.type isr19, @function
.type isr20, @function
.type isr21, @function
.type isr22, @function
.type isr23, @function
.type isr24, @function
.type isr25, @function
.type isr26, @function
.type isr27, @function
.type isr28, @function
.type isr29, @function
.type isr30, @function
.type isr31, @function
# 0: Divide By Zero Exception
isr0:
    push  0
    push  0
    interrupt_handler_stub default_exception_handler

# 1: Debug Exception
isr1:
    push  0
    push  1
    interrupt_handler_stub default_exception_handler

# 2: Non Maskable Interrupt Exception
isr2:
    push  0
    push  2
    interrupt_handler_stub default_exception_handler

# 3: Int 3 Exception
isr3:
    push  0
    push  3
    interrupt_handler_stub default_exception_handler

# 4: INTO Exception
isr4:
    push  0
    push  4
    interrupt_handler_stub default_exception_handler

# 5: Out of Bounds Exception
isr5:
    push  0
    push  5
    interrupt_handler_stub default_exception_handler

# 6: Invalid Opcode Exception
isr6:
    push  0
    push  6
    interrupt_handler_stub default_exception_handler

# 7: Coprocessor Not Available Exception
isr7:
    push  0
    push  7
    interrupt_handler_stub default_exception_handler

# 8: Double Fault Exception (With Error Code!)
isr8:
    push  8
    interrupt_handler_stub default_exception_handler

# 9: Coprocessor Segment Overrun Exception
isr9:
    push  0
    push  9
    interrupt_handler_stub default_exception_handler

# 10: Bad TSS Exception (With Error Code!)
isr10:
    push  10
    interrupt_handler_stub default_exception_handler

# 11: Segment Not Present Exception (With Error Code!)
isr11:
    push  11
    interrupt_handler_stub default_exception_handler

# 12: Stack Fault Exception (With Error Code!)
isr12:
    push  12
    interrupt_handler_stub default_exception_handler

# 13: General Protection Fault Exception (With Error Code!)
isr13:
    push  13
    interrupt_handler_stub default_exception_handler

# 14: Page Fault Exception (With Error Code!)
isr14:
    push  14
    interrupt_handler_stub default_exception_handler

# 15: Reserved Exception
isr15:
    push  0
    push  15
    interrupt_handler_stub default_exception_handler

# 16: Floating Point Exception
isr16:
    push  0
    push  16
    interrupt_handler_stub default_exception_handler

# 17: Alignment Check Exception
isr17:
    push  0
    push  17
    interrupt_handler_stub default_exception_handler

# 18: Machine Check Exception
isr18:
    push  0
    push  18
    interrupt_handler_stub default_exception_handler

# 19: Reserved
isr19:
    push  0
    push  19
    interrupt_handler_stub default_exception_handler

# 20: Reserved
isr20:
    push  0
    push  20
    interrupt_handler_stub default_exception_handler

# 21: Reserved
isr21:
    push  0
    push  21
    interrupt_handler_stub default_exception_handler

# 22: Reserved
isr22:
    push  0
    push  22
    interrupt_handler_stub default_exception_handler

# 23: Reserved
isr23:
    push  0
    push  23
    interrupt_handler_stub default_exception_handler

# 24: Reserved
isr24:
    push  0
    push  24
    interrupt_handler_stub default_exception_handler

# 25: Reserved
isr25:
    push  0
    push  25
    interrupt_handler_stub default_exception_handler

# 26: Reserved
isr26:
    push  0
    push  26
    interrupt_handler_stub default_exception_handler

# 27: Reserved
isr27:
    push  0
    push  27
    interrupt_handler_stub default_exception_handler

# 28: Reserved
isr28:
    push  0
    push  28
    interrupt_handler_stub default_exception_handler

# 29: Reserved
isr29:
    push  0
    push  29
    interrupt_handler_stub default_exception_handler

# 30: Reserved
isr30:
    push  0
    push  30
    interrupt_handler_stub default_exception_handler

# 31: Reserved
isr31:
    push  0
    push  31
    interrupt_handler_stub default_exception_handler

.global isr_pic0, isr_pic1, isr_pic2, isr_pic3, isr_pic4, isr_pic5, isr_pic6, isr_pic7, isr_pic8, isr_pic9, isr_pic10, isr_pic11, isr_pic12, isr_pic13, isr_pic14, isr_pic15
.type isr_pic0, @function
.type isr_pic1, @function
.type isr_pic2, @function
.type isr_pic3, @function
.type isr_pic4, @function
.type isr_pic5, @function
.type isr_pic6, @function
.type isr_pic7, @function
.type isr_pic8, @function
.type isr_pic9, @function
.type isr_pic10, @function
.type isr_pic11, @function
.type isr_pic12, @function
.type isr_pic13, @function
.type isr_pic14, @function
.type isr_pic15, @function

isr_pic0:
	push  0
	push  32
	interrupt_handler_stub timer_handler

isr_pic1:
	push  1
	push  33
	interrupt_handler_stub keyboard_handler

isr_pic2:
	push  2
	push  34
	interrupt_handler_stub default_interrupt_handler

isr_pic3:
	push  3
	push  35
	interrupt_handler_stub default_interrupt_handler

isr_pic4:
	push  4
	push  36
	interrupt_handler_stub default_interrupt_handler

isr_pic5:
	push  5
	push  37
	interrupt_handler_stub default_interrupt_handler

isr_pic6:
	push  6
	push  38
	interrupt_handler_stub default_interrupt_handler

isr_pic7:
	push  7
	push  39
	interrupt_handler_stub default_interrupt_handler

isr_pic8:
	push  8
	push  40
	interrupt_handler_stub default_interrupt_handler

isr_pic9:
	push  9
	push  41
	interrupt_handler_stub default_interrupt_handler

isr_pic10:
	push  10
	push  42
	interrupt_handler_stub default_interrupt_handler

isr_pic11:
	push  11
	push  43
	interrupt_handler_stub default_interrupt_handler

isr_pic12:
	push  12
	push  44
	interrupt_handler_stub default_interrupt_handler

isr_pic13:
	push  13
	push  45
	interrupt_handler_stub default_interrupt_handler

isr_pic14:
	push  14
	push  46
	interrupt_handler_stub default_interrupt_handler

isr_pic15:
	push  15
	push  47
	interrupt_handler_stub default_interrupt_handler