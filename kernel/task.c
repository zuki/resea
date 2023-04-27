/** @file task.c */
#include "task.h"
#include "ipc.h"
#include "kdebug.h"
#include "printk.h"
#include "syscall.h"
#include <arch.h>
#include <config.h>
#include <list.h>
#include <message.h>
#include <string.h>

/** @ingroup task
 * @var tasks
 * @brief 全タスク
 */
static struct task tasks[CONFIG_NUM_TASKS];
/** @ingroup task
 * @var runqueues
 * @brief 実行可能状態のタスクのキュー（現在実行中のタスクは除く）.
 *        indexの小さいものほど優先度は高い。
 */
static list_t runqueues[TASK_PRIORITY_MAX];
/** @ingroup task
 * @var irq_owners
 * @brief IRQの所有者
 */
static struct task *irq_owners[IRQ_MAX];

/** @ingroup task
 * @brief タスクをランキューに入れる.
 * @param task タスクへのポインタ
 */
static void enqueue_task(struct task *task) {
    list_push_back(&runqueues[task->priority], &task->runqueue_next);
}

/** @ingroup task
 * @brief 指定されたタスクIDを持つタスク構造体を取得する（使用済みであるかチェックしない）.
 * @param task_t タスクID
 * @return タスク構造体へのポインタ。IDが不正であった場合は NULL.
 */
struct task *task_lookup_unchecked(task_t tid) {
    if (tid <= 0 || tid > CONFIG_NUM_TASKS) {
        return NULL;
    }

    return &tasks[tid - 1];
}

/** @ingroup task
 * @brief 指定されたタスクIDを持つタスク構造体を取得する.
 * @param task_t タスクID
 * @return タスク構造体へのポインタ。IDが不正である、または、タスクが
 *         未使用(TASK_UNUSED)の場合は NULL.
 */
struct task *task_lookup(task_t tid) {
    struct task *task = task_lookup_unchecked(tid);
    if (!task || task->state == TASK_UNUSED) {
        return NULL;
    }

    return task;
}

/** @ingroup task
 * @brief タスクを新規作成・初期化して、ランキューに入れる.
 * @param task タスク構造体へのポインタ
 * @param name タスク名
 * @param ip 実行するプログラムのエントリポイント
 * @param pager 使用するページャへのポインタ
 * @param flags フラグ
 * @return 正常に処理できたら OK; それ以外は error_t 型のエラーコード.
 */
error_t task_create(struct task *task, const char *name, vaddr_t ip,
                    struct task *pager, unsigned flags) {
    // 1. taskが使用済みの場合はエラー
    if (task->state != TASK_UNUSED) {
        return ERR_ALREADY_EXISTS;
    }

    // 2. flagsに未知の値がないかチェック
    unsigned allowed_flags = TASK_ALL_CAPS | TASK_ABI_EMU | TASK_HV;
    if ((flags & ~allowed_flags) != 0) {
        WARN_DBG("unknown task flags (%x)", flags);
        return ERR_INVALID_ARG;
    }
    // 2.1 TASK_ABI_EMUとTASK_HVは同時には指定できない
#if defined(CONFIG_ABI_EMU) || defined(CONFIG_HYPERVISOR)
    if ((flags & (TASK_ABI_EMU | TASK_HV)) == (TASK_ABI_EMU | TASK_HV)) {
        WARN_DBG("TASK_ABI_EMU and TASK_HV are exclusive", flags);
        return ERR_INVALID_ARG;
    }
#endif
    // 2.2 CONFIG_ABI_EMUが未定義でTASK_ABI_EMUが設定されている
#ifndef CONFIG_ABI_EMU
    if ((flags & TASK_ABI_EMU) != 0) {
        WARN_DBG("ABI emulation is not enabled");
        return ERR_UNAVAILABLE;
    }
#endif
    // 2.3 CONFIG_HYPERVISORが未定義でTASK_HVが設定されている
#ifndef CONFIG_HYPERVISOR
    if ((flags & TASK_HV) != 0) {
        WARN_DBG("hypervisor support is not enabled");
        return ERR_UNAVAILABLE;
    }
#endif

    // 3. アーキテクチャ固有の初期化処理を行う
    error_t err;
    if ((err = arch_task_create(task, ip)) != OK) {
        return err;
    }

    // 4. task構造体のフィールドを初期化する
    TRACE("new task #%d: %s (pager=%s)", task->tid, name,
          pager ? pager->name : NULL);
    task->state = TASK_BLOCKED;
    task->flags = flags;
    task->notifications = 0;
    task->pager = pager;
    task->src = IPC_DENY;
    task->timeout = 0;
    task->quantum = 0;
    task->priority = TASK_PRIORITY_MAX - 1;
    task->ref_count = 0;
    bitmap_fill(task->caps, sizeof(task->caps), (flags & TASK_ALL_CAPS) != 0);
    strncpy2(task->name, name, sizeof(task->name));
    list_init(&task->senders);
    list_nullify(&task->runqueue_next);
    list_nullify(&task->sender_next);

    if (pager) {
        pager->ref_count++;
    }

    // 5. 新規作成したタスクをランキューに追加する.
    if (task != IDLE_TASK && ((flags & TASK_SCHED) == 0)) {
        task_resume(task);
    }

    return OK;
}

