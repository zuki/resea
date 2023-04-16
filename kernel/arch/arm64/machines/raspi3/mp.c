#include <machine/machine.h>
#include <machine/peripherals.h>
#include <printk.h>

#define NUM_CPUS 4

// kernel/arch/arm64/machines/raspi3/boot.Sにある__boot()関数
extern char boot[];

/** @ingroup arm64
 * @brief CPU1-3 (ap) を起動する.
 */
void machine_mp_start(void) {
    for (int cpu = 1; cpu < NUM_CPUS; cpu++) {
        TRACE("Booting CPU #%d...", cpu);

        // https://leiradel.github.io/2019/01/20/Raspberry-Pi-Stubs.html
        // 1. 各CPUはpaddrの内容が変更されるのを待機してwfeしている
        paddr_t paddr = 0xd8 + cpu * 8;
        *((volatile uint32_t *) paddr2ptr(paddr)) = (uint64_t) boot & 0xffffffff;
        // 2. CPUにイベントを送る。イベントが送られたCUPは
        //    paddrの内容(boot())を呼び出す。ここで初めて
        //    cpu1-3はboot.Sのboot関数が実行される
        __asm__ __volatile__("sev");
    }
}
