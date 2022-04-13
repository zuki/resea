# インタフェース定義言語 (IDL)

複数のプログラミング言語をサポートする予定があるので相互可用性のために
独自のインタフェース定義言語 (IDL) を使用します。

メッセージ定義はIDLファイルから自動的に生成されます。

例を見てみましょう。

```
namespace fs {
    rpc open(path: str) -> (handle: handle);
    rpc close(handle: handle) -> ();
    rpc read(handle: handle, offset: offset, len: size) -> (data: bytes);
}
```
