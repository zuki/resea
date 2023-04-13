.section ".boot", "ax"
.global boot
boot:
    // boot: i/Dキャッシュが無効な点がxv6と異なる、それ以外は同じ
    ldr  x0, =0x30d00980 // リトルエンディアン, MMU無効, I/D-キャッシュ無効, RES1
    msr  sctlr_el1, x0
    ldr  x0, =0x80000000 // EL1はAArch64.
    msr  hcr_el2, x0
    ldr  x0, =0x000001c5 // EL1hに移行(SP_EL1を使用), exceptions (AIF)をマスク.
    msr  spsr_el2, x0

    adr  x0, el1_mode   // LRにel1_modeを設定することでeretでel1_modeに戻る
    msr  elr_el2, x0
    eret

el1_mode:
    // Read the CPU ID.
    mrs  x1, mpidr_el1
    and  x1, x1, #3

    // 各CPUのスタックポインタを設定（spのサイズは0x1000）
    ldr  x2, =__boot_stack_base
    ldr  x3, =0x1000
    add  x2, x2, #0x1000
    madd x4, x3, x1, x2 // x4 = x3 * x1 + x2 = PAGE_SIZE * cpu_id + x2
    mov  sp, x4
    cbz x1, bsp_boot

ap_boot:
    b enable_mmu

bsp_boot:
    // カーネルページテーブルの構築.
    //
    //   offsets         |    Table
    //  -------------------------------
    //   0x0000-0x1000   |   Level-0
    //   0x1000-0x2000   |   Level-1
    //   0x2000-0x3000   |   Level-2 (0x0000_0000-0x3eff_ffff)
    //   0x3000-0x4000   |   Level-2 (0x3f00_0000-0x4100_0000)
    //
    //   Note: Don't forget to update __kernl_page_table as well to increase the
    //         size of the page table!
    //
    adrp x4, __kernel_page_table

    // レベル-0のテーブル（1要素）
    mov  x0, x4
    add  x1, x4, #0x00001000 // レベル1テーブルのベースアドレス.
    ldr  x5, =0x00000403     // テーブル記述子 (AF=1).
    orr  x1, x1, x5
    str  x1, [x0]

    // レベル-1のテーブル (2要素)
    add  x0, x4, #0x1000
    add  x1, x4, #0x2000     // レベル2テーブルのベースアドレス
    ldr  x5, =0x00000b03     // テーブル記述子 (nG=1, SH=11).
    orr  x1, x1, x5
    str  x1, [x0]

    add  x0, x0, #8
    add  x1, x4, #0x3000     // レベル2テーブルのベースアドレス.
    ldr  x5, =0x00000b03     // テーブル記述子 (nG=1, SH=11).
    orr  x1, x1, x5
    str  x1, [x0]

    // レベル-2のテーブル: 0x0000_0000-0x3eff_ffff (1008MiB)
    add  x0, x4, #0x2000
    ldr  x1, =0x00000401    // ブロック記述子 (AF=1).
    ldr  x2, =504           // = 1008 (MiB) / 2 (MiB): 1ブロック2MiB
1:
    str  x1, [x0], 0x8
    add  x1, x1, #0x200000  // 2MB
    sub  x2, x2, 1
    cbnz x2, 1b

    // Level-2 table: 0x3f00_0000-0x4100_0000 (32MiB): デバイスアドレス
    ldr  x5, =0xffffffe3        // AttrIndxをクリア (the index in MAIR_EL1).
    and  x1, x1, x5
    orr  x1, x1, #0x00000004    // AttrIndx = 1 にセット.
    ldr  x2, =16
2:
    str  x1, [x0], 0x8
    add  x1, x1, #0x200000
    sub  x2, x2, 1
    cbnz x2, 2b

enable_mmu:
    // Configure paging.
    ldr  x0, =0x000000ff // AttrIndx == 0: Normal memory, cache enabled
                         // AttrIndx == 1: Device-nGnRnE memory
    msr  mair_el1, x0
                            // 4KB指定が TG1=0b10, TG0=0b00 と異なる
    ldr  x0, =0xb5103510    // TTBR0/1 共に4KiB ページ、リージョンサイズは 2^48.
    msr  tcr_el1, x0

    adrp x0, __kernel_page_table
    msr  ttbr1_el1, x0
    msr  ttbr0_el1, x0

    adr  x0, mmu_enabled
    ldr  x1, =0xffff000000000000
    orr  x1, x0, x1
    // 0011 0000 1101 0000 0000 1000 0000 0001
    ldr  x0, =0x30d00801 // Lttile Endian, MMU enabled, Don't disable I/D-cache
    dsb ish
    isb
    msr  sctlr_el1, x0
    isb
    br   x1

mmu_enabled:
    // Update SP to use the upper address.
    mov  x0, sp
    ldr  x1, =0xffff000000000000
    orr  x0, x0, x1
    mov  sp, x0

    ldr  x0, =arm64_init
    br   x0

halt:
    wfi
    b  halt

.rodata
.balign 4096
.global __bootelf
__bootelf:
    .incbin BOOTELF_PATH
.balign 4096
