# メモリ割り当て (malloc)

動的メモリ割り当てAPIを提供しています。

```c
#include <resea/malloc.h>

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
```

詳細についてはUNIXの[manページ](https://linux.die.net/man/3/malloc)を
参照してください。
