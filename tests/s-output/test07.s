        .data
__l1:
        .asciz  "test07"
__l2:
        .asciz  "    - Expect: 3"
__l3:
        .asciz  "    - Actual: "
        .text

main:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #8
        ldr     r0, =__l1
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l2
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l3
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, =1
        str     r0, [fp, #-4]
__l4:
        ldr     r0, [fp, #-4]
        ldr     r1, =5
        cmp     r0, r1
        blt     __l6
        ldr     r0, =0
        b       __l7
__l6:
        ldr     r0, =1
__l7:
        cmp     r0, #0
        beq     __l5
        ldr     r0, [fp, #-4]
        ldr     r1, =3
        cmp     r0, r1
        beq     __l8
        ldr     r0, =0
        b       __l9
__l8:
        ldr     r0, =1
__l9:
        str     r0, [fp, #-8]
        ldr     r0, [fp, #-8]
        ldr     r1, =1
        cmp     r0, r1
        beq     __l10
        ldr     r0, =0
        b       __l11
__l10:
        ldr     r0, =1
__l11:
        cmp     r0, #0
        beq     __l12
        ldr     r0, [fp, #-4]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
__l12:
        ldr     r0, [fp, #-4]
        ldr     r1, =1
        add     r0, r0, r1
        str     r0, [fp, #-4]
        b       __l4
__l5:
        bl      __sayl
        add     sp, sp, #0
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =7
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
