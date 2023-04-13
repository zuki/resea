# メインループ

まず、メインループを書きます。`servers/example`にあるテンプレートを使うことが
できます。

```c
// main.c
#include <resea/ipc.h>
#include <resea/printf.h>
#include <string.h>

void main(void) {
    TRACE("starting...");

    INFO("ready");
    while (true) {
        struct message m;
        bzero(&m, sizeof(m));
        ASSERT_OK(ipc_recv(IPC_ANY, &m));

        switch (m.type) {
            case BENCHMARK_NOP_MSG:
                m.type = BENCHMARK_NOP_REPLY_MSG;
                m.benchmark_nop_reply.value = 123456789;
                ipc_reply(m.src, &m);
                break;
            default:
                TRACE("unknown message %d", m.type);
        }
    }
}
```

この短いスニペットからReseaでのサーバ（アプリケーション）の書き方を学ぶことができます。

- ユーザ空間プログラムにおいては*Resea 標準ライブラリ* (`libs/resea`)を使用します。
  Cでは`<resea/*.h>`ヘッダファイルを指定することでこのライブラリを使用できます。
  - `<resea/ipc.h>`: `ipc_recv`などのメッセージパッシングAPI
  - `<resea/printf.h>`: `INFO`, `TRACE`, `ASSERT_OK`などのプリント/アサーションマクロ。
- メッセージパッシングAPI(`ipc_recv`と`ipc_reply`)は同期的（ブロックする）であり、
  メッセージは固定長（アーキテクチャにより、32-256バイト）です。
- クライアントへのメッセージの応答にメッセージバッファ`m`を再利用する場合は、
  情報漏れを防ぐために`bzero`でバッファをクリアします。
- シングルスレッドのイベントドリブンのプログラム、すなわち、メッセージを受け取り、
  処理し、応答する。そして、新しいメッセージを待つ、というプログラムを書くことを
  推奨します。
  - これによりマルチサーバオペレーティングシステムのような複雑なソフトウェアのデバッグ
    のコストが大幅に削減されます。[John Ousterhout. Why Threads Are A Bad Idea (for most purposes)](https://web.stanford.edu/~ouster/cgi-bin/papers/threads.pdf)を参照してください。
