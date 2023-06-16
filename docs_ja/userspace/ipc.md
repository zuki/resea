# IPC

## ヘッダファイル

```c
#include <resea/ipc.h>
```

## メッセージ構造体

メッセージは固定長です。メッセージ種別（またはエラー）、送り手のタスクID、
メッセージペイロード（[IDL](idl)で定義された任意のバイト）を含みます。

```c
/// メッセージ
struct message {
    /// メッセージ種別。負値の場合は、このフィールドはエラー(error_t)を表す。
    int type;
    /// このメッセージの送り手のタスクID
    task_t src;
    /// メッセージ内容。共用体であり構造体ではないことに注意
    union {
        // 生バイトによるメッセージ内容
        uint8_t raw[MESSAGE_SIZE - sizeof(int) - sizeof(task_t)];

        // メッセージフィールドの共通ヘッダー
        struct {
            /// 送付すべきアウトオブラインのポインタ。MSG_OOLが設定されている場合に使用される。
            void *ool_ptr;
            /// アウトオブラインペイロードのバイト単位のサイズ
            size_t ool_len;
        };

        // 自動生成されるメッセージフィールド:
        //
        //     struct { notifcations_t data; } notifcations;
        //     struct { task_t task; ... } page_fault;
        //     struct { paddr_t paddr; } page_reply_fault;
        //     ...
        //
        IDL_MESSAGE_FIELDS
   };
};
```

## メッセージを送信する

ReseaではIPC操作は同期的です。送付先はタスクIDで指定します。簡単のために
*channel*と呼ばれる間接的なIPC機構は提供していません。

```c
error_t ipc_send(task_t dst, struct message *m);
error_t ipc_send_err(task_t dst, error_t error);
error_t ipc_send_noblock(task_t dst, struct message *m);
```

`ipc_send_err`は`m.type`に`error`をセットしてエラーメッセージを送信する
ラッパー関数です。

`ipc_send_noblock`はメッセージの送信を試みます。送付先のタスクがメッセージを
受け取る準備ができていない場合は、送信元のタスクをブロックする代わりに直ちに
`ERR_WOULD_BLOCK`を返します。

## メッセージを受信する

受信操作に関しては2つの選択肢があります。*open recieve*と*closed receive*です。

- *open receive* (`src == IPC_ANY`の場合): 任意のタスクからのメッセージを受信します
- *closed receive* (それ以外の場合): 指定したタスク(`src`)からのメッセージを受信します。
  他の送り手のタスクはブロックされます。


```c
error_t ipc_recv(task_t src, struct message *m);
```

## サーバからのメッセージに応答する

送り手のタスクが応答メッセージを待たない場合は、次のラッパー関数（`ipc_send_noblock`
をラップ）を使用してください。クライアントが`ipc_call`でサーバを呼び出した場合、
これらのAPIは成功するはずです。

```c
void ipc_reply(task_t dst, struct message *m);
void ipc_reply_err(task_t dst, error_t error);
```

## 通知を送信する

*通知*はLinuxの*シグナル*のような非同期なIPCです。タスクは各自、通知ビット
フィールドを持っています。タスクがメッセージを受信しようとした際に保留通知がある
（ビットフィールドがゼロでない）と、カーネルは通知メッセージ`NOTIFICATIONS_MSG`を
作成して返します。

```c
error_t ipc_notify(task_t dst, notifications_t notifications);
```

`ipc_notify`は送り先タスクの通知ビットフィールドと指定したビットのビット論理和操作、
`dst->notifications |= notifications`を行います。

## メッセージの送受信を一度に行う

```c
error_t ipc_call(task_t dst, struct message *m);
error_t ipc_replyrecv(task_t dst, struct message *m);
```

`ipc_call`は、`ipc_send(dst, m)`を行ってから`ipc_recv(dst, m)`を行うことと
同じです。クライアントは2つのAPIは使わずに`ipc_call`を使用するべきです。
そうでないとサーバからの`ipc_reply`が失敗する可能性があるからです。

両APIとも受信したメッセージでメッセージバッファ`m`を上書きします。

`ipc_replyrecv`は、`ipc_reply(dst, m)`を行ってから`ipc_recv(IPC_ANY, m)`を
行うことと同じです。このAPIを使うとサーバ内のシステムコールの数を減らすことが
できます。
