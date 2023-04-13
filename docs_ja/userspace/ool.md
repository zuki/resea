# Out-of-Lineペイロード

メッセージは固定長であり、そのサイズは非常に小さい（通常は256バイト）ので、
大きなデータ（ファイルコンテンツなど）を送るための別の方法が必要です。

*Out-of-Lineペイロード* (短く*OoL*) はこの目的のために`vm`で実装
されている機能です。

## OoL種別

OoLは次のペイロード種別をサポートしています。

| 種別    | 説明       |
|---------|-------------------|
| `bytes` | 任意のデータ |
| `str`   | `\0`終端の文字列 |

## 注意事項

- `vm`に何回もIPCコールする必要があるため現在のところ遅いです
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

fsサーバではIPCライブラリがOoLペイロードフィールドへの正しいポインタをセット
します。`str`ペイロードでは文字列が`\0`終端されることが保証されています。

```c
struct message m;
ipc_recv(IPC_ANY, &m);
if (m.type == FS_OPEN_MSG) {
    DBG("path = %s", m.fs_open.path);
}
```

`bytes`ペイロードではペイロードのサイズを決めるために`<name>_len`を使用して
ください。

```c
ipc_recv(fs_server, &m);
if (m.type == FS_READ_REPLY_MSG) {
    HEXDUMP(m.fs_read_reply.data, m.fs_read_reply.data_len);
}
```

OoLペイロードを受信するためのメモリバッファは`malloc`で動的に割り当てられます。
**`free`することを忘れないでください。**

## どのように動作するのか

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

1. oolペイロードを持つメッセージについては、IPCスタブ生成器がメッセージ種別
   フィールドに`MSG_OOL`を追加します（したがって、`(m.type & MSG_OOL) != 0`が
   trueになります）。
2. `ipc_recv` APIでは受信側タスクは（`malloc`で割り当てた）OoL受信バッファの場所を
   vmサーバに知らせるために`ool.recv`メッセージを送信します。
3. 送信側タスクは`ipc_send` APIを送信する際、`MSG_OOL`ビットがセットされている場合は
   メッセージを送信する前にvmサーバの`ool.send`を呼び出します。
4. vmサーバは宛先タスクで未使用のOoLバッファを探してOoLペイロードをコピーし、受信者の
   アドレス空間におけるバッファのポインタを返します。
5. 送信側タスクはOoLフィールドを受信側のポインタで上書きし、メッセージを送信します。
6. 受信側タスクはOoL付きのメッセージを受信したらvmの`ool.verify`を呼び出して受信した
   ポインタと長さが正しいかチェックします。
7. `ipc_recv`がリターンします。

### なぜOoLをカーネルで実装しないのか

実際、初期にはOoLはカーネルで実装されていましたが、後に、ページフォルト処理が
カーネルを複雑にすることがわかったので削除されました。

現在は`map`システムコールを通じてユーザ空間のメモリページをマップできるので、
共有メモリを使ったユーザ空間におけるOoLをサポートした、より効率的なメッセージ
パッシングの実装はより良いアイデアだと考えます。