/** @ingroup task
 * @brief タスクを破棄する.
 * @param task タスク構造体へのポインタ
 * @return 正常に処理できたら OK; それ以外は error_t 型のエラーコード.
 */
error_t task_destroy(struct task *task) {
    // 1. カレントタスク、アイドルタスクは破棄できない
    ASSERT(task != CURRENT);
    ASSERT(task != IDLE_TASK);
    // 2. initタスクは破棄できない
    if (task->tid == INIT_TASK) {
        WARN_DBG("tried to destroy the init task");
        return ERR_INVALID_ARG;
    }
    // 3. 未使用タスクが指定された
    if (task->state == TASK_UNUSED) {
        return ERR_INVALID_ARG;
    }
    // 4. 他のタスクから参照されている場合は破棄できない
    if (task->ref_count > 0) {
        WARN_DBG("%s (#%d) is still referenced from %d tasks", task->name,
                 task->tid, task->ref_count);
        return ERR_IN_USE;
    }

    TRACE("destroying %s...", task->name);
    // 5. ランキュー、送信者キューから削除
    list_remove(&task->runqueue_next);
    list_remove(&task->sender_next);
    // 6. アーキテクチャ固有の破棄処理を行う
    arch_task_destroy(task);
    // 7. タスク状態を未使用に戻す
    task->state = TASK_UNUSED;

    // 8. ページャから切り離す
    if (task->pager) {
        task->pager->ref_count--;
    }

    // 9. senderのIPC操作をすべて中止する
    LIST_FOR_EACH (sender, &task->senders, struct task, sender_next) {
        notify(sender, NOTIFY_ABORTED);
        list_remove(&sender->sender_next);
    }

    // 10. IRQの所有権を解除する
    for (unsigned irq = 0; irq < IRQ_MAX; irq++) {
        if (irq_owners[irq] == task) {
            arch_disable_irq(irq);
            irq_owners[irq] = NULL;
        }
    }

    return OK;
}

/** @ingroup task
 * @brief カレントタスクを終了させる.
 * @param exp taskが終了した理由
 */
__noreturn void task_exit(enum exception_type exp) {
    // 1. アイドルタスクは終了できない
    ASSERT(CURRENT != IDLE_TASK);
    // 2. initタスクは終了できない(initタスクだけはpagerがない)
    if (!CURRENT->pager) {
        PANIC("the initial task tried to exit");
    }

    // 3. このタスクが終了したことをページャに伝える
    struct message m;
    m.type = EXCEPTION_MSG;
    m.exception.task = CURRENT->tid;
    m.exception.exception = exp;
    error_t err = ipc(CURRENT->pager, 0, (__user struct message *) &m,
                      IPC_SEND | IPC_KERNEL);
    OOPS_OK(err);

    // 4. ページャがこのタスクを破棄するまで待機する
    CURRENT->state = TASK_BLOCKED;
    CURRENT->src = IPC_DENY;
    task_switch();
    UNREACHABLE();
}

