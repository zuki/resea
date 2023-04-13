/**
 * @file malloc.h
*/
#ifndef __RESEA_MALLOC_H__
#define __RESEA_MALLOC_H__

#include <config.h>
#include <types.h>

/** @ingroup resea
 * @def MALLOC_FREE
 * @brief binが空いていることを表す印 */
#define MALLOC_FREE        0x0a110ced0a110cedULL /* hexspeak of "alloced" */
/** @ingroup resea
 * @def MALLOC_IN_USE
 * @brief binが使用中であることを表す印 */
#define MALLOC_IN_USE      0xdea110cddea110cdULL /* hexspeak of "deallocd" */
/** @ingroup resea
 * @def MALLOC_REDZONE_LEN
 * @brief メモリチャンクのレッドゾーン長 */
#define MALLOC_REDZONE_LEN 16
/** @ingroup resea
 * @def MALLOC_FRAME_LEN
 * @brief メモリチャンクのフレーム長.（under/overレッドゾーンを含み、データ領域を除いたチャンク長） */
#define MALLOC_FRAME_LEN   (sizeof(struct malloc_chunk) + MALLOC_REDZONE_LEN)

/** @ingroup resea
 * @def MALLOC_REDZONE_UNDFLOW_MARKER
 * @brief アンダーフローレッドゾーンのを表す印 */
#define MALLOC_REDZONE_UNDFLOW_MARKER 0x5a
/** @ingroup resea
 * @def MALLOC_REDZONE_OVRFLOW_MARKER
 * @brief オーバーフローレッドゾーンのを表す印 */
#define MALLOC_REDZONE_OVRFLOW_MARKER 0x5b

/** @ingroup resea
 * @struct malloc_chunk
 * @brief （割当済み/空き）メモリリチャンクのヘッダー. データ領域は
 * このヘッダの直後に置かれる（`data`はその領域を指子示す）
 */
struct malloc_chunk {
    struct malloc_chunk *next;  /**< 次のmealloc_chunk構造体へのポインタ */
    size_t capacity;            /**< 最大収容サイズ */
    size_t size;                /**< 現在のサイズ */
    uint64_t magic;             /**< 使用状況を示すマジックナンバー */
    uint8_t underflow_redzone[MALLOC_REDZONE_LEN];      /**< アンダーフローレッドゾーン: 0x5a x 16 */
    uint8_t data[];             /**< データ領域へのポインタ */
    // `overflow_redzone` は`data`の直後に設定する
    // uint8_t overflow_redzone[MALLOC_REDZONE_LEN];    // 0x5b x 16
};

#ifdef ARCH_X64
/// Ensure that it's aligned to 16 bytes for performance (SSE instructions
/// requires 128-bit-aligned memory address).
STATIC_ASSERT(sizeof(struct malloc_chunk) == 48);
#endif

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
char *strndup(const char *s, size_t n);
char *strdup(const char *s);
void malloc_init(void);

#endif
