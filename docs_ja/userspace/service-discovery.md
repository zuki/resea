# サービスの発見

vmサーバはサービスの発見を実装しています。これにより名前でサービスを
探すことができます。

## サービスを登録する

```c
#include <resea/ipc.h>

error_t ipc_serve(const char *name);
```

## サービスを探す

```c
#include <resea/ipc.h>

task_t ipc_lookup(const char *name);
```

この関数は指定した名前のサーバが登録されるまでブロックし、サーバのタスクIDを
返します。
