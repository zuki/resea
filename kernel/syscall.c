#include "syscall.h"
#include "ipc.h"
#include "kdebug.h"
#include "printk.h"
#include "task.h"
#include <arch.h>
#include <list.h>
#include <string.h>
#include <types.h>

/** @ingroup kernel
 * @brief ユーザ空間からメモリをコピーする. ユーザポインタが不正な場合、この関数。
 * または、ページフォルトハンドラがカレントタスクをkillする。
 * @param dst コピー先アドレス
 * @param src コピー元アドレス
 * @param len コピーサイズ
 */
void memcpy_from_user(void *dst, __user const void *src, size_t len) {
    // 1. コピー元範囲がカーネル領域に重なる場合はエラー
    if (is_kernel_addr_range((vaddr_t) src, len)) {
        task_exit(EXP_INVALID_MEMORY_ACCESS);
    }
    // 2. アーキテクチャ固有の処理関数に処理させる
    arch_memcpy_from_user(dst, src, len);
}

/** @ingroup arm64
 * @brief メモリをユーザ空間にコピーする. ユーザポインタが不正な場合、この関数。
 * または、ページフォルトハンドラがカレントタスクをkillする。
 * @param dst コピー先アドレス
 * @param src コピー元アドレス
 * @param len コピーサイズ
 */
void memcpy_to_user(__user void *dst, const void *src, size_t len) {
    // 1. コピー先範囲がカーネル領域に重なる場合はエラー
    if (is_kernel_addr_range((vaddr_t) dst, len)) {
        task_exit(EXP_INVALID_MEMORY_ACCESS);
    }
    // 2. アーキテクチャ固有の処理関数に処理させる
    arch_memcpy_to_user(dst, src, len);
}

/** @ingroup kernel
 * @brief ユーザ空間からメモリをコピーする. NUL文字を考慮しない。すなわち、
 *      単に `len` バイトコピーし、`len` バイト目に '\0' をセットする。したがって、
 *      `dst` には `len + 1` バイトのスベースが必要である。 `len` が0の場合は、
 *      呼び出し元は `dst` に少なくとも1バイトのスペースを用意する必要がある。
 * @param dst コピー先アドレス
 * @param src コピー元アドレス
 * @param len コピーサイズ
 */
static void strncpy_from_user(char *dst, __user const char *src, size_t len) {
    memcpy_from_user(dst, src, len);
    dst[len] = '\0';
}

/** @ingroup kernel
 * @brief task_createシステムコールの実行関数. タスクを作成する。
 * @param tid タスクID
 * @param name タスク名
 * @param ip 実行するプログラムのエントリポイント
 * @param pager 使用するページャのタスクID
 * @param flags フラグ
 * @return 正常に処理できたら OK; それ以外は ::error_t 型のエラーコード.
 */
static error_t sys_task_create(task_t tid, __user const char *name, vaddr_t ip,
                               task_t pager, unsigned flags) {
    // 1. カレントタスクに実行権限なし
    if (!CAPABLE(CURRENT, CAP_TASK)) {
        return ERR_NOT_PERMITTED;
    }
    // 2. 指定のIDのタスクがない、またはカレントタスク
    struct task *task = task_lookup_unchecked(tid);
    if (!task || task == CURRENT) {
        return ERR_INVALID_TASK;
    }

    // 3. タスクを作成する。pager==0の場合、ここではエラーとする
    struct task *pager_task = task_lookup(pager);
    if (!pager_task) {
        return ERR_INVALID_ARG;
    }
    // 4. タスク名をセットする
    char namebuf[CONFIG_TASK_NAME_LEN];
    strncpy_from_user(namebuf, name, sizeof(namebuf) - 1);
    // 5. タスク作成関数を呼び出す
    return task_create(task, namebuf, ip, pager_task, flags);
}

/** @ingroup kernel
 * @brief task_destroyシステムコールの実行関数. タスクを破棄する。
 * @param tid タスクID
 * @return 正常に処理できたら OK; それ以外は ::error_t 型のエラーコード.
 */
static error_t sys_task_destroy(task_t tid) {
    if (!CAPABLE(CURRENT, CAP_TASK)) {
        return ERR_NOT_PERMITTED;
    }

    struct task *task = task_lookup_unchecked(tid);
    if (!task || task == CURRENT) {
        return ERR_INVALID_TASK;
    }

    return task_destroy(task);
}

/** @ingroup kernel
 * @brief task_exitシステムコールの実行関数. カレントタスクを終了する。
 * @return この関数は復帰しない。
 */
static error_t sys_task_exit(void) {
    task_exit(EXP_GRACE_EXIT);
    UNREACHABLE();
}

/** @ingroup kernel
 * @brief task_selfシステムコールの実行関数. カレントタスクのIDを返す。
 * @return カレントタスクのID。
 */
