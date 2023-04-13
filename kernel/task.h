#ifndef __TASK_H__
#define __TASK_H__

#include <arch.h>
#include <bitmap.h>
#include <config.h>
#include <list.h>
#include <message.h>
#include <types.h>

/// The context switching time slice (# of ticks).
#define TASK_TIME_SLICE ((CONFIG_TASK_TIME_SLICE_MS * TICK_HZ) / 1000)
STATIC_ASSERT(TASK_TIME_SLICE > 0);

//
// Task states.
//

/// The task struct is not being used.
#define TASK_UNUSED 0
/// The task is running or is queued in the runqueue.
#define TASK_RUNNABLE 1
/// The task is waiting for a receiver/sender task in IPC.
#define TASK_BLOCKED 2

#define TASK_PRIORITY_MAX 8
STATIC_ASSERT(TASK_PRIORITY_MAX > 0);

// struct arch_cpuvar *
#define ARCH_CPUVAR (&get_cpuvar()->arch)

/// The current task of the current CPU (`struct task *`).
#define CURRENT (get_cpuvar()->current_task)
/// The idle task of the current CPU (`struct task *`).
#define IDLE_TASK (&get_cpuvar()->idle_task)

// Task capabilities.
#define CAP_TASK   0
#define CAP_IRQ    1
#define CAP_IO     2
#define CAP_MAP    3
#define CAP_KDEBUG 4
#define CAP_MAX    4

#define CAPABLE(task, cap)                                                     \
    (bitmap_get((task)->caps, sizeof((task)->caps), cap) != 0)

/// The task struct (so-called Task Control Block).
struct task {
    /// arch固有のフィールド
    struct arch_task arch;
    /// タスクID （1 から始まる）
    task_t tid;
    /// 状態
    int state;
    /// タスク名（NULL終端）
    char name[CONFIG_TASK_NAME_LEN];
    /// フラグ
    unsigned flags;
    /// このタスクを参照しているタスクの数
    unsigned ref_count;
    /// ページャタスク。ページフォルトや例外（ゼロ除算など）が発生すると
    /// カーネルはこのページ兄メッセージを送信して、フォルトを解決させる
    /// （または、タスクをkillする）
    struct task *pager;
    /// タイムスライスの残り（ティック単位）。この値が0になったら、カーネルはT
    /// 次のタスクに切り替える（いわゆるプリエンプティブコンテキストスイッチ）
    int quantum;
    /// タスク優先度。小さい値ほど高い優先でを意味する。スケジューラは常に最高の
    /// 優先度を持つrunnableなタスクを選択する。最高の優先度を持つrunnableタスクが
    /// 複数ある場合、カーネルはラウンドロビン方式でスケジュールする。
    int priority;
    /// メッセージバッファ
    struct message m;
    /// 受け入れ可能な送り手ID。IPC_ANYの場合はアスクはすべてのタスクからの
    /// メッセージを受け入れる。
    task_t src;
    /// 保留中の通知。タスクがそれをメッセージ（NOTEIFACTIONS_MSG）として
    /// 受信するとクリアされる。
    notifications_t notifications;
    /// IPCタイムアウト（ミリ秒単位）。0になるとカーネルはタスクに`NOTIFY_TIMER`を
    /// 通知する。
    msec_t timeout;
    /// このタスクのメッセージを受信できるようになるのを待っているタスクのキュー。
    /// このタスクが準備できた場合、このキューにあるすべてのスレッドを再開させる。
    list_t senders;
    /// runキューの（intrusiveな）リスト要素。
    list_elem_t runqueue_next;
    /// senderキューの（intrusiveな）リスト要素。
    list_elem_t sender_next;
    /// ケーパビリティ（ビットマップ）
    uint8_t caps[BITMAP_SIZE(CAP_MAX)];
};

/// CPU-local variables.
struct cpuvar {
    struct arch_cpuvar arch;
    struct task *current_task;
    struct task idle_task;
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

// Implemented in arch.
/// in kernel/arch/<arch>/mp.c
void lock(void);
void panic_lock(void);
void unlock(void);
void mp_start(void);
int mp_self(void);
int mp_num_cpus(void);
void mp_reschedule(void);
/// in kernel/arch/<arch>/task.c
__mustuse error_t arch_task_create(struct task *task, vaddr_t ip);
void arch_task_destroy(struct task *task);
void arch_task_switch(struct task *prev, struct task *next);
/// in kernel/arch/x86/interrupt.c, kernel/arch/arm64/macies/raspi3/peripherals.c
void arch_enable_irq(unsigned irq);
void arch_disable_irq(unsigned irq);
/// in kernel/arch/<arch>/vm.c
__mustuse error_t arch_vm_map(struct task *task, vaddr_t vaddr, paddr_t paddr,
                              paddr_t kpage, unsigned flags);
__mustuse error_t arch_vm_unmap(struct task *task, vaddr_t vaddr);
paddr_t vm_resolve(struct task *task, vaddr_t vaddr);

#endif
