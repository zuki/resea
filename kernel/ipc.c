/** @file ipc.c */
#include "ipc.h"
#include "printk.h"
#include "syscall.h"
#include "task.h"
#include <list.h>
#include <string.h>
#include <types.h>

/** @ingroup ipc
 * `receiver`タスクのsenderタスクを再開し、`receiver->src`プロパティを更新する.
 * @param receiver receiverタスクへのポインタ
 * @param src 送信元タスクへのポインタ
 */
static void resume_sender(struct task *receiver, task_t src) {
    // 1. srcがIPC_ANYの場合は最初のsender、またはsrcがreceiverのsendersリストにある
    //    senderの場合は、senderを再開してserndersリストから削除する
    LIST_FOR_EACH (sender, &receiver->senders, struct task, sender_next) {
        if (src == IPC_ANY || src == sender->tid) {
            DEBUG_ASSERT(sender->state == TASK_BLOCKED);
            DEBUG_ASSERT(sender->src == IPC_DENY);
            task_resume(sender);
            list_remove(&sender->sender_next);

            // src == IPC_ANY の場合、`sender` にのみメッセージの送信を許可する。
            // その理由を理解するために次のような状況を考える:
            //
            //     [Sender A]              [Receiver C]              [Sender B]
            //         .                        |                        |
            // in C's sender queue              |                        |
            //         .                        |                        |
            //         .        Resume          |                        |
            //         + <--------------------- |                        |
            //         .                        .    Try sending (X)     |
            //         .                        + <--------------------- |
            //         .                        |                        |
            //         V                        |                        |
            //         |                        |                        |
            //
            // (X)が発生した場合、CはすでにAを次の送信者として再開しているため、
            // receiverはBからのメッセージを受け取るべきではない

            // 2. receiverのsrcを更新する
            receiver->src = sender->tid;
            return;
        }
    }
    // 2. receiverのsrcを指定のsrcに更新する
    receiver->src = src;
}

/** @ingroup ipc
 * メッセージを送信して受信する（低速版）.
 * @param dst 送信先タスクへのポインタ
 * @param src 送信元タスクへのポインタ
 * @param m メッセージ構造体へのポインタ. IPC_KERNELがセットされていない場合は
 *          ユーザポインタ
 * @param flags IPCフラグ
 * @return 送受信がすべて正常終了したら OK; それ以外は error_t 型のエラーコード
 */
static error_t ipc_slowpath(struct task *dst, task_t src,
                            __user struct message *m, unsigned flags) {
    // 1. メッセージを送信する.
    if (flags & IPC_SEND) {
        // 1.1. 受信者がカレントページャタスクで、`m`にアクセスするとページフォルトが
        // 発生する場合に備えて、受信者のバッファにメッセージをコピーしておく。
        // この条件が発生した場合、デッドロックが発生する。
        struct message tmp_m;
        if (flags & IPC_KERNEL) {
            memcpy(&tmp_m, (const void *) m, sizeof(struct message));
        } else {
            memcpy_from_user(&tmp_m, m, sizeof(struct message));
        }

        // 1.2. 送信先タスク(receiver)の受信準備ができているかチェックする。
        bool receiver_is_ready =
            dst->state == TASK_BLOCKED
            && (dst->src == IPC_ANY || dst->src == CURRENT->tid);
        // 1.3. 準備ができていない場合
        if (!receiver_is_ready) {
            // 3.1 IPC_NOBLOCKがセットされている場合はエラー
            if (flags & IPC_NOBLOCK) {
                return ERR_WOULD_BLOCK;
            }

            // 3.2 reciverがカレントタスクを再開させるまでsleepする
            CURRENT->src = IPC_DENY;
            task_block(CURRENT);
            list_push_back(&dst->senders, &CURRENT->sender_next);
            task_switch();
            // 3.3 アボート通知で起こされた場合はエラーとする
            if (CURRENT->notifications & NOTIFY_ABORTED) {
                // The receiver task has exited. Abort the system call.
                CURRENT->notifications &= ~NOTIFY_ABORTED;
                return ERR_ABORTED;
            }
        }

        // もう戻れないところまで来てしまった。ここから送信を中止してはいけない。
        // エラーを返したり、ページフォルトを起こしたりしてはいけない！
        //
        // そうする必要がある場合は、CURRENTを送信者キューに押し戻すこと。

        // 1.4. メッセージをコピーする
        tmp_m.src = (flags & IPC_KERNEL) ? KERNEL_TASK : CURRENT->tid;
        memcpy(&dst->m, &tmp_m, sizeof(dst->m));

        // 1.5. receiverタスクを再開させる.
        task_resume(dst);

#ifdef CONFIG_TRACE_IPC
        TRACE("IPC: %s: %s -> %s", msgtype2str(tmp_m.type), CURRENT->name,
              dst->name);
#endif
    }

    // 2. メッセージを受信する
    if (flags & IPC_RECV) {
        struct message tmp_m;
        // 2.1 すべてのタスクから受信する設定で、保留されている通知がある場合
        if (src == IPC_ANY && CURRENT->notifications) {
            // 2.1.1 保留されている通知をメッセージとして受信する
            bzero(&tmp_m, sizeof(tmp_m));
            tmp_m.type = NOTIFICATIONS_MSG;
            tmp_m.src = KERNEL_TASK;
            tmp_m.notifications.data = CURRENT->notifications;
            CURRENT->notifications = 0;
        } else {
            // 2.1 以外の場合
            // 2.2.1 ブロックしない設定があったらエラーとする
            if ((flags & IPC_NOBLOCK) != 0) {
                return ERR_WOULD_BLOCK;
            }

            // 2.2.2 senderタスクを再開して、senderがこのタスクを再開するまでsleepする
            resume_sender(CURRENT, src);
            task_block(CURRENT);
            task_switch();

            // 2.2.3 memocpy_to_userはページフォルトを発生させる可能性があり、その場合
            //       CURRENT->mはページフォルトメッセージで上書きされてしまうので
            //       `tmp_m`にコピーしておく。
            memcpy(&tmp_m, &CURRENT->m, sizeof(struct message));
        }

        // 3. メッセージを受信し、receiverのバッファにコピーする
        if (flags & IPC_KERNEL) {
            memcpy((void *) m, &tmp_m, sizeof(struct message));
        } else {
            memcpy_to_user(m, &tmp_m, sizeof(struct message));
        }
    }

    return OK;
}

