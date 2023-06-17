/**
 * @file malloc.c
*/
#include <list.h>
#include <resea/malloc.h>
#include <resea/printf.h>
#include <string.h>

/** @ingroup resea
 * @def NUM_BINS
 * @brief binsの個数
 */
#define NUM_BINS 16
/* 以下の2変数はlibs/resea/arch/arm64/user.ld で定義 */
extern char __heap[];
extern char __heap_end[];

/** @ingroup resea
 * @var bins
 * @brief 固定サイズのメモリ配列. binsの各要素は2^idxサイズでidx=15までは
 * 固定サイズで静的に確保。それ以外はリンクリストで管理して追加が可能。
 * <pre>
 * bins: 0, 1, 2, 3,  4,  5,  6,   7,   8,   9, 10, 11, 12, 13,  14,  15
 * size: 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1K, 2K, 4K, 8K, 16K, 32K
 * </pre>
 */
static struct malloc_chunk *bins[NUM_BINS];

/** @ingropu resea
 * @brief バッファオーバーフローをチェックする. オーバーフローした場合はpanicで停止する。
 * @param chunk メモリチャンクへのポインタ
*/
static void check_buffer_overflow(struct malloc_chunk *chunk) {
    // 1. 未使用なら問題なし
    if (chunk->magic == MALLOC_FREE) {
        return;
    }

    // 2. アンダーフロー領域が更新されていたらアンダーフロー: panic
    for (size_t i = 0; i < MALLOC_REDZONE_LEN; i++) {
        if (chunk->underflow_redzone[i] != MALLOC_REDZONE_UNDFLOW_MARKER) {
            PANIC("detected a malloc buffer underflow: ptr=%p", chunk->data);
        }
    }

    // 3. データ領域のオーバーフロー領域が更新されていたらオーバーフロー: panic
    for (size_t i = 0; i < MALLOC_REDZONE_LEN; i++) {
        if (chunk->data[chunk->capacity + i] != MALLOC_REDZONE_OVRFLOW_MARKER) {
            PANIC("detected a malloc buffer overflow: ptr=%p", chunk->data);
        }
    }
}

/** @ingroup resea
 * @brief 新たにメモリチャンクを追加する. binsのidx=15以降の最後に追加される。
 * @param ptr 追加するメモリチャンクへのポインタ
 * @param len チャンクの長さ
 * @return 追加したメモリチャンクへの構造体
 */
static struct malloc_chunk *insert(void *ptr, size_t len) {
    ASSERT(len > MALLOC_FRAME_LEN);
    // 1. 新規チャンクを設定する
    struct malloc_chunk *new_chunk = ptr;
    new_chunk->magic = MALLOC_FREE;
    new_chunk->capacity = len - MALLOC_FRAME_LEN;
    new_chunk->size = 0;
    new_chunk->next = NULL;

    // 2. 固定のbins配列の後ろに新規作成したチャンクのリンクリストがあるが、その末尾を探す.
    struct malloc_chunk **chunk = &bins[NUM_BINS - 1];
    while (*chunk != NULL) {
        check_buffer_overflow(*chunk);
        chunk = &(*chunk)->next;
    }
    // 3. 新規作成したチャンクをリストにつなげる（以下の*chankは末尾のchunk->next）
    *chunk = new_chunk;
    //INFO("insert: new_chunk=0x%#llx, len=0x%#llx", new_chunk, len);
    return new_chunk;
}

/** @ingroup resea
 * @brief chunkを分割する. このチャンクはチャンクリストには繋がない。
 * @param ptr 分割するメモリチャンクへのポインタ
 * @param len 新規チャンクのデータ長
 * @return 新規メモリチャンクへの構造体
 */
static struct malloc_chunk *split(struct malloc_chunk *chunk, size_t len) {
    // 1. overflowレッドゾーンを加味した新規チャンク長を設定
    size_t new_chunk_len = MALLOC_FRAME_LEN + len;
    ASSERT(chunk->capacity >= new_chunk_len);
    // 2. 新規チャンクの先頭ポインタを計算
    void *new_chunk_ptr =
        &chunk->data[chunk->capacity + MALLOC_REDZONE_LEN - new_chunk_len];
    // 3. chunkの収容サイズから新規チャンク分を減ずる
    chunk->capacity -= new_chunk_len;