/** @ingroup task
 * @brief タスクを一時停止する. `task->src` の更新も忘れないこと。
 * @param task タスクへのポインタ
 */
void task_block(struct task *task) {
    DEBUG_ASSERT(task->state == TASK_RUNNABLE);
    task->state = TASK_BLOCKED;
}

/** @ingroup task
 * @brief タスクを再開する.
 * @param task タスクへのポインタ
 */
void task_resume(struct task *task) {
    DEBUG_ASSERT(task->state == TASK_BLOCKED);
    task->state = TASK_RUNNABLE;
    enqueue_task(task);
    mp_reschedule();
}

/** @ingroup task
 * @brief タスクのスケジューリングポリシーを更新する.
 * @param task タスクへのポインタ
 * @param priority 新たな優先度
 */
error_t task_schedule(struct task *task, int priority) {
    // 1. 優先度チェック
    if (priority >= TASK_PRIORITY_MAX) {
        return ERR_INVALID_ARG;
    }
    // 2. タスク優先度を指定の優先度に変更
    task->priority = priority;
    // 3. ランキューにある場合は、一旦削除してエンキューし直す
    //    ランキューは優先度ごとにあるため
    if (task->state == TASK_RUNNABLE) {
        list_remove(&task->runqueue_next);
        enqueue_task(task);
    }

    return OK;
}

/** @ingroup task
 * @brief タスクスケジューラ.
 * @param current カレントタスク
 * @return スケジュールされたタスクへのポインタ
 */
static struct task *scheduler(struct task *current) {
    // 1. カレントタスクがまだ実行可能な場合はランキューに入れる。
    if (current != IDLE_TASK && current->state == TASK_RUNNABLE) {
        enqueue_task(current);
    }

    // 2. 最も優先度の高いタスクを探す。優先度が同じタスクはラウンドロビン方式で
    //    スケジュールする。
    for (int i = 0; i < TASK_PRIORITY_MAX; i++) {
        struct task *next =
            LIST_POP_FRONT(&runqueues[i], struct task, runqueue_next);
        if (next) {
            return next;
        }
    }

    return IDLE_TASK;
}

/** @ingroup task
 * @brief コンテキストスイッチを行う. 現在のコンテキストをスタックに保存して
 *        次のスレッドのコンテキストを復元する。
 */
void task_switch(void) {
    // 1. カレントタスクがスタックを使い尽くしていないかチェックする
    stack_check();
    // 2. 次に実行するタスクを求める
    struct task *prev = CURRENT;
    struct task *next = scheduler(prev);
    // 3. 次に実行するタスクのquantumをセットする
    next->quantum = TASK_TIME_SLICE;
    // 4. カレントタスク以外に実行可能なタスクがない場合はカレントタスクを
    //    引き続き実行する
    if (next == prev) {
        return;
    }
    // 5. アーキテクチャ固有の実際の切り替え処理を行う
    CURRENT = next;
    arch_task_switch(prev, next);
    // 6. 切り替えたタスクがスタックを使い尽くしていないかチェックする
    stack_check();
}

/** @ingroup task
 * @brief IRQからの通知の受信を開始する.
 * @param task タスクへのポインタ
 * @param irq 通知を受信するirq番号
 */
