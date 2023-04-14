/** @file kernel/task.h */
#ifndef __TASK_H__
#define __TASK_H__

#include <arch.h>
#include <bitmap.h>
#include <config.h>
#include <list.h>
#include <message.h>
#include <types.h>

/** @ingroup task
 * @def TASK_TIME_SLICE
 * @brief コンテキストスイッチを行うタイムスライス(tick数).
 */
#define TASK_TIME_SLICE ((CONFIG_TASK_TIME_SLICE_MS * TICK_HZ) / 1000)
STATIC_ASSERT(TASK_TIME_SLICE > 0);

/** @ingroup task
 * @def TASK_UNUSED
 * @brief タスクステータス: 未使用
 */
#define TASK_UNUSED 0
/** @ingroup task
 * @def TASK_RUNNABLE
 * @brief タスクステータス: 実行中、またはランキューにある
 */
#define TASK_RUNNABLE 1
/** @ingroup task
 * @def TASK_BLOCKED
 * @brief タスクステータス: IPCで送受信タスクを待機中
 */
#define TASK_BLOCKED 2

/** @ingroup task
 * @def TASK_PRIORITY_MAX
 * @brief 最大優先度
 */
#define TASK_PRIORITY_MAX 8
STATIC_ASSERT(TASK_PRIORITY_MAX > 0);

/** @ingroup task
 * @def ARCH_CPUVAR
 * @brief arch_cpuvar構造体
 */
#define ARCH_CPUVAR (&get_cpuvar()->arch)

/** @ingroup task
 * @def CURRENT
 * @brief カレントCPUのカレントタスク (`struct task *`)
 */
#define CURRENT (get_cpuvar()->current_task)

/** @ingroup task
 * @def IDLE_TASK
 * @brief カレントCPUのアイドルタスク (`struct task *`)
 */
#define IDLE_TASK (&get_cpuvar()->idle_task)

/** @ingroup task
 * @def CAP_TASK
 * @brief タスクケーパビリティ: TASK
 */
#define CAP_TASK   0
/** @ingroup task
 * @def CAP_IRQ
 * @brief タスクケーパビリティ: IRQ
 */
#define CAP_IRQ    1
/** @ingroup task
 * @def CAP_IO
 * @brief タスクケーパビリティ: IO
 */
#define CAP_IO     2
/** @ingroup task
 * @def CAP_MAP
 * @brief タスクケーパビリティ: MAP
 */
#define CAP_MAP    3
/** @ingroup task
 * @def CAP_KDEBUG
 * @brief タスクケーパビリティ: KDEBUG
 */
#define CAP_KDEBUG 4
/** @ingroup task
 * @def CAP_MAX
 * @brief 最大タスクケーパビリティ
 */
#define CAP_MAX    4

/** @ingroup task
 * @def CAPABLE
 * @brief taskはcapを実行できるか（関数）
 */
#define CAPABLE(task, cap)                                                     \
    (bitmap_get((task)->caps, sizeof((task)->caps), cap) != 0)

/** @ingroup task
 * @struct task
 * @brief タスク構造体（いわゆる、タスク制御ブロック）.
 */
