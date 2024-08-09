        .data
__l1:
        .asciz  "test25: v2"
__l2:
        .asciz  "    - Expect: 1 2 3 4 5"
__l3:
        .asciz  "    - Actual: "
__l4:
        .asciz  " "
__l5:
        .asciz  " "
__l6:
        .asciz  " "
__l7:
        .asciz  " "
__l8:
        .asciz  " "
__l9:
        .asciz  "test25: v3"
__l10:
        .asciz  "    - Expect: 1 2 3 4 5"
__l11:
        .asciz  "    - Actual: "
__l12:
        .asciz  " "
__l13:
        .asciz  " "
__l14:
        .asciz  " "
__l15:
        .asciz  " "
__l16:
        .asciz  " "
__l17:
        .asciz  "test25: v1"
__l18:
        .asciz  "    - Expect: 1 2 3 4 5"
__l19:
        .asciz  "    - Actual: "
__l20:
        .asciz  " "
__l21:
        .asciz  " "
__l22:
        .asciz  " "
__l23:
        .asciz  " "
__l24:
        .asciz  " "
        .text

sub:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #20
        bl      __sayl
        add     sp, sp, #0
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
        ldr     r0, [fp, #8]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l4
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #12]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l5
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #16]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l6
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #20]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l7
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #24]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l8
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, [fp, #8]
        str     r0, [fp, #-4]
        ldr     r0, [fp, #12]
        str     r0, [fp, #-8]
        ldr     r0, [fp, #16]
        str     r0, [fp, #-12]
        ldr     r0, [fp, #20]
        str     r0, [fp, #-16]
        ldr     r0, [fp, #24]
        str     r0, [fp, #-20]
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l9
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l10
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l11
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-4]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l12
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-8]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l13
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-12]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l14
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-16]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l15
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-20]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l16
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =0
        mov     sp, fp
        pop     {fp, lr}
        bx      lr

main:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #20
        ldr     r0, =1
        str     r0, [fp, #-4]
        ldr     r0, =2
        str     r0, [fp, #-8]
        ldr     r0, =3
        str     r0, [fp, #-12]
        ldr     r0, =4
        str     r0, [fp, #-16]
        ldr     r0, =5
        str     r0, [fp, #-20]
        ldr     r0, =__l17
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l18
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l19
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-4]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l20
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-8]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l21
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-12]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l22
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-16]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l23
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-20]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        ldr     r0, =__l24
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
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
        bl      sub
        add     sp, sp, #20
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =25
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
