#include "kdebug.h"
#include "ipc.h"
#include "printk.h"
#include "task.h"
#include <string.h>

/** @ingroup kernel
 * @brief kdebugを十個する.
 * @param cmdline コマンド
 * @param buf バッファ
 * @param len バッファ長
 * @return 正常終了したら OK; 総出なければ ::error_t 型のエラーコード
 */
error_t kdebug_run(const char *cmdline, char *buf, size_t len) {
    // コマンドが指定されていない場合は、単に無視する。
    if (strlen(cmdline) == 0) {
        return OK;
    } else if (strcmp(cmdline, "help") == 0) {
        INFO("Kernel debugger commands:");
        INFO("");
        INFO("  ps - List tasks.");
        INFO("  q  - Quit the emulator.");
        INFO("");
    } else if (strcmp(cmdline, "ps") == 0) {
        task_dump();
    } else if (strcmp(cmdline, "q") == 0) {
#ifdef CONFIG_SEMIHOSTING
        arch_semihosting_halt();
#endif
        PANIC("halted by the kdebug");
    //  klogをbufに書き込む
    } else if (strcmp(cmdline, "_log") == 0) {
        if (!len) {
            return ERR_TOO_SMALL;
        }

        int read_len = klog_read(buf, len - 1);
        buf[read_len - 1] = '\0';
    } else {
        WARN("Invalid debugger command: '%s'.", cmdline);
        return ERR_NOT_FOUND;
    }

    return OK;
}

/** @ingroup kernel
 * @brief スタックキャナリのアドレスを取得する.
 * @return スタックキャナリのアドレス
 */
static uint32_t *get_canary_ptr(void) {
    vaddr_t sp = (vaddr_t) __builtin_frame_address(0);
    return (uint32_t *) ALIGN_DOWN(sp, STACK_SIZE);
}

/** @ingroup kernel
 * @brief カレントカーネルスタックの底にスタックキャナリを書き込む.
 */
void stack_set_canary(void) {
    *get_canary_ptr() = STACK_CANARY_VALUE;
}

/** @ingroup kernel
 * @brief カーネルスタックキャナリが変更されていないかチェックする.
 *      変更されていたら panic.
 */
void stack_check(void) {
    if (*get_canary_ptr() != STACK_CANARY_VALUE) {
        PANIC("the kernel stack has been exhausted");
    }
}
