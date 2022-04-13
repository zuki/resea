# アウトオブラインペイロード

メッセージは固定長であり、そのサイズは非常に小さい（通常は256バイト）ので、
大きなデータ（ファイルコンテンツなど）を送るための別の方法が必要です。

*アウトオブラインペイロード* (短く*OoL*) はこの目的のために`vm`で実装
されている機能です。

## OoL種別

OoLは次のペイロード種別をサポートしています。

| 種別    | 説明       |
|---------|-------------------|
| `bytes` | 任意のデータ |
| `str`   | `\0`終端の文字列 |

## 注意事項

- `vm`に何回ものIPCコールが必要なため現在のところ遅いです
- メッセージあたりOolペイロードは1つしかサポートしていません
- OoLペイロードの最大サイズはビルドconfigで構成可能です。

## OoLペイロードを送信する

OolはIDLとユーザ空間ライブラリに統合されています。例を見てみましょう。

```
namespace fs {
    rpc open(path: str) -> (handle: handle);
    rpc create(path: str, exist_ok: bool) -> (handle: handle);
    rpc close(handle: handle) -> ();
    rpc read(handle: handle, offset: offset, len: size) -> (data: bytes);
    rpc write(handle: handle, offset: offset, data: bytes) -> ();
    rpc stat(path: str) -> (size: size);
}
```

上に示した`fs`インタフェース定義において、ご覧のようにいくつかのメソッドでOoL
ペイロードが使われています。`fs.open`はオープンするパス名を送信するために`str`
ペイロードを使い、`fs_read`は読み込んだファイルコンテンツを`bytes`ペイロードで
返しています。

`bytes`を送信するにはデータのポインタとデータの長さをセットします。

```c
static void read_file_contents(task_t client, uint8_t *data, size_t len) {
    struct message m;
    m.type = FS_READ_REPLY_MSG;
    m.fs_read_reply.data = data;
    m.fs_read_reply.data_len = len;
    ipc_call(client, &m);
}
```

`str`を送信するには、`str`ペイロードとして*ヌル終端した*ASCII文字列へのポインタを
セットしてください。IPCライブラリは`strlen`を使ってOoLペイロードの長さを計算して
透過的に送信先タスク（fsサーバ）へコピーします。

```c
struct message m;
m.type = FS_OPEN_MSG;
m.fs_open.path = "/hello.txt";
error_t err = ipc_call(fs_server, &m);
```

## OoLペイロードを受信する

In the fs server, the IPC library sets a valid pointer to the OoL payload field. For `str` payloads, it is guaranteed that the string is terminalted by `\0`.

```c
struct message m;
ipc_recv(IPC_ANY, &m);
if (m.type == FS_OPEN_MSG) {
    DBG("path = %s", m.fs_open.path);
}
```

For `bytes` payload, use `<name>_len` to determine the size of the payload:

```c
ipc_recv(fs_server, &m);
if (m.type == FS_READ_REPLY_MSG) {
    HEXDUMP(m.fs_read_reply.data, m.fs_read_reply.data_len);
}
```

A memory buffer for received OoL payload is dynamically allocated by `malloc`. **Don't forget to `free`!**

## How It Works
```
+--------+  3. ool.send  +------+  2. ool.recv  +----------+
| sender |  -----------> |  vm  | <------------ | receiver |
|  task  |               |      |               |   task   |
|        |               |      |  4. copy OoL  |          |
|        |               |      | ------------> |          |
|        |               |      |               |          |
|        |               |      | 5. send msg   |          |
|        | -----------------------------------> |          |
|        |               |      |               |          |
|        |               |      | 6. ool.verify |          |
|        |               |      | <------------ |          |
+--------+               +------+               +----------+
```

1. For messages with a ool payload, IPC stub generator adds `MSG_OOL` to the message type field (i.e. `(m.type & MSG_OOL) != 0` is true).
2. In `ipc_recv` API, the receiver task sends a `ool.recv` message to tell the location of the OoL receive buffer (allocated by `malloc`) to the vm server.
3. When a sender task `ipc_send` API, if `MSG_OOL` bit is set, it calls `ool.send` to the vm server before sending the message.
4. The vm server looks for an unsed OoL buffer in the desitnation task, copies the OoL payload into the buffer, and returns the pointer to buffer in the receiver's address space.
5. The sender tasks overwrites the OoL field with the receiver's pointer and sends the message.
6. Once receiver task received a message with OoL, it calls vm's `ool.verify` to check if the received pointer and the length is valid.
7. `ipc_recv` returns.

### Why not Implement OoL in Kernel?
In fact, OoL is initially implemented in the kernel and is removed later because it turned out that page fault handling makes the kernel complicated.

Since we can now map memory pages in the userspace through the `map` system call, I believe that it is a better idea to implement a more efficient message passing with OoL support within userspace using shared memory.
