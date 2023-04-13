# kernel/arch/arm64/machines/raspi3/mp.c

```c
#include <machine/machine.h>
#include <machine/peripherals.h>
#include <printk.h>

#define NUM_CPUS 4

extern char boot[];     // in kernel/arch/arm64/machines/raspi3/boot.S

/// ap (cpu1-3)を起動: 各CPUはアドレス（0xd8 + cpu * 8）が0以外になるのを待ってwfeしている
void machine_mp_start(void) {
    for (int cpu = 1; cpu < NUM_CPUS; cpu++) {
        TRACE("Booting CPU #%d...", cpu);

        // https://leiradel.github.io/2019/01/20/Raspberry-Pi-Stubs.htmlはarmstubのコード解説
        paddr_t paddr = 0xd8 + cpu * 8;
        *((volatile uint32_t *) paddr2ptr(paddr)) = (uint64_t) boot & 0xffffffff;   // bootアドレスを設定

        __asm__ __volatile__("sev");    // イベント発生
    }
}
```

##  https://github.com/raspberrypi/tools/blob/master/armstubs/armstub8.S

```
mrs x6, MPIDR_EL1
	and x6, x6, #0x3
	cbz x6, primary_cpu         // cpu=0 (bsp)ならprimary_cpuへ分岐

	adr x5, spin_cpu0           // x4 = 0x08
secondary_spin:
	wfe                         // イベントが来るまでwait
	ldr x4, [x5, x6, lsl #3]    // x4 = 0x08 + cpu * 8
	cbz x4, secondary_spin      // if x4 == 0 ならループ
	mov x0, #0
	b boot_kernel               // x3に呼び出しアドレスがセットされたらboot_kernelへ分岐

primary_cpu:
	ldr w4, kernel_entry32
	ldr w0, dtb_ptr32

boot_kernel:
	mov x1, #0
	mov x2, #0
	mov x3, #0
	br x4                       // x4を呼び出す

.ltorg

.org 0xd8
.globl spin_cpu0
spin_cpu0:
	.quad 0
.org 0xe0
.globl spin_cpu1
spin_cpu1:
	.quad 0
.org 0xe8
.globl spin_cpu2
spin_cpu2:
	.quad 0
.org 0xf0
.globl spin_cpu3
spin_cpu3:
	# Shared with next two symbols/.word
	# FW clears the next 8 bytes after reading the initial value, leaving
	# the location suitable for use as spin_cpu3
.org 0xf0
.globl dtb_ptr32            // dtb_ptr32とkernel_entry32はGPUが設定する
dtb_ptr32:
	.word 0x0
.org 0xfc
.globl kernel_entry32
kernel_entry32:
	.word 0x0
``
