/** @file arch.h */
#ifndef __ARCH_H__
#define __ARCH_H__

#include "asm.h"
#include <machine/machine.h>
#include <types.h>

#define STACK_SIZE        4096
#define TICK_HZ           1000
#define IRQ_MAX           64
#define KERNEL_BASE_ADDR  0xffff000000000000
#define STRAIGHT_MAP_ADDR 0x03000000            // 未使用
#define STRAIGHT_MAP_END  0x3f000000            // 未使用

/** @ingroup arm64
 * @struct arch_task
 * @brief ARM64固有のタスク構造体.
 */
struct arch_task {
    vaddr_t syscall_stack;      /**< システムコール用のスタックポインタ */
    vaddr_t stack;              /**< タスク用のスタックポインタ */
    uint64_t *page_table;       /**< レベル0のページテーブル*/
    paddr_t ttbr0;              /**< ユーザ用ページテーブルの物理アドレス */
};

/** @ingroup arm64
 * @brief 物理アドレスから仮想アドレスに変換する.
 * @param addr 物理アドレス
 * @return 仮想アドレス
 */
static inline void *paddr2ptr(paddr_t addr) {
    return (void *) (addr + KERNEL_BASE_ADDR);
}

/** @ingroup arm64
 * @brief 仮想アドレスから物理アドレスに変換する.
 * @param addr 仮想アドレス
 * @return 物理アドレス
 */
static inline paddr_t ptr2paddr(void *addr) {
    return ((vaddr_t) addr - KERNEL_BASE_ADDR);
}

/** @ingroup arm64
 * @brief 指定の仮想アドレス範囲がカーネル空間にあるかチェックする.
 * @param base 開始アドレス
 * @param len 長さ
 * @return 範囲内にあれば true; そうでなければ false
 */
static inline bool is_kernel_addr_range(vaddr_t base, size_t len) {
    // 最初の式は整数のオーバーフローが生じていないかチェックしている
    return base + len < len || base >= KERNEL_BASE_ADDR
           || base + len >= KERNEL_BASE_ADDR;
}

// これらのシンボルは*物理*アドレスを指していることに注意
extern char __kernel_image[];
extern char __kernel_image_end[];

/** @ingroup arm64
 * @brief 指定の物理アドレスがカーネルアドレスであるかをチェックする.
 * @param pbase 物理アドレス
 * @param len 長さ
 * @return カーネルアドレスであれば true; そうでなければ false
 */
static inline bool is_kernel_paddr(paddr_t paddr) {
    return (paddr_t) __kernel_image <= paddr
           && paddr <= (paddr_t) __kernel_image_end;
}

/** @ingroup arm64
 * @brief カレントタスクを実行しているコア番号を返す.
 * @return コア番号
 */
static inline int mp_self(void) {
    return ARM64_MRS(mpidr_el1) & 0xff;
}

/** @ingroup arm64
 * @brief カレントタスクの実行コアがbspであるかをチェックする.
 * @return bspなら true; apなら false
 */
static inline bool mp_is_bsp(void) {
    return mp_self() == 0;
}

struct arch_cpuvar {};

struct cpuvar *arm64_get_cpuvar(void);

/** @ingroup arm64
 * @brief カレントタスクの実行コアのローカル情報を取得する.
 * @return ローカル情報へのポインタ
 */
static inline struct cpuvar *get_cpuvar(void) {
    return arm64_get_cpuvar();
}

#endif
