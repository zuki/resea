# デバッグ

今のところリッチなデバッグ機能はありません。printfマクロを使用
してください。幸運を祈ります。

## IPCにおけるデッドロックの扱い方

*mutex*のようなロックを使わなくても（そのようなものは提供していないことに注意）、
IPC操作によりプログラムが永遠にブロックされることがあります。

よくあるケースはプログラムと宛先タスクの双方が互いにメッセージを送信しようと
している場合です。これはカーネルデバッガの`ps`コマンドで確認することができます。

```
kdebug> ps
#1 vm: state=blocked, src=0
#2 display: state=blocked, src=0
#3 e1000: state=blocked, src=6
  senders:
    - #6 tcpip
#4 ps2kbd: state=blocked, src=0
#5 shell: state=blocked, src=0
#6 tcpip: state=blocked, src=3
  senders:
    - #3 e1000
#7 webapi: state=blocked, src=0
```

`e1000`と`tcpip`がブロックされており、両者が互いに送信していることに
注目してください。
