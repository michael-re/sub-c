        .data
__l1:
        .asciz  "test16"
__l2:
        .asciz  "    - Expect: 37"
__l3:
        .asciz  "    - Actual: "
        .text

sub1:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #4
        ldr     r0, [fp, #8]
        ldr     r1, [fp, #12]
        add     r0, r0, r1
        str     r0, [fp, #-4]
        ldr     r0, [fp, #-4]
        ldr     r1, [fp, #16]
        sub     r0, r0, r1
        str     r0, [fp, #-4]
        ldr     r0, [fp, #-4]
        ldr     r1, [fp, #20]
        sub     r0, r0, r1
        str     r0, [fp, #-4]
        ldr     r0, [fp, #-4]
        ldr     r1, [fp, #24]
        add     r0, r0, r1
        str     r0, [fp, #-4]
        ldr     r0, [fp, #-4]
        ldr     r1, [fp, #28]
        add     r0, r0, r1
        str     r0, [fp, #-4]
        ldr     r0, [fp, #-4]
        mov     sp, fp
        pop     {fp, lr}
        bx      lr

sub2:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #4
        ldr     r0, [fp, #8]
        push    {r0}
        ldr     r0, [fp, #12]
        push    {r0}
        ldr     r0, [fp, #16]
        push    {r0}
        ldr     r0, [fp, #20]
        push    {r0}
        ldr     r0, [fp, #24]
        push    {r0}
        ldr     r0, [fp, #28]
        push    {r0}
        bl      sub1
        add     sp, sp, #24
        str     r0, [fp, #-4]
        ldr     r0, [fp, #-4]
        mov     sp, fp
        pop     {fp, lr}
        bx      lr

main:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #28
        ldr     r0, =16
        str     r0, [fp, #-4]
        ldr     r0, =17
        str     r0, [fp, #-8]
        ldr     r0, =18
        str     r0, [fp, #-12]
        ldr     r0, =19
        str     r0, [fp, #-16]
        ldr     r0, =20
        str     r0, [fp, #-20]
        ldr     r0, =21
        str     r0, [fp, #-24]
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
        ldr     r0, [fp, #-24]
        push    {r0}
        ldr     r0, [fp, #-20]
        push    {r0}
        ldr     r0, [fp, #-16]
        push    {r0}
        ldr     r0, [fp, #-12]
        push    {r0}
        ldr     r0, [fp, #-8]
        push    {r0}
        ldr     r0, [fp, #-4]
        push    {r0}
        bl      sub2
        add     sp, sp, #24
        str     r0, [fp, #-28]
        ldr     r0, [fp, #-28]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =16
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
