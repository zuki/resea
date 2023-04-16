/** @file kernel/arch/arm64/init.c */
#include "asm.h"
#include <boot.h>
#include <bootinfo.h>
#include <machine/peripherals.h>
#include <printk.h>
#include <string.h>
#include <task.h>
#include <types.h>

// kernel/arch/arm64/trap.S で宣言
extern uint8_t exception_vector;

/** @ingroup arm64
 * アイドルタスク処理関数.
 */
void arch_idle(void) {
    // 1. コンテキストスイッチ
    task_switch();
    // 2. 無限ループ
    while (true) {
        // 2.1 big lockを解放
        unlock();
        // 2.2 割り込みを有効にする
        __asm__ __volatile__("msr daifclr, #2");
        // 2.3 割り込みを待機
        __asm__ __volatile__("wfi");

        // ここで割り込みがあり起床する

        // 2.4 割り込みを無効にする
        __asm__ __volatile__("msr daifset, #2");
        // 2.5 big lockを取得
        lock();
    }
}

// kernel/arch/arm64/machines/raspi3/kernel.ldで宣言
extern char __bss[];
extern char __bss_end[];

/** @ingroup arm64
 * @brief CPUを初期化する. 復帰しない.
 *
 */
__noreturn void mpinit(void) {
    while (true) {
        // 1. 割り込み(IRQ)を有効にする
        __asm__ __volatile__("msr daifset, #2");
        // 2. イベントが通知するのを待つ
        __asm__ __volatile__("wfe");
        // 3. 現在のところ、イベントが通知されて起床しても
        //    (1)に戻るだけで(3)には移行しないし、イベントも
        //    送られないようだ
    }
    // 3. CPUのmain関数を実行する（現在のところここには来ない）
    mpmain();
    // 4. mpmain()が復帰した場合は何か問題あり。panic
    PANIC("mpmain returned");
    for (;;) {
        halt();
    }
}

/** @ingroup arm64
 * @var bootinfo
 * @brief ブート情報
 */
static struct bootinfo bootinfo;

/** @ingroup arm64
 * @brief boot.Sから呼び出される最初のC関数.
 *
 */
void arm64_init(void) {
    // TODO: 未使用の例外テーブルベクタを無効にする
    // TODO: smp
    // TODO: kdebug
    // TODO: tlbフラッシュの改善

    // 1. 例外ベクタの登録
    ARM64_MSR(vbar_el1, &exception_vector);
    // 2. cpu1-3 (ap)の初期化
    if (!mp_is_bsp()) {
        lock();
        mpinit();
        UNREACHABLE();
    }

    // 以降はcpu0 (bsp) だけが十個する

    // 3. 以後、下位アドレスにアクセス必要はない（FIXME: ユーザ空間は?)
    ARM64_MSR(ttbr0_el1, 0ull);

    // 4. CPUローカル変数、.bss領域を0クリア
    bzero(get_cpuvar(), sizeof(struct cpuvar));
    bzero(__bss, (vaddr_t) __bss_end - (vaddr_t) __bss);
    // 5. ペリフェラル (uart, timer) を初期化する
    arm64_peripherals_init();
    // 6. ロック取得
    lock();

    // 7. d-cache と i-cache を有効にする.
    ARM64_MSR(sctlr_el1, ARM64_MRS(sctlr_el1) | (1 << 2) | (1 << 12));

    // 8. ベンチマーク用のパフォーマンスカウンタを初期化する
    ARM64_MSR(pmcr_el0, 0b1ull);   // カウンタをリセットする
    ARM64_MSR(pmcr_el0, 0b11ull);  // カウンタを有効にする

    // 9. QEMUはパフォーマンスカウンタをサポートしていない。
    //    Raspberry Pi 3B+実機はサポートしているので設定する。
#ifndef CONFIG_SEMIHOSTING
    int num_perf_counters = (ARM64_MRS(pmcr_el0) >> 11) & 0b11111;
    ASSERT(num_perf_counters >= 5);

    uint32_t pmceid = ARM64_MRS(pmceid0_el0);
    ASSERT((pmceid & (1 << 4)) != 0 && "L1D_CACHE event is not supported");
    ASSERT((pmceid & (1 << 22)) != 0 && "L2D_CACHE event is not supported");
    ASSERT((pmceid & (1 << 19)) != 0 && "MEM_ACCESS event is not supported");
    ASSERT((pmceid & (1 << 9)) != 0 && "EXC_TAKEN event is not supported");
    ASSERT((pmceid & (1 << 5)) != 0 && "L1D_TLB_REFILL event is not supported");
    ARM64_MSR(pmevtyper0_el0, 0x04ull);
    ARM64_MSR(pmevtyper1_el0, 0x16ull);
    ARM64_MSR(pmevtyper2_el0, 0x13ull);
    ARM64_MSR(pmevtyper3_el0, 0x0aull);
    ARM64_MSR(pmevtyper4_el0, 0x05ull);
#endif

    ARM64_MSR(pmcntenset_el0,
              0x8000001full);  // Enable the cycle and 5 event counters.
    ARM64_MSR(pmuserenr_el0, 0b11ull);  // Enable user access to the counters.

    // 10: FIXME: machine固有の設定（カーネル直後に128MiBのメモリ）
    bootinfo.memmap[0].base = (vaddr_t) __kernel_image_end;
    bootinfo.memmap[0].len = 128 * 1024 * 1024;  // 128MiB
    bootinfo.memmap[0].type = BOOTINFO_MEMMAP_TYPE_AVAILABLE;
    // 11. kmain()を呼び出す
    kmain(&bootinfo);

    PANIC("kmain returned");
    for (;;) {
        halt();
    }
}

void arch_semihosting_halt(void) {
    // ARM Semihosting
    uint64_t params[] = {
        0x20026,  // application exit
        0,        // exit code
    };

    register uint64_t x0 __asm__("x0") = 0x20;  // exit
    register uint64_t x1 __asm__("x1") = (uint64_t) params;
    __asm__ __volatile__("hlt #0xf000" ::"r"(x0), "r"(x1));
}
