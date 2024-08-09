        .data
__l16:
        .asciz  "test17"
__l17:
        .asciz  "    - Expect: 1454"
__l18:
        .asciz  "    - Actual: "
        .text

days_in_month:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #0
        ldr     r0, [fp, #8]
        ldr     r1, =2
        cmp     r0, r1
        beq     __l1
        ldr     r0, =0
        b       __l2
__l1:
        ldr     r0, =1
__l2:
        cmp     r0, #0
        beq     __l3
        ldr     r0, =28
        mov     sp, fp
        pop     {fp, lr}
        bx      lr
__l3:
        ldr     r0, [fp, #8]
        ldr     r1, =4
        cmp     r0, r1
        beq     __l4
        ldr     r0, =0
        b       __l5
__l4:
        ldr     r0, =1
__l5:
        cmp     r0, #0
        beq     __l6
        ldr     r0, =30
        mov     sp, fp
        pop     {fp, lr}
        bx      lr
__l6:
        ldr     r0, [fp, #8]
        ldr     r1, =6
        cmp     r0, r1
        beq     __l7
        ldr     r0, =0
        b       __l8
__l7:
        ldr     r0, =1
__l8:
        cmp     r0, #0
        beq     __l9
        ldr     r0, =30
        mov     sp, fp
        pop     {fp, lr}
        bx      lr
__l9:
        ldr     r0, [fp, #8]
        ldr     r1, =9
        cmp     r0, r1
        beq     __l10
        ldr     r0, =0
        b       __l11
__l10:
        ldr     r0, =1
__l11:
        cmp     r0, #0
        beq     __l12
        ldr     r0, =30
        mov     sp, fp
        pop     {fp, lr}
        bx      lr
__l12:
        ldr     r0, [fp, #8]
        ldr     r1, =11
        cmp     r0, r1
        beq     __l13
        ldr     r0, =0
        b       __l14
__l13:
        ldr     r0, =1
__l14:
        cmp     r0, #0
        beq     __l15
        ldr     r0, =30
        mov     sp, fp
        pop     {fp, lr}
        bx      lr
__l15:
        ldr     r0, =31
        mov     sp, fp
        pop     {fp, lr}
        bx      lr

date_diff:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #24
        ldr     r0, =365
        ldr     r1, [fp, #8]
        mul     r0, r0, r1
        str     r0, [fp, #-4]
        ldr     r0, [fp, #12]
        push    {r0}
        bl      days_in_month
        add     sp, sp, #4
        str     r0, [fp, #-8]
        ldr     r0, [fp, #-4]
        ldr     r1, [fp, #-8]
        add     r0, r0, r1
        str     r0, [fp, #-12]
        ldr     r0, [fp, #-12]
        ldr     r1, [fp, #16]
        add     r0, r0, r1
        str     r0, [fp, #-12]
        ldr     r0, =365
        ldr     r1, [fp, #20]
        mul     r0, r0, r1
        str     r0, [fp, #-16]
        ldr     r0, [fp, #24]
        push    {r0}
        bl      days_in_month
        add     sp, sp, #4
        str     r0, [fp, #-20]
        ldr     r0, [fp, #-16]
        ldr     r1, [fp, #-20]
        add     r0, r0, r1
        str     r0, [fp, #-24]
        ldr     r0, [fp, #-24]
        ldr     r1, [fp, #28]
        add     r0, r0, r1
        str     r0, [fp, #-24]
        ldr     r0, [fp, #-12]
        ldr     r1, [fp, #-24]
        sub     r0, r0, r1
        mov     sp, fp
        pop     {fp, lr}
        bx      lr

main:
        push    {fp, lr}
        mov     fp, sp
        sub     sp, sp, #24
        ldr     r0, =51
        str     r0, [fp, #-4]
        ldr     r0, =7
        str     r0, [fp, #-8]
        ldr     r0, =4
        str     r0, [fp, #-12]
        ldr     r0, =__l16
        push    {r0}
        bl      __says
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
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
        mov     r0, #13
        push    {r0}
        ldr     r0, [fp, #-12]
        push    {r0}
        mov     r0, #47
        push    {r0}
        ldr     r0, [fp, #-8]
        push    {r0}
        mov     r0, #6
        push    {r0}
        ldr     r0, [fp, #-4]
        push    {r0}
        bl      date_diff
        add     sp, sp, #24
        str     r0, [fp, #-16]
        ldr     r0, [fp, #-16]
        push    {r0}
        bl      __sayn
        add     sp, sp, #4
        bl      __sayl
        add     sp, sp, #0
        bl      __sayl
        add     sp, sp, #0
        ldr     r0, [fp, #-16]
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
