#ifndef __ARCH_H__
#define __ARCH_H__

#include "asm.h"
#include <machine/machine.h>
#include <types.h>

#define STACK_SIZE        4096
#define TICK_HZ           1000
#define IRQ_MAX           32
#define KERNEL_BASE_ADDR  0xffff000000000000
#define STRAIGHT_MAP_ADDR 0x00000000            // 未使用
#define STRAIGHT_MAP_END  0x3f000000            // 未使用

struct arch_task {
    vaddr_t syscall_stack;
    vaddr_t stack;
    /// The level-0 page table.
    uint64_t *page_table;
    /// The user's page table paddr.
    paddr_t ttbr0;
};

/// 物理アドレスから仮想アドレスへの変換
static inline void *paddr2ptr(paddr_t addr) {
    return (void *) (addr + KERNEL_BASE_ADDR);
}


/// 仮想アドレスから物理アドレスへの変換
static inline paddr_t ptr2paddr(void *addr) {
    return ((vaddr_t) addr - KERNEL_BASE_ADDR);
}

/// baseとbase+lenがカーネルアドレス範囲内にあるかチェック
static inline bool is_kernel_addr_range(vaddr_t base, size_t len) {
    // 最初の式は整数のオーバーフローが生じていないかチェックしている
    return base + len < len || base >= KERNEL_BASE_ADDR
           || base + len >= KERNEL_BASE_ADDR;
}

// これらのシンボルは*物理*アドレスを指していることに注意
extern char __kernel_image[];
extern char __kernel_image_end[];

/// paddrがカーネルアドレスであるかをチェック
static inline bool is_kernel_paddr(paddr_t paddr) {
    return (paddr_t) __kernel_image <= paddr
           && paddr <= (paddr_t) __kernel_image_end;
}


/// カレントタスクを実行しているCPU番号を返す
static inline int mp_self(void) {
    return ARM64_MRS(mpidr_el1) & 0xff;
}

/// カレントタスクの実行CPUがbspであるかをチェック
static inline bool mp_is_bsp(void) {
    return mp_self() == 0;
}

struct arch_cpuvar {};

struct cpuvar *arm64_get_cpuvar(void);
static inline struct cpuvar *get_cpuvar(void) {
    return arm64_get_cpuvar();
}

#endif
