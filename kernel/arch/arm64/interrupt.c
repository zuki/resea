#include "asm.h"
#include <config.h>
#include <machine/peripherals.h>
#include <printk.h>
#include <syscall.h>
#include <task.h>
#include <types.h>

// usercopy.S で定義
extern char arm64_usercopy1[];
extern char arm64_usercopy2[];
extern char arm64_usercopy3[];  // これは未定義

/** @ingroup arm64
 * @brief ARM64固有の割り込みハンドラ.
 */
void arm64_handle_interrupt(void) {
    arm64_timer_reload();
    handle_timer_irq();
}

/** @ingroup arm64
 * @brief ARM64固有の例外ハンドラ.
 */
void arm64_handle_exception(void) {
    uint64_t esr = ARM64_MRS(esr_el1);      // 例外診断情報
    uint64_t elr = ARM64_MRS(elr_el1);      // 例外リターンアドレス
    uint64_t far = ARM64_MRS(far_el1);      // 例外発生アドレス
    unsigned ec = esr >> 26;                // 例外理由
    switch (ec) {
        // SVC.
        case 0x15:
            // exception_common (trap.S) で処理される
            UNREACHABLE();
            break;
        // ユーザ空間でのInstruction Abort (ページフォルト)
        case 0x20:
#ifdef CONFIG_TRACE_EXCEPTION
            TRACE("Instruction Abort: task=%s, far=%p, elr=%p, esr=%p",
                  CURRENT->name, far, elr, esr);
#endif
            handle_page_fault(far, elr,
                              EXP_PF_USER | EXP_PF_WRITE /* FIXME: */);
            break;
        // ユーザ空間でのData Abort (ページフォルト)
        case 0x24:
#ifdef CONFIG_TRACE_EXCEPTION
            TRACE("Data Abort: task=%s, far=%p, elr=%p, esr=%p", CURRENT->name,
                  far, elr, esr);
#endif
            handle_page_fault(far, elr,
                              EXP_PF_USER | EXP_PF_WRITE /* FIXME: */);
            break;
        // カーネルでのData Abort
        case 0x25:
#ifdef CONFIG_TRACE_EXCEPTION
            TRACE("Data Abort (kernel): far=%p, elr=%p", far, elr);
#endif
            // ユーザ空間との間のコピー以外で発生した場合はpanic
            if (elr != (vaddr_t) arm64_usercopy1
                && elr != (vaddr_t) arm64_usercopy2) {
                PANIC("page fault in the kernel: task=%s, far=%p, elr=%p",
                      CURRENT->name, far, elr);
            }
            // ユーザ空間との間のコピーでページフォルト
            handle_page_fault(far, elr,
                              EXP_PF_USER | EXP_PF_WRITE /* FIXME: */);
            break;
        default:
            PANIC("unknown exception: ec=%d (0x%x), elr=%p, far=%p", ec, ec,
                  elr, far);
    }
}
