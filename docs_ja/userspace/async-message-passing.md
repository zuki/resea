# 非同期IPC

同期IPCでほとんどの場合はうまくいきますが、非同期のメッセージパッシングが
便利な場面もあります。

Resea標準ライブラリは同期メッセージパッシングと通知を使った非同期メッセージ
パッシングを提供しています。詳細は以下の例を見てください。

```c
#include <resea/async.h>

error_t async_send(task_t dst, struct message *m);
error_t async_recv(task_t src, struct message *m);
error_t async_reply(task_t dst);
```

一言で言うと、非同期ライブラリはメッセージキューを管理します。非同期メッセージは
キューに入れられ、宛先タスクには保留の非同期メッセージがあることが通知されます。
このメッセージはクライアントがpullリクエスト（`ASYNC_MSG`）を送信した際に配信
されます。

## 非同期メッセージを送信する

`asyn_send`によりメッセージをキューに入れ、`async_reply`によりpullリクエスト
メッセージ（`ASYNC_MSG`）を処理します。

```c
// my_server.c

void somewhere(void) {
    // `async_send`はメッセージをキューに入れ、通知 `NOTIFY_ASYNC`を
    // 宛先タスクに通知します
    m.type = BENCHMARK_NOP_MSG;
    async_send(dst, &m);
}

void main(void) {
    while (true) {
        struct message m;
        bzero(&m, sizeof(m));
        ASSERT_OK(ipc_recv(IPC_ANY, &m));

        switch (m.type) {
            case ASYNC_MSG:
                // クライアントのasync_recv()でリクエストを処理する
                async_reply(m.src);
                break;
        }
    }
}
```

## 非同期メッセージを受信する

`NOTIFY_ASYNC`通知を待ち、`ipc_recv`を使って保留中の非同期メッセージを
受信します。

```c
// my_client.c

void main(void) {
    while (true) {
        struct message m;
        bzero(&m, sizeof(m));
        ASSERT_OK(ipc_recv(IPC_ANY, &m));

        switch (m.type) {
            case NOTIFICATIONS_MSG:
                if (m.notifications.data & NOTIFY_ASYNC) {
                    // 保留中の非同期メッセージをサーバから取り込む。
                    // 見ての通り、事前にどのサーバが非同期メッセージを
                    // 送るのかは知っている必要があるが、どのタスクが
                    // NOTIFY_ASYNCを通知したのかは知ることができない。
                    async_recv(my_server, &m);
                    switch (m.type) {
                        case BENCHMARK_NOP_MSG:
                            INFO("received a async message!");
                    }
                }
        }
    }
}
```