struct task {
    struct arch_task arch;  /**< arch固有のフィールド */
    task_t tid;             /**< タスクID （1 から始まる） */
    int state;              /**< 状態 */
    char name[CONFIG_TASK_NAME_LEN];    /**< タスク名（NULL終端） */
    unsigned flags;         /**< フラグ */
    unsigned ref_count;     /**< このタスクを参照しているタスクの数 */
    struct task *pager;     /**< ページャタスク. ページフォルトや例外（ゼロ除算など）が発生するとカーネルはこのページャにメッセージを送信して、フォルトを解決させる（または、タスクをkillする） */
    int quantum;            /**< タイムスライスの残り（ティック単位）. この値が0になったら、カーネルは次のタスクに切り替える（いわゆるプリエンプティブコンテキストスイッチ） */
    int priority;           /**< タスク優先度. 小さい値ほど高い優先でを意味する。スケジューラは常に最高の優先度を持つrunnableなタスクを選択する。最高の優先度を持つrunnableタスクが複数ある場合、カーネルはラウンドロビン方式でスケジュールする。 */
    struct message m;       /**< メッセージバッファ */
    task_t src;             /**< 受け入れ可能な送信元のタスクID. IPC_ANYの場合、タスクはすべてのタスクからのメッセージを受け入れる。*/
    notifications_t notifications;  /**< 保留中の通知. タスクがそれをメッセージ（NOTEIFACTIONS_MSG）として受信するとクリアされる。 */
    msec_t timeout;         /**< IPCタイムアウト（ミリ秒単位）. 0になるとカーネルはタスクに`NOTIFY_TIMER`を通知する。 */
    list_t senders;         /**< このタスクのメッセージが受信可能になるのを待機しているタスクのキュー. このタスクが準備できた場合、このキューにあるすべてのスレッドを再開させる。 */
    list_elem_t runqueue_next;  /**< runキューの（intrusiveな）リスト要素 */
    list_elem_t sender_next;    /**< senderキューの（intrusiveな）リスト要素 */
    uint8_t caps[BITMAP_SIZE(CAP_MAX)]; /**< ケーパビリティ（ビットマップ） */
};

/** @ingroup task
 * @struct cpuvar
 * @brief CPUローカルな変数.
 */
struct cpuvar {
    struct arch_cpuvar arch;        /**< arch_cpuvar構造体 */
    struct task *current_task;      /**< 現在実行中のタスクへのポインタ */
    struct task idle_task;          /**< アイドルタスク */
};

__mustuse error_t task_create(struct task *task, const char *name, vaddr_t ip,
                              struct task *pager, unsigned flags);
__mustuse error_t task_destroy(struct task *task);
__noreturn void task_exit(enum exception_type exp);
void task_block(struct task *task);
void task_resume(struct task *task);
error_t task_schedule(struct task *task, int priority);
struct task *task_lookup(task_t tid);
struct task *task_lookup_unchecked(task_t tid);
void task_switch(void);
__mustuse error_t vm_map(struct task *task, vaddr_t vaddr, paddr_t paddr,
                         paddr_t kpage, unsigned flags);
__mustuse error_t vm_unmap(struct task *task, vaddr_t vaddr);
__mustuse error_t task_listen_irq(struct task *task, unsigned irq);
__mustuse error_t task_unlisten_irq(unsigned irq);
void handle_timer_irq(void);
void handle_irq(unsigned irq);
void handle_page_fault(vaddr_t addr, vaddr_t ip, unsigned fault);
void task_dump(void);
void task_init(void);

// アーキテクチャ固有の実装

/// kernel/arch/<arch>/mp.c で実装
void lock(void);
void panic_lock(void);
void unlock(void);
void mp_start(void);
int mp_self(void);
int mp_num_cpus(void);
void mp_reschedule(void);
/// kernel/arch/<arch>/task.c で実装
__mustuse error_t arch_task_create(struct task *task, vaddr_t ip);
void arch_task_destroy(struct task *task);
void arch_task_switch(struct task *prev, struct task *next);
/// kernel/arch/x86/interrupt.c, kernel/arch/arm64/macies/raspi3/peripherals.c で実装
void arch_enable_irq(unsigned irq);
void arch_disable_irq(unsigned irq);
/// kernel/arch/<arch>/vm.c で実装
__mustuse error_t arch_vm_map(struct task *task, vaddr_t vaddr, paddr_t paddr,
                              paddr_t kpage, unsigned flags);
__mustuse error_t arch_vm_unmap(struct task *task, vaddr_t vaddr);
paddr_t vm_resolve(struct task *task, vaddr_t vaddr);

#endif
