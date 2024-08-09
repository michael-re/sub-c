        .data
__l7:
        .asciz  "test10"
__l8:
        .asciz  "    - Expect: 4"
__l9:
        .asciz  "    - Actual: "
        .text

max:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #0
        ldr     r0, [fp, #8]
        ldr     r1, [fp, #12]
        cmp     r0, r1
        bgt     __l1
        ldr     r0, =0
        b       __l2
__l1:
        ldr     r0, =1
__l2:
        cmp     r0, #0
        beq     __l3
        ldr     r0, [fp, #8]
        mov     sp, fp
        pop     {fp, lr}
        bx      lr
__l3:
        ldr     r0, [fp, #8]
        ldr     r1, [fp, #12]
        cmp     r0, r1
        ble     __l4
        ldr     r0, =0
        b       __l5
__l4:
        ldr     r0, =1
__l5:
        cmp     r0, #0
        beq     __l6
        ldr     r0, [fp, #12]
        mov     sp, fp
        pop     {fp, lr}
        bx      lr
__l6:
        ldr     r0, =0
        ldr     r1, =999
        sub     r0, r0, r1
        mov     sp, fp
        pop     {fp, lr}
        bx      lr

main:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #12
        ldr     r0, =3
        str     r0, [fp, #-4]
        ldr     r0, =4
        str     r0, [fp, #-8]
        ldr     r0, =__l7
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l8
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =__l9
        push    {r0}
        bl      __says
        add     sp, sp, #4
        ldr     r0, [fp, #-8]
        push    {r0}
        ldr     r0, [fp, #-4]
        push    {r0}
        bl      max
        add     sp, sp, #8
        str     r0, [fp, #-12]
        ldr     r0, [fp, #-12]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, =10
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
