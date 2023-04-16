/** @file task.c */
#include "asm.h"
#include <boot.h>
#include <string.h>
#include <syscall.h>
#include <task.h>

void arm64_start_task(void);

/** @ingroup arm64
 * @var page_tables
 * @brief ページテーブル（レベル0）配列. タスクごとに1要素.
 */
static uint64_t page_tables[CONFIG_NUM_TASKS][512] __aligned(PAGE_SIZE);
/** @ingroup arm64
 * @var kernel_stacks
 * @brief カーネルスタック配列. タスクごとに1要素.
 */
static uint8_t kernel_stacks[CONFIG_NUM_TASKS][STACK_SIZE] __aligned(
    STACK_SIZE);
/** @ingroup arm64
 * @var exception_stacks
 * @brief 例外スタック配列. タスクごとに1要素.
 */
static uint8_t exception_stacks[CONFIG_NUM_TASKS][STACK_SIZE] __aligned(
    STACK_SIZE);

/** @ingroup arm64
 * @brief arm64_task_switch()用の初期スタックを用意する.
 * @param task タスクへのポインタ
 * @param pc プログラムエントリポイントのアドレス
 */
static void init_stack(struct task *task, vaddr_t pc) {
    // 1. ページテーブルを初期化する
    memset(task->arch.page_table, 0, PAGE_SIZE);
    task->arch.ttbr0 = ptr2paddr(task->arch.page_table);
    // 2. スタックを初期化する
    vaddr_t exception_stack = (vaddr_t) exception_stacks[task->tid];    // stack top
    uint64_t *sp = (uint64_t *) (exception_stack + STACK_SIZE);         // stack pointer
    // 3. arm64_start_task()用のスタック値を設定する: contextの構成
    *--sp = pc;         // arm64_start_task()のLRとなる: task関数が実行される

    int num_zeroed_regs = 11;  // x19-x29: ここからがcontext
    for (int i = 0; i < num_zeroed_regs; i++) {
        *--sp = 0;
    }
    *--sp = (vaddr_t) arm64_start_task;  // タスクの開始アドレス (x30).

    task->arch.stack = (vaddr_t) sp;
}

/** @ingroup arm64
 * @brief ARM64固有タスク構造体を作成する.
 * @param task タスクへのポインタ
 * @param pc プログラムエントリポイントのアドレス
 */
error_t arch_task_create(struct task *task, vaddr_t pc) {
    void *syscall_stack = (void *) kernel_stacks[task->tid];
    task->arch.page_table = page_tables[task->tid];
    task->arch.syscall_stack = (vaddr_t) syscall_stack + STACK_SIZE;
    init_stack(task, pc);
    return OK;
}

/** @ingroup arm64
 * @brief タスクを履きする. 現在は何もしない
 * @param task タスクへのポインタ
 */
void arch_task_destroy(struct task *task) {
}

/** @ingroup arm64
 * @brief ARM64固有のタスクスイッチャ. 実装はkernel/arch/arm64/switch.S
 * @param prev_sp 切り替え前のスタックポインタ
 * @param next_sp 切り替え後のスタックポインタ
 */
void arm64_task_switch(vaddr_t *prev_sp, vaddr_t next_sp);

/** @ingroup arm64
 * @brief アーキテクチャ固有のタスクスイッチャ.
 * @param prev 切り替え前のタスクへのポインタ
 * @param next 切り替え後のタスクへのポインタ
 */
void arch_task_switch(struct task *prev, struct task *next) {
    // 1. TTBR0を変更する
    ARM64_MSR(ttbr0_el1, next->arch.ttbr0);
    __asm__ __volatile__("dsb ish");
    __asm__ __volatile__("isb");
    __asm__ __volatile__("tlbi vmalle1is");
    __asm__ __volatile__("dsb ish");
    __asm__ __volatile__("isb");
    // 2. 実際の切り替え処理を行う
    arm64_task_switch(&prev->arch.stack, next->arch.stack);
}