static task_t sys_task_self(void) {
    return CURRENT->tid;
}

/** @ingroup kernel
 * @brief task_scheduleシステムコールの実行関数. タスクのスケジューリングポリシを更新する。
 * @param tid タスクID
 * @param priority 新しい優先度
 * @return 正常に処理できたら OK; それ以外は ::error_t 型のエラーコード.
 */
static error_t sys_task_schedule(task_t tid, int priority) {
    if (!CAPABLE(CURRENT, CAP_TASK)) {
        return ERR_NOT_PERMITTED;
    }

    struct task *task = task_lookup_unchecked(tid);
    if (!task || task == CURRENT) {
        return ERR_INVALID_TASK;
    }

    return task_schedule(task, priority);
}

/** @ingroup kernel
 * @brief ipcシステムコールの実行関数. IPCメッセージを送受信する。
 * @param dst 送信先のタスクID
 * @param src 送信元のタスクID
 * @param m メッセージへのポインタ
 * @param flass IPCフラグ
 * @return 正常に処理できたら OK; それ以外は ::error_t 型のエラーコード.
 */
static error_t sys_ipc(task_t dst, task_t src, __user struct message *m,
                       unsigned flags) {
    // 1. カーネルからの呼び出しではないこと
    if (flags & IPC_KERNEL) {
        return ERR_INVALID_ARG;
    }

    // 2. 送信元のタスクIDが正しいこと
    if (src < 0 || src > CONFIG_NUM_TASKS) {
        return ERR_INVALID_ARG;
    }

    // 3. IDから送信先のタスク構造体を取得する
    struct task *dst_task = NULL;
    if (flags & IPC_SEND) {
        dst_task = task_lookup(dst);
        if (!dst_task) {
            return ERR_INVALID_TASK;
        }
    }
    // 4. ipc()関数に処理させる
    return ipc(dst_task, src, m, flags);
}

/** @ingroup kernel
 * @brief notifyシステムコールの実行関数. 通知を送信する。
 * @param dst 送信先のタスクID
 * @param notifications 通知
 * @return 正常に処理できたら OK; それ以外は ::error_t 型のエラーコード.
 */
static error_t sys_notify(task_t dst, notifications_t notifications) {
    // 1. IDから送信先のタスク構造体を取得する
    struct task *dst_task = task_lookup(dst);
    if (!dst_task) {
        return ERR_INVALID_TASK;
    }
    // 2. notify()に処理させる
    notify(dst_task, notifications);
    return OK;
}

/** @ingroup kernel
 * @brief notiftimer_setシステムコールの実行関数. タスクのタイマーをセットする。
 * @param timeout タイムアウト値
 * @return 常に OK
 */
static error_t sys_timer_set(msec_t timeout) {
    CURRENT->timeout = timeout;
    return OK;
}

/** @ingroup kernel
 * @brief irq_acquireシステムコールの実行関数. IRQの所有権を取得する。
 * @param irq IRQ番号
 * @return 正常に処理できたら OK; それ以外は ::error_t 型のエラーコード.
 */
static error_t sys_irq_acquire(int irq) {
    if (!CAPABLE(CURRENT, CAP_IRQ)) {
        return ERR_NOT_PERMITTED;
    }

    return task_listen_irq(CURRENT, irq);
}

/** @ingroup kernel
 * @brief irq_releaseシステムコールの実行関数. IRQの所有権を解放する。
 * @param irq IRQ番号
 * @return 正常に処理できたら OK; それ以外は ::error_t 型のエラーコード.
 */
static error_t sys_irq_release(int irq) {
    if (!CAPABLE(CURRENT, CAP_IRQ)) {
        return ERR_NOT_PERMITTED;
    }

    return task_unlisten_irq(irq);
}

/** @ingroup kernel
 * @brief `vaddr` からマッピングされる物理メモリアドレスを解決する.
 * @param vaddr 仮想アドレス
 * @return 正常に処理できたら OK; それ以外は ::error_t 型のエラーコード.
 */
static paddr_t resolve_paddr(vaddr_t vaddr) {
    if (CURRENT->tid == INIT_TASK) {
        return vaddr;
    } else {
        return vm_resolve(CURRENT, vaddr);
    }
}

/** @ingroup kernel
 * @brief vm_mapシステムコールの実行関数. タスクの仮想メモリ空間のメモリページを
 *      マップする。これはシステムコールのうちで一番危険なそうであることに注意
 *      されたい。ユーザタスクはカーネルデータ領域を含む全ての物理メモリ空間を
 *      マップすることができる。
 * @param tid タスクID
 * @param vaddr 仮想アドレス
 * @param src マップする仮想アドレス
 * @param kpage ページアドレス. アーキテクチャ固有のページテーブル構造体用の
 *      メモリページを提供するメモリページ（仮想アドレス）
 * @param flags フラグ
 * @return 正常に処理できたら ::OK; それ以外は ::error_t 型のエラーコード
 */
