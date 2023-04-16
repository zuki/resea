//
//  例外ベクタ
//
.balign 2048
.global exception_vector
exception_vector:
    b exception_common
    .balign 128
    b interrupt_common
    .balign 128
    b interrupt_common
    .balign 128
    b exception_common
    .balign 128

    b exception_common
    .balign 128
    b interrupt_common
    .balign 128
    b interrupt_common
    .balign 128
    b exception_common
    .balign 128

    b exception_common
    .balign 128
    b interrupt_common
    .balign 128
    b interrupt_common
    .balign 128
    b exception_common
    .balign 128

    b exception_common
    .balign 128
    b interrupt_common
    .balign 128
    b interrupt_common
    .balign 128
    b exception_common
    .balign 128

/// x0以外の例外コンテキストをスタックに保存する
.macro save_context
    stp  x1,  x2,  [sp, #-16]!
    stp  x3,  x4,  [sp, #-16]!
    stp  x5,  x6,  [sp, #-16]!
    stp  x7,  x8,  [sp, #-16]!
    stp  x9,  x10, [sp, #-16]!
    stp  x11, x12, [sp, #-16]!
    stp  x13, x14, [sp, #-16]!
    stp  x15, x16, [sp, #-16]!
    stp  x17, x18, [sp, #-16]!
    stp  x19, x20, [sp, #-16]!
    stp  x21, x22, [sp, #-16]!
    stp  x23, x24, [sp, #-16]!
    stp  x25, x26, [sp, #-16]!
    stp  x27, x28, [sp, #-16]!
    stp  x29, x30, [sp, #-16]!

    // システムコール処理中にユーザコピー関数でページフォルトが
    // 発生する可能性があるため（入れ子の例外となる）、例外
    // コンテキストをスタックに保存する
    mrs  x9, spsr_el1
    mrs  x10, elr_el1
    mrs  x11, sp_el0
    stp  x9,  x10, [sp, #-16]!
    stp  x11, x12, [sp, #-16]!
.endm

/// x0以外の例外コンテキストを復元する
.macro restore_context
    ldp  x11, x12, [sp], #16
    ldp  x9,  x10, [sp], #16
    msr  spsr_el1, x9
    msr  elr_el1, x10
    msr  sp_el0, x11

    ldp  x29, x30, [sp], #16
    ldp  x27, x28, [sp], #16
    ldp  x25, x26, [sp], #16
    ldp  x23, x24, [sp], #16
    ldp  x21, x22, [sp], #16
    ldp  x19, x20, [sp], #16
    ldp  x17, x18, [sp], #16
    ldp  x15, x16, [sp], #16
    ldp  x13, x14, [sp], #16
    ldp  x11, x12, [sp], #16
    ldp  x9,  x10, [sp], #16
    ldp  x7,  x8,  [sp], #16
    ldp  x5,  x6,  [sp], #16
    ldp  x3,  x4,  [sp], #16
    ldp  x1,  x2,  [sp], #16
    eret
.endm

// 共通割り込みハンドラ
interrupt_common:
    save_context

    // arm固有ハンドラの前後でx0を保存/復元
    stp  x0, xzr, [sp, #-16]!
    bl   arm64_handle_interrupt
    ldp  x0, xzr, [sp], #16

    restore_context

// 共通例外ハンドラ
exception_common:
    save_context

    // 例外がシステムコールか確認する
    mrs  x9, esr_el1
    lsr  x9, x9, #26
    cmp  x9, 0x15
    bne  1f         // 違う -> 1f

    // SVCの処理。x0は返り値が設定されるので保存しない
    bl   handle_syscall
    b    2f
1:
    // Exceptions except SVC.
    stp  x0, xzr, [sp, #-16]!
    bl   arm64_handle_exception
    ldp  x0, xzr, [sp], #16
2:
    restore_context
