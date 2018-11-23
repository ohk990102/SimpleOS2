#define ASM_FILE        1
.intel_syntax noprefix

.extern default_interrupt_handler
.section .text
default_isr_common_stub:
	pusha # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
	mov ax, ds # Lower 16-bits of eax = ds.
	push eax # save the data segment descriptor
	mov ax, 0x10  # kernel data segment descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
    # 2. Call C handler
	call default_interrupt_handler
	
    # 3. Restore state
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	add esp, 8 # Cleans up the pushed error code and pushed ISR number
	sti
	iret # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
	
# We don't get information about which interrupt was caller
# when the handler is run, so we will need to have a different handler
# for every interrupt.
# Furthermore, some interrupts push an error code onto the stack but others
# don't, so we will push a dummy error code for those which don't, so that
# we have a consistent stack for all of them.

# First make the ISRs global
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
    cli
    push  0
    push  0
    jmp default_isr_common_stub

# 1: Debug Exception
isr1:
    cli
    push  0
    push  1
    jmp default_isr_common_stub

# 2: Non Maskable Interrupt Exception
isr2:
    cli
    push  0
    push  2
    jmp default_isr_common_stub

# 3: Int 3 Exception
isr3:
    cli
    push  0
    push  3
    jmp default_isr_common_stub

# 4: INTO Exception
isr4:
    cli
    push  0
    push  4
    jmp default_isr_common_stub

# 5: Out of Bounds Exception
isr5:
    cli
    push  0
    push  5
    jmp default_isr_common_stub

# 6: Invalid Opcode Exception
isr6:
    cli
    push  0
    push  6
    jmp default_isr_common_stub

# 7: Coprocessor Not Available Exception
isr7:
    cli
    push  0
    push  7
    jmp default_isr_common_stub

# 8: Double Fault Exception (With Error Code!)
isr8:
    cli
    push  8
    jmp default_isr_common_stub

# 9: Coprocessor Segment Overrun Exception
isr9:
    cli
    push  0
    push  9
    jmp default_isr_common_stub

# 10: Bad TSS Exception (With Error Code!)
isr10:
    cli
    push  10
    jmp default_isr_common_stub

# 11: Segment Not Present Exception (With Error Code!)
isr11:
    cli
    push  11
    jmp default_isr_common_stub

# 12: Stack Fault Exception (With Error Code!)
isr12:
    cli
    push  12
    jmp default_isr_common_stub

# 13: General Protection Fault Exception (With Error Code!)
isr13:
    cli
    push  13
    jmp default_isr_common_stub

# 14: Page Fault Exception (With Error Code!)
isr14:
    cli
    push  14
    jmp default_isr_common_stub

# 15: Reserved Exception
isr15:
    cli
    push  0
    push  15
    jmp default_isr_common_stub

# 16: Floating Point Exception
isr16:
    cli
    push  0
    push  16
    jmp default_isr_common_stub

# 17: Alignment Check Exception
isr17:
    cli
    push  0
    push  17
    jmp default_isr_common_stub

# 18: Machine Check Exception
isr18:
    cli
    push  0
    push  18
    jmp default_isr_common_stub

# 19: Reserved
isr19:
    cli
    push  0
    push  19
    jmp default_isr_common_stub

# 20: Reserved
isr20:
    cli
    push  0
    push  20
    jmp default_isr_common_stub

# 21: Reserved
isr21:
    cli
    push  0
    push  21
    jmp default_isr_common_stub

# 22: Reserved
isr22:
    cli
    push  0
    push  22
    jmp default_isr_common_stub

# 23: Reserved
isr23:
    cli
    push  0
    push  23
    jmp default_isr_common_stub

# 24: Reserved
isr24:
    cli
    push  0
    push  24
    jmp default_isr_common_stub

# 25: Reserved
isr25:
    cli
    push  0
    push  25
    jmp default_isr_common_stub

# 26: Reserved
isr26:
    cli
    push  0
    push  26
    jmp default_isr_common_stub

# 27: Reserved
isr27:
    cli
    push  0
    push  27
    jmp default_isr_common_stub

# 28: Reserved
isr28:
    cli
    push  0
    push  28
    jmp default_isr_common_stub

# 29: Reserved
isr29:
    cli
    push  0
    push  29
    jmp default_isr_common_stub

# 30: Reserved
isr30:
    cli
    push  0
    push  30
    jmp default_isr_common_stub

# 31: Reserved
isr31:
    cli
    push  0
    push  31
    jmp default_isr_common_stub