/** @ingroup ipc
 * メッセージを送信して受信する（高速版）. 一般的なケースに最適化したIPCの実装。
 * @param dst 送信先タスクへのポインタ
 * @param src 送信元タスクへのポインタ
 * @param m メッセージ構造体へのポインタ. IPC_KERNELがセットされていない場合は
 *          ユーザポインタ
 * @param flags IPCフラグ
 * @return 送受信がすべて正常終了したら OK; それ以外は error_t 型のエラーコード
 */
error_t ipc(struct task *dst, task_t src, __user struct message *m,
            unsigned flags) {
    // 1. 自分宛てに送信したらエラー
    if (dst == CURRENT) {
        WARN_DBG("%s: tried to send a message to myself", CURRENT->name);
        return ERR_INVALID_ARG;
    }

#ifdef CONFIG_IPC_FASTPATH
    // 2. メッセージが高速経路で送信できるかチェックする
    DEBUG_ASSERT((flags & IPC_SEND) == 0 || dst);
    int fastpath =
        // 高速版は ipc_call()とipc_replyrecv()歯科実装していない
        (flags & ~IPC_NOBLOCK) == IPC_CALL
        // receiverはすでに送信元を待っている
        && dst->state == TASK_BLOCKED
        && (dst->src == IPC_ANY || dst->src == CURRENT->tid)
        // 高速版は保留通知を受信しない
        && CURRENT->notifications == 0;
    // 3. 高速版を使えない場合は低速版を使用する
    if (!fastpath) {
        return ipc_slowpath(dst, src, m, flags);
    }

    // 4. 送信フェーズ: メッセージをコピーしてreceiverタスクを再開させる。
    //    このユーザコピーはページフォルトを発生する可能性があることに注意。
    memcpy_from_user(&dst->m, m, sizeof(struct message));
    dst->m.src = CURRENT->tid;
    task_resume(dst);

#    ifdef CONFIG_TRACE_IPC
    TRACE("IPC: %s: %s -> %s (fastpath)", msgtype2str(dst->m.type),
          CURRENT->name, dst->name);
#    endif

    // 5. 受信フェーズ: メッセージを待機し、ユーザバッファにコピーし、ユーザに返す。
    resume_sender(CURRENT, src);
    task_block(CURRENT);
    task_switch();

    // 上のユーザコピーでページを設定しているので、このユーザコピーでは
    // ページフォルトは発生しないはず。
    memcpy_to_user(m, &CURRENT->m, sizeof(struct message));
    return OK;
#else
    return ipc_slowpath(dst, src, m, flags);
#endif  // CONFIG_IPC_FASTPATH
}

/** @ingroup ipc
 * 他スックにnotificationsを通知する.
 * @param dst 送信先タスクへのポインタ
 * @param notifictions 通知情報
 */
void notify(struct task *dst, notifications_t notifications) {
    if (dst->state == TASK_BLOCKED && dst->src == IPC_ANY) {
        // 1. 直ちに NOTIFICATIONS_MSG メッセージを送信する
        dst->m.type = NOTIFICATIONS_MSG;
        dst->m.src = KERNEL_TASK;
        dst->m.notifications.data = dst->notifications | notifications;
        dst->notifications = 0;
        task_resume(dst);
    } else {
        // 2. タスクがイベントメッセージを受信する準備ができていない。
        //    メッセージを送信する代わりに保留中notificationを更新する。
        dst->notifications |= notifications;
    }
}