error_t task_listen_irq(struct task *task, unsigned irq) {
    // 1. irqパラメタのチェック
    if (irq >= IRQ_MAX) {
        return ERR_INVALID_ARG;
    }
    // 2. irqはすでに他に所有者がいる
    if (irq_owners[irq]) {
        return ERR_ALREADY_EXISTS;
    }
    // 3. irqの所有者となる
    irq_owners[irq] = task;
    // 4. irqを有効にする
    arch_enable_irq(irq);
    TRACE("enabled IRQ: task=%s, vector=%d", task->name, irq);
    return OK;
}

/** @ingroup task
 * @brief IRQからの通知の受信を停止する.
 * @param irq 通知を受信しているirq番号
 */
error_t task_unlisten_irq(unsigned irq) {
    if (irq >= IRQ_MAX) {
        return ERR_INVALID_ARG;
    }
    // 1. irqを無効にする
    arch_disable_irq(irq);
    // 2. irqの所有権を手放す
    irq_owners[irq] = NULL;
    TRACE("disabled IRQ: vector=%d", irq);
    return OK;
}

/** @ingroup task
 * @brief タスクの仮想メモリ空間にメモリページをマップする.
 * @param task タスクへのポインタ
 * @param vaddr 仮想アドレス
 * @param paddr 物理アドレス
 * @param kpage アーキテクチャ固有のページテーブル構造体用のメモリページを与えるメモリページ
 * @param flags フラグ
 * @return マッピングに成功したら OK; それ以外は error_t 型のエラーコード
 */
__mustuse error_t vm_map(struct task *task, vaddr_t vaddr, paddr_t paddr,
                         paddr_t kpage, unsigned flags) {
    // 1. 各アドレスはページサイズにアライメントされていること
    DEBUG_ASSERT(IS_ALIGNED(vaddr, PAGE_SIZE));
    DEBUG_ASSERT(IS_ALIGNED(paddr, PAGE_SIZE));
    DEBUG_ASSERT(IS_ALIGNED(kpage, PAGE_SIZE));

    // 2. カーネルメモリの破損を防ぐ。vaddrがカーネルメモリ領域にある場合はエラー。
    //    ページテーブル構造体をマッピングすることによりユーザは依然として
    //    このチェックをバイパスしてカーネルメモリにアクセスできることに注意されたい。
    if (is_kernel_addr_range(vaddr, PAGE_SIZE)) {
        WARN_DBG("vaddr %p points to a kernel memory area", vaddr);
        return ERR_NOT_ACCEPTABLE;
    }

    // 3. マッピングが悪意のないものであることをチェックしてページテーブルを更新する。
    return arch_vm_map(task, vaddr, paddr, kpage, flags);
}

/** @ingroup task
 * @brief タスクの仮想メモリ空間からメモリページをアンマップする.
 * @param task タスクへのポインタ
 * @param vaddr 仮想アドレス
 * @return マッピングに成功したら OK; それ以外は error_t 型のエラーコード
 */
error_t vm_unmap(struct task *task, vaddr_t vaddr) {
    // 1. アドレスはページサイズにアライメントされていること
    DEBUG_ASSERT(IS_ALIGNED(vaddr, PAGE_SIZE));

    // 2. 仮想アドレスがマップされているか確認する
    paddr_t paddr = vm_resolve(task, vaddr);
    if (!paddr) {
        return ERR_NOT_FOUND;
    }

    // 3. アーキテクチャ固有のアンマップ処理を行う
    return arch_vm_unmap(task, vaddr);
}

/** @ingroup task
 * @brief タイマー割り込みを処理する. タイマーは 1/TICK_HZ 毎に発火する.
 */