    ASSERT(new_chunk_len > MALLOC_FRAME_LEN);
    // 4. 新規チャンク構造体を設定する
    struct malloc_chunk *new_chunk = new_chunk_ptr;
    new_chunk->magic = MALLOC_FREE;
    new_chunk->capacity = len;
    new_chunk->size = 0;
    new_chunk->next = NULL;

    return new_chunk;
}

/** @ingroup resea
 * @brief サイズに合うbins要素のインデックスを求める.
 * @param size メモリサイズ
 * @return 要求サイズが最大のサイズ固定チャンク(32KB)以下の場合はそのインデックス、
 *         超える場合はチャンクリストにあることを示す -1;
 */
static int get_bin_idx_from_size(size_t size) {
    for (size_t i = 0; i < NUM_BINS - 1; i++) {
        if (size <= 1 << i) {
            return i;
        }
    }

    return -1;
}

/** @ingroup resea
 * @brief 指定サイズのメモリを割り当てる.
 * @param size メモリサイズ
 * @return メモリへのポインタ
 */
void *malloc(size_t size) {
    // 1. 要求サイズが0の場合は1とする
    if (!size) {
        size = 1;
    }

    // 2. 16バイト境界に丸めあげる。要求メモリが16バイト未満(0を含む）の場合は
    //    16バイト割り当てる。（FIXME: 16バイト未満のbinsの意味は?）
    size = ALIGN_UP(size, 16);

    // 3. サイズに合うbins配列要素のインデックスを取得する
    int bin_idx = get_bin_idx_from_size(size);
#if 0
    INFO("malloc[3]: size=%#llx, bin_idx=%d, bins[bin_idx]=0x%p\n", size, bin_idx,
        bin_idx != -1 ? bins[bin_idx] : NULL);
    INFO("malloc: size=0x%#llx, bin_idx=%d", size, bin_idx);
    struct malloc_chunk *c15 = bins[15];
    INFO("[MALLOC] bins[15]: chunk=0x%#llx, cap=%#llx, size=%#llx, magic=0x%#llx, next=0x%#llx", c15, c15->capacity, c15->size, c15->magic, c15->next);
#endif
    // 固定サイズチャンクがあいていれば割り当てる
    if (bin_idx != -1 && bins[bin_idx] != NULL) {
        // 該当する大きさのチャンクが空いているかチェック
        struct malloc_chunk *allocated = bins[bin_idx];
        ASSERT(allocated->magic == MALLOC_FREE);

        allocated->magic = MALLOC_IN_USE;
        allocated->size = size;
        memset(allocated->underflow_redzone, MALLOC_REDZONE_UNDFLOW_MARKER,
               MALLOC_REDZONE_LEN);
        memset(&allocated->data[allocated->capacity],
               MALLOC_REDZONE_OVRFLOW_MARKER, MALLOC_REDZONE_LEN);
        //INFO("- use bins[%d], next bins[%d]=0x%p", bin_idx, bin_idx, allocated->next);
        bins[bin_idx] = allocated->next;
        allocated->next = NULL;
#if 0
        INFO("allocated=0x%#llx from bins[%d]", allocated, bin_idx);

            struct malloc_chunk *c15 = bins[15];
            INFO("[FIX] bins[15]: chunk=0x%#llx, cap=%#llx, size=%#llx, magic=0x%#llx, next=0x%#llx", c15, c15->capacity, c15->size, c15->magic, c15->next);
            uint8_t *p = c15->underflow_redzone;
            INFO(" - underflow: %x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
            p = c15->data;
            INFO(" - data: %x", c15->data);
#endif
        return allocated->data;
    }

    // 動的チャンクから割り当てる
    struct malloc_chunk *prev = NULL;
    for (struct malloc_chunk *chunk = bins[NUM_BINS - 1]; chunk;
         chunk = chunk->next) {
#if 0
        if (chunk->magic != MALLOC_FREE) {
            INFO("BAD chunk: bin_idx=%d, chunk=0x%#llx, cap=%#llx, size=%#llx, magic=0x%#llx, next=0x%#llx", bin_idx, chunk, chunk->capacity, chunk->size, chunk->magic, chunk->next);
        }
#endif
        ASSERT(chunk->magic == MALLOC_FREE);

        struct malloc_chunk *allocated = NULL;
        size_t chunk_size = bin_idx < 0 ? size : (1 << bin_idx);
        // チャンクからchunk_sizeを切り出してもまだ余裕あり
        if (chunk->capacity > chunk_size + MALLOC_FRAME_LEN) {
            allocated = split(chunk, chunk_size);
        // チャンクからchunk_sizeを切り出せるのは今回のみ
        } else if (chunk->capacity >= chunk_size) {
            allocated = chunk;
            // リストからチャンクを削除
            if (prev) {
                // チャンクがリストの先頭でない場合
                prev->next = chunk->next;
            } else {
                // チャンクがリストの戦闘の場合
                bins[NUM_BINS - 1] = bins[NUM_BINS - 1]->next;
            }
            //INFO("no additional chunk");
        }
        //INFO("allocated=0x%#llx from chunk", allocated);
        if (allocated) {
            allocated->magic = MALLOC_IN_USE;
            allocated->size = size;
            memset(allocated->underflow_redzone, MALLOC_REDZONE_UNDFLOW_MARKER,
                   MALLOC_REDZONE_LEN);
            memset(&allocated->data[allocated->capacity],
                   MALLOC_REDZONE_OVRFLOW_MARKER, MALLOC_REDZONE_LEN);
            allocated->next = NULL;
            return allocated->data;
        }
        prev = chunk;
        //INFO("next-loop: 0x%p", chunk->next);
    }

    PANIC("out of memory: 0x%#llx", size);
}

/// ptr(chunk->data)からchunkを返す
static struct malloc_chunk *get_chunk_from_ptr(void *ptr) {
    struct malloc_chunk *chunk =
        (struct malloc_chunk *) ((uintptr_t) ptr - sizeof(struct malloc_chunk));

    // Check its magic and underflow/overflow redzones.
    ASSERT(chunk->magic == MALLOC_IN_USE);
    check_buffer_overflow(chunk);
    return chunk;
}

void free(void *ptr) {
    if (!ptr) {
        return;
    }
    struct malloc_chunk *chunk = get_chunk_from_ptr(ptr);
    if (chunk->magic == MALLOC_FREE) {
        PANIC("double-free bug!");
    }

    chunk->magic = MALLOC_FREE;

    int bin_idx = get_bin_idx_from_size(chunk->capacity);
    bin_idx = bin_idx < 0 ? NUM_BINS - 1 : bin_idx;

    struct malloc_chunk *head = bins[bin_idx];
    if (head) {
        chunk->next = head;
    }
    bins[bin_idx] = chunk;
    //INFO("free: bins[%d]=0x%p", bin_idx, bins[bin_idx]);
}

void *realloc(void *ptr, size_t size) {
    if (!ptr) {
        return malloc(size);
    }

    struct malloc_chunk *chunk = get_chunk_from_ptr(ptr);
    size_t prev_size = chunk->size;
    if (size <= chunk->capacity) {
        // There's enough room. Keep using the current chunk.
        return ptr;
    }

    // There's not enough room. Allocate a new space and copy old data.
    void *new_ptr = malloc(size);
    memcpy(new_ptr, ptr, prev_size);
    free(ptr);
    return new_ptr;
}

char *strndup(const char *s, size_t n) {
    char *new_s = malloc(n + 1);
    strncpy2(new_s, s, n + 1);
    return new_s;
}

char *strdup(const char *s) {
    return strndup(s, strlen(s));
}

/** @ingroup resea
 * @brief mallocを初期化する.
*/
void malloc_init(void) {
    //INFO("malloc_init: heap=%p, len=0x%#llx", __heap, (size_t) __heap_end - (size_t) __heap);
    insert(__heap, (size_t) __heap_end - (size_t) __heap);
}
