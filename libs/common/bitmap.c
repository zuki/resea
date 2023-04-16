/** @file bitmap.c */
#include <bitmap.h>
#include <print_macros.h>

/** @ingroup common
 * @brief bitmap全体を0xffまたは0x00で埋める.
 * @param bitmap bitmapへのポインタ
 * @param size bitmapのバイト数
 * @param value 1: 0xffで埋める、0: 0x00で埋める
 */
void bitmap_fill(uint8_t *bitmap, size_t size, int value) {
    memset(bitmap, value ? 0xff : 0x00, size);
}

/** @ingroup common
 * @brief bitmapのindex位置の値を取得する.
 * @param bitmap bitmapへのポインタ
 * @param size bitmapのバイト数
 * @param index 取得するbitmapの位置
 * @return index位置の値
 */
int bitmap_get(uint8_t *bitmap, size_t size, size_t index) {
    DEBUG_ASSERT(index < size * BITS_PER_BYTE);
    return bitmap[index / BITS_PER_BYTE] >> (index % BITS_PER_BYTE);
}

/** @ingroup common
 * @brief bitmapのindex位置に1をセットする.
 * @param bitmap bitmapへのポインタ
 * @param size bitmapのバイト数
 * @param index セットするbitmapの位置
 */
void bitmap_set(uint8_t *bitmap, size_t size, size_t index) {
    DEBUG_ASSERT(index < size * BITS_PER_BYTE);
    bitmap[index / BITS_PER_BYTE] |= 1 << (index % BITS_PER_BYTE);
}

/** @ingroup common
 * @brief bitmapのindex位置の値をクリアする(0をセットする).
 * @param bitmap bitmapへのポインタ
 * @param size bitmapのバイト数
 * @param index クリアするbitmapの位置
 */
void bitmap_clear(uint8_t *bitmap, size_t size, size_t index) {
    DEBUG_ASSERT(index < size * BITS_PER_BYTE);
    bitmap[index / BITS_PER_BYTE] &= ~(1 << (index % BITS_PER_BYTE));
}
