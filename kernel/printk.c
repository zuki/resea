/** @file printk.c */
#include "printk.h"
#include "ipc.h"
#include <string.h>
#include <vprintf.h>

/** @ingroup kernel
 * @var klog
 * @brief カーネルログ(klog)バッファ.
 */
static struct klog klog;

/** @ingroup kernel
 * @brief カーネルログバッファを読み込む.
 * @param buf バッファ
 * @param buf_len バッファ長
 * @return 読み込んだバイト数
 */
size_t klog_read(char *buf, size_t buf_len) {
    size_t remaining = buf_len;
    /** 末尾 > 先頭: 末尾からバッファの終端まで読み込む */
    if (klog.tail > klog.head) {
        int copy_len = MIN(remaining, CONFIG_KLOG_BUF_SIZE - klog.tail);
        memcpy(buf, &klog.buf[klog.tail], copy_len);
        buf += copy_len;
        remaining -= copy_len;
        klog.tail = 0;
    }
    /** 末尾から先頭まで読み込む */
    int copy_len = MIN(remaining, klog.head - klog.tail);
    memcpy(buf, &klog.buf[klog.tail], copy_len);
    remaining -= copy_len;
    klog.tail = (klog.tail + copy_len) % CONFIG_KLOG_BUF_SIZE;
    return buf_len - remaining;
}

/** @ingroup kernel
 * @brief カーネルログバッファに1文字書き出す.
 * @param ch 書き出す文字
 */
void klog_write(char ch) {
    klog.buf[klog.head] = ch;
    klog.head = (klog.head + 1) % CONFIG_KLOG_BUF_SIZE;
    if (klog.head == klog.tail) {
        // バッファは満杯。末尾を動かして文字を捨てる。
        klog.tail = (klog.tail + 1) % CONFIG_KLOG_BUF_SIZE;
    }
}

/** @ingroup kernel
 * @brief 1文字画面とカーネルゴルに書き出す.
 * @param ctx （未使用）
 * @param ch 書き出す文字
 */
static void printchar(__unused struct vprintf_context *ctx, char ch) {
    arch_printchar(ch);
    klog_write(ch);
}

/** @ingroup kernel
 * @brief メッセージをプリントする. 整形仕様の詳細はvprintf()を参照されたい。
 * @param ctx （未使用）
 * @param ch 書き出す文字
 */
void printk(const char *fmt, ...) {
    struct vprintf_context ctx = {.printchar = printchar};
    va_list vargs;
    va_start(vargs, fmt);
    vprintf_with_context(&ctx, fmt, vargs);
    va_end(vargs);
}
