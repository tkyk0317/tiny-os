.section ".vector"
.global vector

.macro kernel_entry el
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
    stp   x20, x21, [sp, #-16]!
    stp   x22, x23, [sp, #-16]!
    stp   x24, x25, [sp, #-16]!
    stp   x26, x27, [sp, #-16]!
    stp   x28, x29, [sp, #-16]!
    stp   x30, xzr, [sp, #-16]!
    mrs	  x22, elr_el1
    mrs	  x23, spsr_el1
    stp   x22, x23, [sp, #-16]!

    .if \el == 0
        mrs x22, sp_el0
        stp x22, xzr, [sp, #-16]!
    .endif
.endm

.macro kernel_exit el
    .if \el == 0
        ldp x22, x23, [sp], #16
        msr sp_el0, x22
    .endif

    ldp   x22, x23, [sp], #16
    msr   elr_el1, x22
    msr   spsr_el1, x23
    ldp   x30, xzr, [sp], #16
    ldp   x28, x29, [sp], #16
    ldp   x26, x27, [sp], #16
    ldp   x24, x25, [sp], #16
    ldp   x22, x23, [sp], #16
    ldp   x20, x21, [sp], #16
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
.endm

// Interrupt Handler for EL1-IRQ
el1_irq:
    kernel_entry 1
    bl    __irq_handler
    kernel_exit 1

// Interrupt Handler for EL0-IRQ
el0_irq:
    kernel_entry 0
    bl    __irq_handler
    kernel_exit 0

// system call
el0_svc:
    kernel_entry 0

    // システムコール発行
    adr	x27, sys_call_tbl // システムコールテーブルを読み込む
    uxtw w26, w8          // システムコール番号を読み込む
    ldr	x16, [x27, x26, lsl #3]
    blr	x16

    bl disable_irq
    kernel_exit 0

.balign 2048
vector:
.balign 128
    // Sync Interrupt
    b hang
.balign 128
    // EL1t IRQ
    b hang
.balign 128
    // FIQ Interrupt
    b hang
.balign 128
    // SError Interrupt
    b hang
.balign 128
    // Sync Interrupt
    b hang
.balign 128
    // EL1h IRQ
    b el1_irq
.balign 128
    // FIQ Interrupt
    b hang
.balign 128
    // SError Interrupt
    b hang
.balign 128
    // Sync Interrupt
    b el0_svc
.balign 128
    // EL0t IRQ
    b el0_irq
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
    bl show_registers
loop:
    wfi
    b loop

.global enable_irq
enable_irq:
    msr   daifclr, #2
    ret

.global disable_irq
disable_irq:
    msr   daifset, #2
    ret

.global __switch_el0
__switch_el0:
    bl    disable_irq
    msr   elr_el1, x0  // ジャンプ先を例外リンクレジスタへ
    mov   x30, x0
    msr   sp_el0, x1   // EL0スタックポインタを設定
    mov   x3, #0       // EL0t
    msr   spsr_el1, x3 // CPUステータスを設定
    mov   x0, x2
    eret

