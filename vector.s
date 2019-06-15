.section ".vector"
.global vector

.balign 2048
vector:
.balign 128
    b hang
.balign 128
    stp   x0,  x1,  [sp, #-16]!
    stp   x2,  x3,  [sp, #-16]!
    stp   x4,  x5,  [sp, #-16]!
    stp   x6,  x7,  [sp, #-16]!
    stp   x8,  x9,  [sp, #-16]!
    stp   x10, x11, [sp, #-16]!
    stp   x12, x13, [sp, #-16]!
    stp   x14, x15, [sp, #-16]!
    stp   x16, x17, [sp, #-16]!
    stp   x18, x19, [sp, #-16]!

    bl    __irq_handler

    ldp   x18, x19, [sp], #16
    ldp   x16, x17, [sp], #16
    ldp   x14, x15, [sp], #16
    ldp   x12, x13, [sp], #16
    ldp   x10, x11, [sp], #16
    ldp   x8,  x9,  [sp], #16
    ldp   x6,  x7,  [sp], #16
    ldp   x4,  x5,  [sp], #16
    ldp   x2,  x3,  [sp], #16
    ldp   x0,  x1,  [sp], #16
    eret
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang
.balign 128
    b hang

.global hang
hang:
    wfi
    b hang

.global enable_irq
enable_irq:
    msr   daifclr, #2
    ret

.global disable_irq
disable_irq:
    msr   daifset, #2
    ret
