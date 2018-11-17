#define ASM_FILE        1

.section .text
.global set_gdt
.type set_get, @function

set_gdt:
    lgdt 8(%esp)
    ret