static error_t sys_vm_map(task_t tid, vaddr_t vaddr, vaddr_t src, vaddr_t kpage,
                          unsigned flags) {
    if (!CAPABLE(CURRENT, CAP_MAP)) {
        return ERR_NOT_PERMITTED;
    }
    // アドレスはすべてページ境界にあること
    if (!IS_ALIGNED(vaddr, PAGE_SIZE) || !IS_ALIGNED(src, PAGE_SIZE)
        || !IS_ALIGNED(kpage, PAGE_SIZE)) {
        return ERR_INVALID_ARG;
    }

    // srcを物理アドレスに解決する
    paddr_t paddr = resolve_paddr(src);
    if (!paddr) {
        return ERR_NOT_FOUND;
    }

    // kpageを物理アドレスに解決する
    paddr_t kpage_paddr = resolve_paddr(kpage);
    if (!kpage_paddr) {
        return ERR_NOT_FOUND;
    }

    // 以下のpaddrチェックはデバッグ目的で追加されたものであることに注意
    // されたい。セキュアではない。ユーザはページテーブルディレクトリを変更
    // することでカーネルも森空間にアクセスすることができるからである。
    //      => 現在はエラーにしている。
    if (is_kernel_paddr(paddr)) {
        WARN_DBG("paddr %p points to a kernel memory area", paddr);
        return ERR_NOT_ACCEPTABLE;
    }

    if (is_kernel_paddr(kpage_paddr)) {
        WARN_DBG("kpage %p points to a kernel memory area", kpage);
        return ERR_NOT_ACCEPTABLE;
    }

    // IDからタスク構造体を取得する
    struct task *task = task_lookup(tid);
    if (!task) {
        return ERR_INVALID_TASK;
    }
    // vm_map()に処理させる
    return vm_map(task, vaddr, paddr, kpage_paddr, flags);
}

/** @ingroup kernel
 * @brief vm_unmapシステムコールの実行関数. タスクの仮想メモリ空間のメモリページを
 *      アンマップする。
 * @param tid タスクID
 * @param vaddr 仮想アドレス
 * @return 正常に処理できたら ::OK; それ以外は ::error_t 型のエラーコード
 */
static error_t sys_vm_unmap(task_t tid, vaddr_t vaddr) {
    if (!CAPABLE(CURRENT, CAP_MAP)) {
        return ERR_NOT_PERMITTED;
    }

    if (!IS_ALIGNED(vaddr, PAGE_SIZE)) {
        return ERR_INVALID_ARG;
    }

    struct task *task = task_lookup(tid);
    if (!task) {
        return ERR_INVALID_TASK;
    }

    return vm_unmap(task, vaddr);
}

/** @ingroup kernel
 * @brief console_writeシステムコールの実行関数. ログメッセージをアーキテクチャの
 *      コンソール（通常はシリアルポート）とカーネルログバッファに書き出す。
 * @param buf ログメッセージが入ったバッファ
 * @param buf_len バッファ長
 * @return 正常に処理できたら ::OK; それ以外は ::error_t 型のエラーコード
 */
static error_t sys_console_write(__user const char *buf, size_t buf_len) {
    if (buf_len > 1024) {
        WARN_DBG("console_write: too long buffer length");
        return ERR_TOO_LARGE;
    }

    char kbuf[512];
    int remaining = buf_len;
    while (remaining > 0) {
        int copy_len = MIN(remaining, (int) sizeof(kbuf));
        memcpy_from_user(kbuf, buf, copy_len);
        for (int i = 0; i < copy_len; i++) {
            printk("%c", kbuf[i]);
        }
        remaining -= copy_len;
    }

    return OK;
}

/** @ingroup kernel
 * @brief console_readシステムコールの実行関数. アーキテクチャのコンソール
 *      （通常はシリアルポート）から文字列を読み込む。
 * @param buf 読み込んだ文字列を保管するバッファ
 * @param max_len バッファ長
 * @return 読み込んだ文字数; それ以外は ::error_t 型のエラーコード
 */
static int sys_console_read(__user char *buf, int max_len) {
    if (!max_len) {
        return 0;
    }

    int i = 0;
    for (; i < max_len - 1; i++) {
        char ch;
        if ((ch = kdebug_readchar()) <= 0) {
            break;
        }

        memcpy_to_user(buf + i, &ch, 1);
    }

    memcpy_to_user(buf + i, "\0", 1);
    return i;
}

