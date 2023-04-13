# インタフェース定義言語 (IDL)

複数のプログラミング言語をサポートする予定があるので相互可用性のために
独自のインタフェース定義言語 (IDL) を使用します。

メッセージ定義はIDLファイルから自動的に生成されます。

例を見てみましょう。

## IDLファイル

```
namespace fs {
    rpc open(path: str) -> (handle: handle);
    rpc close(handle: handle) -> ();
    rpc read(handle: handle, offset: offset, len: size) -> (data: bytes);
}
```

## 生成されるメッセージ定義 (`build/include/idl.h`)

```c
struct fs_open_fields {
    char *path;
    size_t path_len;            // 自動的に追加されている
};

struct fs_open_reply_fields {
    handle_t handle;
};

struct fs_close_fields {
    handle_t handle;
};

struct fs_close_reply_fields {
};

struct fs_read_fields {
    handle_t handle;
    offset_t offset;
    size_t len;
};

struct fs_read_reply_fields {
    void *data;
    size_t data_len;            // 自動的に追加されている
};

#define IDL_MESSAGE_FIELDS \
    ...
    struct fs_open_fields fs_open; \
    struct fs_open_reply_fields fs_open_reply; \
    struct fs_close_fields fs_close; \
    struct fs_close_reply_fields fs_close_reply; \
    struct fs_read_fields fs_read; \
    struct fs_read_reply_fields fs_read_reply; \
    ...

#define IDL_STATIC_ASSERTS \
    ...
    STATIC_ASSERT(offsetof(struct message, fs_open.path) == offsetof(struct message, ool_ptr)); \
    STATIC_ASSERT(offsetof(struct message, fs_open.path_len) == offsetof(struct message, ool_len)); \
    STATIC_ASSERT(offsetof(struct message, fs_read_reply.data) == offsetof(struct message, ool_ptr)); \
    STATIC_ASSERT(offsetof(struct message, fs_read_reply.data_len) == offsetof(struct message, ool_len)); \
    ...
```

## 変換の実行 (in Makefile)

```Makefile
$(BUILD_DIR)/include/idl.h: tools/genidl.py $(wildcard *.idl */*.idl */*/*.idl)
    $(PROGRESS) "GEN" $@
    mkdir -p $(@D)
    ./tools/genidl.py --lang c --idl interface.idl -o $@
```

## 現在のReaseでの`.idl`ファイル

```bash
$ find . -name *.idl
./interface.idl
./servers/dm/interface.idl
./servers/random/interface.idl
./servers/tcpip/interface.idl
```

## データ型

```python
    builtins = dict(
        str="char *",
        bytes="void *",
        char="char",
        bool="bool",
        int="int",
        task="task_t",
        uint="unsigned",
        int8="int8_t",
        int16="int16_t",
        int32="int32_t",
        int64="int64_t",
        uint8="uint8_t",
        uint16="uint16_t",
        uint32="uint32_t",
        uint64="uint64_t",
        size="size_t",
        offset="offset_t",
        notifications="notifications_t",
        vaddr="vaddr_t",
        paddr="paddr_t",
        gpaddr="gpaddr_t",
        trap_frame="trap_frame_t",
        hv_frame="hv_frame_t",
        handle="handle_t",
        exception_type="enum exception_type",
    )
```