void handle_timer_irq(void) {
    bool resumed_by_timeout = false;
    // 1. CPUがBSPの場合のみ処理する
    if (mp_is_bsp()) {
        // 2. タイムアウトを処理する
        for (int i = 0; i < CONFIG_NUM_TASKS; i++) {
            struct task *task = &tasks[i];
            // 2.1 未使用またはtimerが設定されていない場合は読み飛ばす
            if (task->state == TASK_UNUSED || !task->timeout) {
                continue;
            }
            // 2.2 経過時間を差し引く
            task->timeout--;
            // 2.3 タイムアウト時間になったら通知する
            if (!task->timeout) {
                notify(task, NOTIFY_TIMER);
                resumed_by_timeout = true;
            }
        }
    }

    // 3. カレントタスクがquantumを使い尽くしたらタスクスイッチを行う
    DEBUG_ASSERT(CURRENT == IDLE_TASK || CURRENT->quantum >= 0);
    CURRENT->quantum--;
    if (CURRENT->quantum < 0 || (CURRENT == IDLE_TASK && resumed_by_timeout)) {
        task_switch();
    }
}

/** @ingroup task
 * @brief カーネルで使用されているタイマーデバイス以外の割り込み例外を処理する.
 * @param irq 割り込み番号
 */
void handle_irq(unsigned irq) {
    struct task *owner = irq_owners[irq];

    // 1. irqの所有者に割り込みを通知する。
    if (owner) {
        notify(owner, NOTIFY_IRQ);
        // 2. カレントタスクがアイドルタスクだったらタスクを切り替える
        if (CURRENT == IDLE_TASK) {
            task_switch();
        }
    }
}

/// The page fault handler. It calls a pager to ask to update the page table.
/** @ingroup task
 * @brief ページフォルトを処理する. ページャを呼び出してページテーブルを
 *        更新するよう依頼する。
 * @param addr フォルトメモリアドレス
 * @param ip フォルトが発生したプログラムアドレス
 * @param fault フォルト原因
 */
void handle_page_fault(vaddr_t addr, vaddr_t ip, unsigned fault) {
    // 1. initタスクでフォルトが発生したらpanic
    if (CURRENT->pager == NULL) {
        PANIC("page fault in the init task: addr=%p, ip=%p", addr, ip);
    }

    // 2. ページャに送信するメッセージを作成する
    struct message m;
    m.type = PAGE_FAULT_MSG;
    m.page_fault.task = CURRENT->tid;
    m.page_fault.vaddr = addr;
    m.page_fault.ip = ip;
    m.page_fault.fault = fault;
    error_t err = ipc(CURRENT->pager, CURRENT->pager->tid,
                      (__user struct message *) &m, IPC_CALL | IPC_KERNEL);
    // 3. エラーが発生した場合はタスクを終了させる
    if (err != OK || m.type != PAGE_FAULT_REPLY_MSG) {
        task_exit(EXP_INVALID_MSG_FROM_PAGER);
    }
}

/** @ingroup task
 * @brief タスクの状態をプリントする. デバックで使用する。
 */
void task_dump(void) {
    const char *states[] = {
        [TASK_UNUSED] = "unused",
        [TASK_RUNNABLE] = "runnable",
        [TASK_BLOCKED] = "blocked",
    };

    for (unsigned i = 0; i < CONFIG_NUM_TASKS; i++) {
        struct task *task = &tasks[i];
        if (task->state == TASK_UNUSED) {
            continue;
        }

        INFO("#%d %s: state=%s, src=%d", task->tid, task->name,
             states[task->state], task->src);
        if (!list_is_empty(&task->senders)) {
            INFO("  senders:");
            LIST_FOR_EACH (sender, &task->senders, struct task, sender_next) {
                INFO("    - #%d %s", sender->tid, sender->name);
            }
        }
    }
}

/** @ingroup task
 * @brief タスクサブシステムを初期化する.
 */
void task_init(void) {
    // 1. ランキューを初期化する
    for (int i = 0; i < TASK_PRIORITY_MAX; i++) {
        list_init(&runqueues[i]);
    }
    // 2. タスク配列を初期化する
    for (int i = 0; i < CONFIG_NUM_TASKS; i++) {
        tasks[i].state = TASK_UNUSED;
        tasks[i].tid = i + 1;
    }
    // 3. irq所有者配列を初期化する
    for (int i = 0; i < IRQ_MAX; i++) {
        irq_owners[i] = NULL;
    }
}