/** @ingroup kernel
 * @brief kdebugシステムコールの実行関数. カーネルデバッガコマンドを実行して、
 *      結果をbufにコピーする。
 * @param cmd コマンド
 * @param cmd_len コマンド長
 * @param buf klogを読み込むバッファ
 * @param buf_len バッファ長
 * @return 正常に処理できたら ::OK; それ以外は ::error_t 型のエラーコード
 */
static error_t sys_kdebug(__user const char *cmd, size_t cmd_len,
                          __user char *buf, size_t buf_len) {
    if (!CAPABLE(CURRENT, CAP_KDEBUG)) {
        return ERR_NOT_PERMITTED;
    }

    char cmd_buf[128];
    if (cmd_len >= sizeof(cmd_buf) - 1) {
        return ERR_TOO_LARGE;
    }

    // klogを書き出すためのバッファ。bufは__userなので一旦カーネル空間の
    // バッファに書き込んでから、bufにmemcpy_to_user()する
    char out_buf[512];
    out_buf[0] = '\0';
    if (buf_len >= sizeof(out_buf) - 1) {
        return ERR_TOO_LARGE;
    }

    strncpy_from_user(cmd_buf, cmd, cmd_len);

    error_t err = kdebug_run(cmd_buf, out_buf, buf_len);
    if (err != OK) {
        return err;
    }

    memcpy_to_user(buf, out_buf, strlen(out_buf) + 1);
    return OK;
}

/** @ingroup kernel
 * @brief システムコールハンドラ関数.
 * @param n システムコール番号
 * @param a1 第1引数
 * @param a2 第2引数
 * @param a3 第3引数
 * @param a4 第4引数
 * @param a5 第5引数
 * @return 各システムコール処理関数のリターン値
 */
long handle_syscall(int n, long a1, long a2, long a3, long a4, long a5) {
    stack_check();

    long ret;
    switch (n) {
        case SYS_IPC:
            ret = sys_ipc(a1, a2, (__user struct message *) a3, a4);
            break;
        case SYS_NOTIFY:
            ret = sys_notify(a1, a2);
            break;
        case SYS_TIMER_SET:
            ret = sys_timer_set(a1);
            break;
        case SYS_CONSOLE_WRITE:
            ret = sys_console_write((__user const char *) a1, a2);
            break;
        case SYS_CONSOLE_READ:
            ret = sys_console_read((__user char *) a1, a2);
            break;
        case SYS_TASK_CREATE:
            ret = sys_task_create(a1, (__user const char *) a2, a3, a4, a5);
            break;
        case SYS_TASK_DESTROY:
            ret = sys_task_destroy(a1);
            break;
        case SYS_TASK_EXIT:
            ret = sys_task_exit();
            break;
        case SYS_TASK_SELF:
            ret = sys_task_self();
            break;
        case SYS_TASK_SCHEDULE:
            ret = sys_task_schedule(a1, a2);
            break;
        case SYS_VM_MAP:
            ret = sys_vm_map(a1, a2, a3, a4, a5);
            break;
        case SYS_VM_UNMAP:
            ret = sys_vm_unmap(a1, a2);
            break;
        case SYS_IRQ_ACQUIRE:
            ret = sys_irq_acquire(a1);
            break;
        case SYS_IRQ_RELEASE:
            ret = sys_irq_release(a1);
            break;
        case SYS_KDEBUG:
            ret = sys_kdebug((__user const char *) a1, a2, (__user char *) a3,
                             a4);
            break;
        case SYS_NOP:
            ret = OK;
            break;
        default:
            ret = ERR_INVALID_ARG;
    }

    stack_check();
    return ret;
}

#ifdef CONFIG_ABI_EMU
/// The system call handler for ABI emulation.
void abi_emu_hook(trap_frame_t *frame, enum abi_hook_type type) {
    struct message m;
    m.type = ABI_HOOK_MSG;
    m.abi_hook.type = type;
    m.abi_hook.task = CURRENT->tid;
    memcpy(&m.abi_hook.frame, frame, sizeof(m.abi_hook.frame));

    error_t err = ipc(CURRENT->pager, CURRENT->pager->tid,
                      (__user struct message *) &m, IPC_CALL | IPC_KERNEL);
    if (IS_ERROR(err)) {
        WARN_DBG("%s: aborted kernel ipc", CURRENT->name);
        task_exit(EXP_ABORTED_KERNEL_IPC);
    }

    // Check if the reply is valid.
    if (m.type != ABI_HOOK_REPLY_MSG) {
        WARN_DBG("%s: invalid abi hook reply (type=%d)", CURRENT->name, m.type);
        task_exit(EXP_INVALID_MSG_FROM_PAGER);
    }

    memcpy(frame, &m.abi_hook_reply.frame, sizeof(*frame));
}
#endif
