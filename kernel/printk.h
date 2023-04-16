/** @file printk.h */
#ifndef __PRINTK_H__
#define __PRINTK_H__

#include <config.h>
#include <print_macros.h>
#include <types.h>

/** @ingroup kernel
 * @struct klog
 * @brief カーネルログ(klog)バッファ構造体.
 */
struct klog {
    char buf[CONFIG_KLOG_BUF_SIZE]; /**< バッファ: リングバッファ */
    size_t head;                    /**< 先頭（書き込し）ポインタ */
    size_t tail;                    /**< 末尾（前回最後に読み込んだ）ポインタ */
};

void klog_write(char ch);
size_t klog_read(char *buf, size_t buf_len);
void printk(const char *fmt, ...);

// Implemented in arch.
void arch_printchar(char ch);

#endif
