        .data
__l1:
        .asciz  " "
__l2:
        .asciz  " "
__l3:
        .asciz  " "
__l4:
        .asciz  " "
__l5:
        .asciz  "test22"
__l6:
        .asciz  "    - Expect: 1 2 3 4"
__l7:
        .asciz  "    - Actual: "
        .text

sub:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #0
        ldr     r0, [fp, #8]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l1
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #12]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l2
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #16]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l3
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #20]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l4
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, =0
        mov     sp, fp
        pop     {fp, lr}
        bx      lr

main:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #0
        ldr     r0, =__l5
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l6
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l7
        push    {r0}
        bl      __says
        add     sp, sp, #4
        mov     r0, #4
        push    {r0}
        mov     r0, #3
        push    {r0}
        mov     r0, #2
        push    {r0}
        mov     r0, #1
        push    {r0}
        bl      sub
        add     sp, sp, #16
        bl      __sayl
        add     sp, sp, #0
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =22
        mov     sp, fp
        pop     {fp, lr}
        bx      lr

@ ===================================================================
@ subc intrinsics
@ ===================================================================
        .data
        .align  4

__l0:
        .asciz  "\r\n"
        .equ    __stdout,    0x01
        .equ    __swi_pstr,  0x69
        .equ    __swi_pint,  0x6b

        .global __says, __sayn, __sayl
        .text
        .align  4

@ ===================================================================
@ intrinsic: int __says(const char* __x__)
@       prints the given null terminated string to stdout. returns 0.
@ ===================================================================
__says:
        push    {r1, lr}
        mov     r1, r0
        ldr     r0, =__stdout
        swi     __swi_pstr
        mov     r0, #0
        pop     {r1, lr}
        bx      lr

@ ===================================================================
@ intrinsic: int __sayn(int __x__)
@       prints the given number to stdout. returns 0.
@ ===================================================================
__sayn:
        push    {r1, lr}
        mov     r1, r0
        ldr     r0, =__stdout
        swi     __swi_pint
        mov     r0, #0
        pop     {r1, lr}
        bx      lr

@ ===================================================================
@ intrinsic: int __sayl(void)
@       prints an empty line to stdout. returns 0.
@ ===================================================================
__sayl:
        push    {lr}
        mov     r1, r0
        ldr     r0, =__l0
        bl      __says
        mov     r0, #0
        pop     {lr}
        bx      lr
