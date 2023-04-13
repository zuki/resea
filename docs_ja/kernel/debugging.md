# カーネルデバッグ

ReseaのカーネルはCで書かれています。「Cはひどい言語だ。Rustで全部書き直すべきだ」と
言う人がいますが、何が起きているのかを理解しやすいのでカーネルを書くにはｃは非常に
良い選択です。

とはいえ、Cコードのデバッグは（特にカーネルの世界では）本当に骨が折れることです。
このページでは、カーネルデバッグのための便利な機能を紹介します。

## printk

次のマクロを使ってください（`libs/common/include/print_macros.h`で定義）。

- `TRACE(fmt, ...)`
  - traceメッセージ。リリースビルドでは無効になります。
- `DEBUG(fmt, ...)`
  - debugメッセージ。リリースビルドでは無効になります。
- `INFO(fmt, ...)`
  - infoメッセージ。
- `WARN(fmt, ...)`
  - warningメッセージ。
- `OOPS(fmt, ...)`
  - `WARN`と同じですが、バックトレースも出力します。
- `OOPS_OK(expr)`
  - `expr != OK`の場合、oopsメッセージを出力します。
- `PANIC(fmt, ...)`
  - カーネルパニック。メッセージを出力してCPUを停止します。

## バックトレース

- `backtrace()`
  - 次のようなバックトレースを出力します。代わりに`OOPS`マクロの使用を勧めます。

```
[kernel] WARN: Backtrace:
[kernel] WARN:     #0: ffff80000034a7e0 backtrace()+0x3c
[kernel] WARN:     #1: ffff800000113de7 kernel_ipc()+0x77
[kernel] WARN:     #2: ffff80000010f7bb mainloop()+0x6b
[kernel] WARN:     #3: ffff80000010f4a9 kernel_server_main()+0x149
[kernel] WARN:     #4: ffff8000001027d6 x64_start_kernel_thread(+0xa
```

## カーネルデバッガ

カーネルデバッガはデバッグビルドでのみ使用できます。これはシリアルポート経由で
利用できます。実装されているコマンドは次のとおりです。

- `ps`
  - プロセスとスレッドを一覧表示します。デッドロックのデバッグで役に立ちます。

訳注: システムコールで実装されている。

## ランタイムチェッカー

デバッグビルドでは次のランタイムチェッカーが有効になります。

- カーネルスタックカナリー
  - カーネルスタックの過剰な消費を検知します。
- [未定義動作サニタイザ (UBSan)](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html)
  - 整数オーバーフローなどの未定義動作を検知します。
