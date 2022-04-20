# Macでarm64を実行

## config

```
CONFIG_BUILD_DEBUG=y
# CONFIG_BUILD_RELEASE is not set
CONFIG_MODULES=y
CONFIG_BOOT_TASK="vm"
# CONFIG_BENCHMARK_SERVER is not set
# CONFIG_BENCHMARK_SERVER_SERVER is not set
CONFIG_HELLO_SERVER=m
# CONFIG_PRINT_PERIODICALLY is not set
# CONFIG_HELLO_FROM_RUST_SERVER is not set
CONFIG_MEMORY_LEAK_TEST_SERVER=m
# CONFIG_SHM_TEST_SERVER is not set
# CONFIG_TEST_SERVER is not set
# CONFIG_WEBAPI_SERVER is not set
CONFIG_DM_SERVER=y
# CONFIG_EXAMPLE_SERVER is not set
# CONFIG_HV_SERVER is not set
# CONFIG_MINLIN_SERVER is not set
CONFIG_RANDOM_SERVER=y
CONFIG_SHELL_SERVER=y
CONFIG_TCPIP_SERVER=y
# CONFIG_VIRTIO_GPU_SERVER is not set
# CONFIG_PS2_SERVER is not set
CONFIG_E1000_SERVER=y
# CONFIG_VIRTIO_NET_SERVER is not set
CONFIG_RTC_SERVER=y
CONFIG_DATETIME_SERVER=y
# CONFIG_HDAUDIO_SERVER is not set
# CONFIG_FATFS_SERVER is not set
# CONFIG_TARFS_SERVER is not set
# CONFIG_ARCH_X64 is not set
# CONFIG_ARCH_EXAMPLE is not set
CONFIG_ARCH_ARM64=y
CONFIG_ARCH="arm64"
# CONFIG_BOOT_TASK_NOMMU_TEST is not set
CONFIG_BOOT_TASK_VM=y
CONFIG_MACHINE_RASPI3=y
# CONFIG_OPT_LEVEL_0 is not set
CONFIG_OPT_LEVEL_2=y
# CONFIG_OPT_LEVEL_3 is not set
# CONFIG_OPT_LEVEL_S is not set
CONFIG_NUM_SYMBOLS=512
CONFIG_TRACE_EXCEPTION=y
CONFIG_MACHINE="raspi3"
# CONFIG_FORCE_REBOOT_BY_WATCHDOG is not set
CONFIG_SEMIHOSTING=y
# CONFIG_TRACE_IPC is not set
CONFIG_IPC_FASTPATH=y
CONFIG_NUM_TASKS=64
CONFIG_TASK_NAME_LEN=16
CONFIG_TASK_TIME_SLICE_MS=10
CONFIG_KLOG_BUF_SIZE=1024
# CONFIG_ABI_EMU is not set
CONFIG_OOL_BUFFER_LEN=16384
```

## コンパイル

```
$ make clean && make
$ make
       GEN  build/Kconfig.autogen
       GEN  build/include/config.h
       GEN  build/include/idl.h
       GEN  build/libs/libc/targ-include
        CC  kernel/boot.c
        CC  kernel/task.c
        CC  kernel/ipc.c
        CC  kernel/syscall.c
        CC  kernel/printk.c
        CC  kernel/kdebug.c
       GEN  build/vm/__name__.c
        CC  build/vm/__name__.c
        CC  servers/vm/main.c
        CC  servers/vm/task.c
        CC  servers/vm/ool.c
        CC  servers/vm/page_alloc.c
        CC  servers/vm/page_fault.c
        CC  servers/vm/bootfs.c
       GEN  build/datetime/__name__.c
        CC  build/datetime/__name__.c
        CC  servers/experimental/datetime/main.c
        CC  libs/common/string.c
        CC  libs/common/vprintf.c
        CC  libs/common/ubsan.c
        CC  libs/common/bitmap.c
        LD  build/libs/common.lib.o
        AR  build/libs/common.a
        CC  libs/resea/init.c
        CC  libs/resea/printf.c
        CC  libs/resea/malloc.c
        CC  libs/resea/handle.c
        CC  libs/resea/async.c
        CC  libs/resea/task.c
        CC  libs/resea/syscall.c
        CC  libs/resea/ipc.c
        CC  libs/resea/timer.c
        CC  libs/resea/cmdline.c
        CC  libs/resea/datetime.c
        CC  libs/resea/arch/arm64/start.S
        LD  build/libs/resea.lib.o
        AR  build/libs/resea.a
        CC  libs/unittest/dummy.c
        LD  build/libs/unittest.lib.o
        AR  build/libs/unittest.a
        LD  build/datetime.debug.elf
   SYMBOLS  build/datetime.debug.elf
     STRIP  build/datetime.elf
       GEN  build/dm/__name__.c
        CC  build/dm/__name__.c
        CC  servers/dm/main.c
        CC  servers/dm/pci.c
        CC  libs/driver/dma.c
        CC  libs/driver/io.c
        CC  libs/driver/irq.c
        LD  build/libs/driver.lib.o
        AR  build/libs/driver.a
        LD  build/dm.debug.elf
   SYMBOLS  build/dm.debug.elf
     STRIP  build/dm.elf
       GEN  build/e1000/__name__.c
        CC  build/e1000/__name__.c
        CC  servers/drivers/net/e1000/main.c
        CC  servers/drivers/net/e1000/e1000.c
        LD  build/e1000.debug.elf
   SYMBOLS  build/e1000.debug.elf
     STRIP  build/e1000.elf
       GEN  build/hello/__name__.c
        CC  build/hello/__name__.c
        CC  servers/apps/hello/main.c
        LD  build/hello.debug.elf
   SYMBOLS  build/hello.debug.elf
     STRIP  build/hello.elf
       GEN  build/memory_leak_test/__name__.c
        CC  build/memory_leak_test/__name__.c
        CC  servers/apps/memory_leak_test/main.c
        LD  build/memory_leak_test.debug.elf
   SYMBOLS  build/memory_leak_test.debug.elf
     STRIP  build/memory_leak_test.elf
       GEN  build/ramdisk/__name__.c
        CC  build/ramdisk/__name__.c
        CC  servers/drivers/blk/ramdisk/main.c
       GEN  build/ramdisk.img
4098+0 records in
4098+0 records out
4196352 bytes transferred in 0.042025 secs (99853494 bytes/sec)
       GEN  build/servers/drivers/blk/ramdisk/disk.o
        LD  build/ramdisk.debug.elf
   SYMBOLS  build/ramdisk.debug.elf
     STRIP  build/ramdisk.elf
       GEN  build/random/__name__.c
        CC  build/random/__name__.c
        CC  servers/random/main.c
        LD  build/random.debug.elf
   SYMBOLS  build/random.debug.elf
     STRIP  build/random.elf
       GEN  build/rtc/__name__.c
        CC  build/rtc/__name__.c
        CC  servers/drivers/rtc/rtc/main.c
        LD  build/rtc.debug.elf
   SYMBOLS  build/rtc.debug.elf
     STRIP  build/rtc.elf
       GEN  build/shell/__name__.c
        CC  build/shell/__name__.c
        CC  servers/shell/main.c
        CC  servers/shell/commands.c
        CC  servers/shell/http.c
        CC  servers/shell/fs.c
        LD  build/shell.debug.elf
   SYMBOLS  build/shell.debug.elf
     STRIP  build/shell.elf
       GEN  build/tcpip/__name__.c
        CC  build/tcpip/__name__.c
        CC  servers/tcpip/main.c
        CC  servers/tcpip/arp.c
        CC  servers/tcpip/device.c
        CC  servers/tcpip/dhcp.c
        CC  servers/tcpip/ethernet.c
        CC  servers/tcpip/ipv4.c
        CC  servers/tcpip/mbuf.c
        CC  servers/tcpip/tcp.c
        CC  servers/tcpip/udp.c
        CC  servers/tcpip/stats.c
        CC  servers/tcpip/icmp.c
        CC  servers/tcpip/dns.c
        LD  build/tcpip.debug.elf
   SYMBOLS  build/tcpip.debug.elf
     STRIP  build/tcpip.elf
  MKBOOTFS  build/bootfs.bin
        CC  servers/vm/bootfs_image.S
        CC  servers/vm/shm.c
        CC  libs/elf/dummy.c
        LD  build/libs/elf.lib.o
        AR  build/libs/elf.a
        LD  build/vm.debug.elf
   SYMBOLS  build/vm.debug.elf
     STRIP  build/vm.elf
        CC  kernel/arch/arm64/trap.S
        CC  kernel/arch/arm64/init.c
        CC  kernel/arch/arm64/vm.c
        CC  kernel/arch/arm64/mp.c
        CC  kernel/arch/arm64/task.c
        CC  kernel/arch/arm64/switch.S
        CC  kernel/arch/arm64/interrupt.c
        CC  kernel/arch/arm64/usercopy.S
        CC  kernel/arch/arm64/machines/raspi3/boot.S
        CC  kernel/arch/arm64/machines/raspi3/peripherals.c
        CC  kernel/arch/arm64/machines/raspi3/mp.c
        CC  libs/common/string.c
        CC  libs/common/vprintf.c
        CC  libs/common/ubsan.c
        CC  libs/common/bitmap.c
       GEN  build/kernel/__name__.c
        CC  build/kernel/__name__.c
        LD  build/resea.elf
       GEN  build/resea.symbols
   SYMBOLS  build/resea.elf
       GEN  build/compile_commands.json
```

## 実行

```
$ make run
   OBJCOPY  build/kernel8.img
       RUN  build/kernel8.img
qemu-system-aarch64: unsupported machine type
Use -machine help to list supported machines
make: *** [run] Error 1

$ make run V=1
printf "  \\033[1;96m%8s\\033[0m  \\033[1;m%s\\033[0m\\n" "RUN" build/kernel8.img
       RUN  build/kernel8.img
qemu-system-aarch64 -serial mon:stdio -semihosting -d guest_errors,unimp -nographic  -M raspi3 -smp 4 -kernel build/kernel8.img
qemu-system-aarch64: unsupported machine type
Use -machine help to list supported machines
make: *** [run] Error 1

$ qemu-system-aarch64 -machine help
Supported machines are:
...
raspi0               Raspberry Pi Zero (revision 1.2)
raspi1ap             Raspberry Pi A+ (revision 1.1)
raspi2b              Raspberry Pi 2B (revision 1.1)
raspi3ap             Raspberry Pi 3A+ (revision 1.0)
raspi3b              Raspberry Pi 3B (revision 1.2)
...
```

- `kern/arch/arm64/machines/raspi3/build.mk`を修正

```
$ vi kern/arch/arm64/machines/raspi3/build.mk
- QEMUFLAGS += -M raspi3 -smp 4
+ QEMUFLAGS += -M raspi3b -smp 4

$ make
```

- 再実行

```
$ make run
   OBJCOPY  build/kernel8.img
       RUN  build/kernel8.img
bcm2835_property: 0x00038002 set clock rate NYI

Booting Resea v0.9.0 (f6df265)...
[kernel] Booting CPU #1...
[kernel] Booting CPU #2...
[kernel] Booting CPU #3...
[kernel] new task #1: vm (pager=(null))
[kernel] boot ELF: entry=00000000001163a0
[kernel] boot ELF: 0000000000100000 -> 0000000000092000 (5708KiB)
[kernel] boot ELF: 0000000000693000 -> 0000000000625000 (52KiB)
[kernel] boot ELF: 0000000003000000 -> 0000000000000000 (4300KiB, zeroed)
[kernel] new task #0: (idle) (pager=(null))
[kernel] Booted CPU #0
[vm] malloc_init: heap=0000000003000000, len=0x20000
[vm] insert: new_chunk=0x3000000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x3000000, cap=131008, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301ff80
[vm] starting...
[vm] malloc: size=0x20, bin_idx=5
[vm] CHUNK: chunk=0x3000000, cap=130880, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301ff20
[vm] available RAM region #0: 0000000003000000-000000000871e000 (87MiB)
[vm] launching datetime...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=130784, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301eee0
[kernel] new task #2: datetime (pager=vm)
[vm] launching dm...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=126624, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301dea0
[kernel] new task #3: dm (pager=vm)
[vm] launching e1000...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=122464, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301ce60
[kernel] new task #4: e1000 (pager=vm)
[vm] launching ramdisk...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=118304, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301be20
[kernel] new task #5: ramdisk (pager=vm)
[vm] launching random...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=114144, size=0, magic=0xa110ced0a110ced, next=0x0
[kernel] Instruction Abort: task=datetime, far=000000000010b870, elr=000000000010b870, esr=0000000082000004
[kernel] Instruction Abort: task=dm, far=000000000010d218, elr=000000000010d218, esr=0000000082000004
[kernel] Instruction Abort: task=e1000, far=000000000010c7b0, elr=000000000010c7b0, esr=0000000082000004
[kernel] Instruction Abort: task=ramdisk, far=000000000010b620, elr=000000000010b620, esr=0000000082000004
[vm] allocated=0x301ade0
[kernel] new task #6: random (pager=vm)
[vm] launching rtc...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=109984, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3019da0
[kernel] new task #7: rtc (pager=vm)
[vm] launching shell...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=105824, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3018d60
[kernel] new task #8: shell (pager=vm)
[vm] launching tcpip...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=101664, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3017d20
[kernel] new task #9: tcpip (pager=vm)
[vm] ready
[vm] malloc: size=0x4000, bin_idx=14
[vm] CHUNK: chunk=0x3000000, cap=97504, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3013ce0
[kernel] Instruction Abort: task=random, far=000000000010b548, elr=000000000010b548, esr=0000000082000004
[kernel] Instruction Abort: task=rtc, far=000000000010be90, elr=000000000010be90, esr=0000000082000004
[kernel] Instruction Abort: task=shell, far=000000000010d000, elr=000000000010d000, esr=0000000082000004
[kernel] Instruction Abort: task=tcpip, far=000000000011a630, elr=000000000011a630, esr=0000000082000004
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x3000000, cap=81056, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3013c60
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x3000000, cap=80928, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3013be0
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x3000000, cap=-7494024613320454159, size=5946845032915931121, magic=0x528a300ba9412bef, next=0xa9012befb90027e9
[vm] libs/resea/malloc.c:113: ASSERTION FAILURE: chunk->magic == MALLOC_FREE
[vm] WARN: Backtrace:
[vm] WARN:     #0: 000000000010f04c malloc()+0xb08
[vm] WARN:     #1: 0000000000107be8 task_page_alloc()+0x374
[vm] WARN:     #2: 0000000000109730 map_page()+0x100
[vm] WARN:     #3: 0000000000100d60 main()+0x8e0
[vm] WARN:     #4: 000000000010e150 resea_init()+0x50
[vm] WARN:     #5: 00000000001163ac halt()+0x0
QEMU: Terminated
```

### stcとdatetimeを外す

- 結果は同じ。
- 競合が生じていると思わる

```
$ make run
   OBJCOPY  build/kernel8.img
       RUN  build/kernel8.img
bcm2835_property: 0x00038002 set clock rate NYI

Booting Resea v0.9.0 (f6df265)...
[kernel] Booting CPU #1...
[kernel] Booting CPU #2...
[kernel] Booting CPU #3...
[kernel] new task #1: vm (pager=(null))
[kernel] boot ELF: entry=0000000000116418
[kernel] boot ELF: 0000000000100000 -> 0000000000092000 (5432KiB)
[kernel] boot ELF: 000000000064e000 -> 00000000005e0000 (52KiB)
[kernel] boot ELF: 0000000003000000 -> 0000000000000000 (4300KiB, zeroed)
[kernel] new task #0: (idle) (pager=(null))
[kernel] Booted CPU #0
[vm] malloc_init: heap=0000000003000000, len=0x20000
[vm] insert: new_chunk=0x3000000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x3000000, cap=131008, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301ff80
[vm] starting...
[vm] malloc: size=0x20, bin_idx=5
[vm] CHUNK: chunk=0x3000000, cap=130880, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301ff20
[vm] available RAM region #0: 0000000003000000-00000000086d9000 (86MiB)
[vm] launching dm...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=130784, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301eee0
[kernel] new task #2: dm (pager=vm)
[vm] launching e1000...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=126624, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301dea0
[kernel] new task #3: e1000 (pager=vm)
[vm] launching ramdisk...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=122464, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301ce60
[kernel] new task #4: ramdisk (pager=vm)
[vm] launching random...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=118304, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x301be20
[kernel] new task #5: random (pager=vm)
[vm] launching shell...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=114144, size=0, magic=0xa110ced0a110ced, next=0x0
[kernel] Instruction Abort: task=dm, far=000000000010d218, elr=000000000010d218, esr=0000000082000004
[kernel] Instruction Abort: task=e1000, far=000000000010c7b0, elr=000000000010c7b0, esr=0000000082000004
[kernel] Instruction Abort: task=ramdisk, far=000000000010b620, elr=000000000010b620, esr=0000000082000004
[kernel] Instruction Abort: task=random, far=000000000010b548, elr=000000000010b548, esr=0000000082000004
[vm] allocated=0x301ade0
[kernel] new task #6: shell (pager=vm)
[vm] launching tcpip...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x3000000, cap=109984, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3019da0
[kernel] new task #7: tcpip (pager=vm)
[vm] ready
[vm] malloc: size=0x4000, bin_idx=14
[vm] CHUNK: chunk=0x3000000, cap=105824, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3015d60
[kernel] Instruction Abort: task=shell, far=000000000010d000, elr=000000000010d000, esr=0000000082000004
[kernel] Instruction Abort: task=tcpip, far=000000000011a630, elr=000000000011a630, esr=0000000082000004
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x3000000, cap=89376, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3015ce0
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x3000000, cap=89248, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0x3015c60

[vm] map_page: task=1, kpage=0000000003001000           // 競合しているように見える
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000003001000
[vm] malloc: size=0x30, bin_idx=6
                            //   0xB81E43A9F2A00220         0xF81D83AAF2C00000
[vm] CHUNK: chunk=0x3000000, cap=-5179628124230385120, size=-568153207738466304, magic=0xf9001bebf2e00000, next=0xf81e83a1d29c8200
[vm] libs/resea/malloc.c:113: ASSERTION FAILURE: chunk->magic == MALLOC_FREE
[vm] WARN: Backtrace:
[vm] WARN:     #0: 000000000010f0c4 malloc()+0xb08
[vm] WARN:     #1: 0000000000107be8 task_page_alloc()+0x374
[vm] WARN:     #2: 0000000000109788 map_page()+0x158
[vm] WARN:     #3: 0000000000100d60 main()+0x8e0
[vm] WARN:     #4: 000000000010e1c8 resea_init()+0x50
[vm] WARN:     #5: 0000000000116424 halt()+0x0
QEMU: Terminated
```

### 同条件でx86_64で実行

```
Booting Resea v0.9.0 (f6df265)...
[kernel] new task #1: vm (pager=(null))
[kernel] boot ELF: entry=0000000001016610
[kernel] boot ELF: 0000000001000000 -> 0000000000116000 (5336KiB)
[kernel] boot ELF: 0000000003000000 -> 000000000064c000 (36KiB)
[kernel] boot ELF: 0000000004000000 -> 0000000000000000 (137292KiB, zeroed)
[kernel] new task #0: (idle) (pager=(null))
[kernel] Booted CPU #0
[vm] malloc_init: heap=0000000004200000, len=0x8000000
[vm] insert: new_chunk=0x4200000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134217664, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1fff80
[vm] starting...
[vm] malloc: size=0x20, bin_idx=5
[vm] CHUNK: chunk=0x4200000, cap=134217536, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1fff20
[vm] available RAM region #3: 0000000010000000-0000000020698000 (262MiB)
[vm] launching dm...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x4200000, cap=134217440, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1feee0
[kernel] new task #2: dm (pager=vm)
[vm] launching e1000...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x4200000, cap=134213280, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1fdea0
[kernel] new task #3: e1000 (pager=vm)
[vm] launching ramdisk...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x4200000, cap=134209120, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1fce60
[kernel] new task #4: ramdisk (pager=vm)
[vm] launching random...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x4200000, cap=134204960, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1fbe20
[kernel] new task #5: random (pager=vm)
[vm] launching shell...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x4200000, cap=134200800, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1fade0
[kernel] new task #6: shell (pager=vm)
[vm] launching tcpip...
[vm] malloc: size=0x1000, bin_idx=12
[vm] CHUNK: chunk=0x4200000, cap=134196640, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f9da0
[kernel] new task #7: tcpip (pager=vm)
[vm] ready
[vm] malloc: size=0x4000, bin_idx=14
[vm] CHUNK: chunk=0x4200000, cap=134192480, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5d60
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134176032, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5ce0
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134175904, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5c60
[vm] map_page: task=1, kpage=0000000010001000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134175776, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5be0
[vm] map_page: task=1, kpage=0000000010002000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010002000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134175648, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5b60
[vm] map_page: task=2, kpage=0000000010003000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134175520, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5ae0
[vm] map_page: task=2, kpage=0000000010004000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134175392, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5a60
[vm] map_page: task=2, kpage=0000000010005000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134175264, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f59e0
[vm] map_page: task=1, kpage=0000000010006000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010006000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134175136, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5960
[vm] map_page: task=3, kpage=0000000010007000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134175008, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f58e0
[vm] map_page: task=3, kpage=0000000010008000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134174880, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5860
[vm] map_page: task=3, kpage=0000000010009000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134174752, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f57e0
[vm] map_page: task=1, kpage=000000001000a000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=000000001000a000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134174624, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5760
[vm] map_page: task=4, kpage=000000001000b000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134174496, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f56e0
[vm] map_page: task=4, kpage=000000001000c000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134174368, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5660
[vm] map_page: task=4, kpage=000000001000d000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134174240, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f55e0
[vm] map_page: task=1, kpage=000000001000e000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=000000001000e000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134174112, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5560
[vm] map_page: task=5, kpage=000000001000f000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134173984, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f54e0
[vm] map_page: task=5, kpage=0000000010010000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134173856, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5460
[vm] map_page: task=5, kpage=0000000010011000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134173728, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f53e0
[vm] map_page: task=1, kpage=0000000010012000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010012000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134173600, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5360
[vm] map_page: task=6, kpage=0000000010013000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134173472, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f52e0
[vm] map_page: task=6, kpage=0000000010014000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134173344, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5260
[vm] map_page: task=6, kpage=0000000010015000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134173216, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f51e0
[vm] map_page: task=1, kpage=0000000010016000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010016000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134173088, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5160
[vm] map_page: task=7, kpage=0000000010017000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134172960, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f50e0
[vm] map_page: task=7, kpage=0000000010018000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134172832, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f5060
[vm] map_page: task=7, kpage=0000000010019000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134172704, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4fe0
[vm] map_page: task=1, kpage=000000001001a000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=000000001001a000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134172576, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4f60
[vm] map_page: task=2, kpage=000000001001b000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134172448, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4ee0
[vm] map_page: task=1, kpage=000000001001c000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=000000001001c000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134172320, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4e60
[vm] map_page: task=3, kpage=000000001001d000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134172192, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4de0
[vm] map_page: task=1, kpage=000000001001e000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=000000001001e000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134172064, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4d60
[vm] map_page: task=4, kpage=000000001001f000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134171936, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4ce0
[vm] map_page: task=1, kpage=0000000010020000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010020000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134171808, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4c60
[vm] map_page: task=5, kpage=0000000010021000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134171680, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4be0
[vm] map_page: task=1, kpage=0000000010022000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010022000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134171552, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4b60
[vm] map_page: task=6, kpage=0000000010023000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134171424, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4ae0
[vm] map_page: task=1, kpage=0000000010024000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010024000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134171296, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4a60
[vm] map_page: task=7, kpage=0000000010025000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134171168, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f49e0
[vm] map_page: task=1, kpage=0000000010026000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010026000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134171040, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4960
[vm] map_page: task=1, kpage=0000000010027000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010027000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134170912, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f48e0
[vm] map_page: task=1, kpage=0000000010028000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010028000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134170784, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4860
[vm] map_page: task=1, kpage=0000000010029000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010029000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134170656, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f47e0
[vm] map_page: task=1, kpage=000000001002a000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=000000001002a000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134170528, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4760
[vm] map_page: task=1, kpage=000000001002b000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=000000001002b000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134170400, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f46e0
[vm] map_page: task=1, kpage=000000001002c000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=000000001002c000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134170272, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4660
[vm] map_page: task=2, kpage=000000001002d000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134170144, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f45e0
[vm] map_page: task=1, kpage=000000001002e000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=000000001002e000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134170016, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4560
[vm] map_page: task=3, kpage=000000001002f000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134169888, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f44e0
[vm] map_page: task=1, kpage=0000000010030000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010030000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134169760, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4460
[vm] map_page: task=4, kpage=0000000010031000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134169632, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f43e0
[vm] map_page: task=1, kpage=0000000010032000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010032000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134169504, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4360
[vm] map_page: task=5, kpage=0000000010033000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134169376, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f42e0
[vm] map_page: task=1, kpage=0000000010034000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010034000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134169248, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4260
[vm] map_page: task=6, kpage=0000000010035000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134169120, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f41e0
[vm] map_page: task=1, kpage=0000000010036000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010036000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134168992, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4160
[vm] map_page: task=7, kpage=0000000010037000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134168864, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f40e0
[vm] map_page: task=1, kpage=0000000010038000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010038000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134168736, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f4060
[vm] map_page: task=1, kpage=0000000010039000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010039000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134168608, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3fe0
[vm] map_page: task=1, kpage=000000001003a000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=000000001003a000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134168480, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3f60
[vm] map_page: task=1, kpage=000000001003b000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=000000001003b000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134168352, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3ee0
[vm] map_page: task=1, kpage=000000001003c000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=000000001003c000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134168224, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3e60
[vm] map_page: task=1, kpage=000000001003d000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=000000001003d000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134168096, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3de0
[vm] map_page: task=1, kpage=000000001003e000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=000000001003e000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134167968, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3d60
[vm] map_page: task=1, kpage=000000001003f000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=000000001003f000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134167840, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3ce0
[vm] map_page: task=1, kpage=0000000010040000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010040000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134167712, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3c60
[vm] map_page: task=1, kpage=0000000010041000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010041000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134167584, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3be0
[vm] map_page: task=1, kpage=0000000010042000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010042000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134167456, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3b60
[vm] map_page: task=1, kpage=0000000010043000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010043000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134167328, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3ae0
[vm] map_page: task=1, kpage=0000000010044000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010044000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134167200, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3a60
[vm] map_page: task=1, kpage=0000000010045000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010045000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134167072, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f39e0
[vm] map_page: task=1, kpage=0000000010046000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010046000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134166944, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3960
[vm] map_page: task=4, kpage=0000000010047000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134166816, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f38e0
[vm] map_page: task=1, kpage=0000000010048000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010048000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134166688, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3860
[vm] map_page: task=1, kpage=0000000010049000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010049000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134166560, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f37e0
[vm] map_page: task=1, kpage=000000001004a000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=000000001004a000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134166432, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3760
[vm] map_page: task=1, kpage=000000001004b000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=000000001004b000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134166304, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f36e0
[vm] map_page: task=1, kpage=000000001004c000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=000000001004c000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134166176, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3660
[vm] map_page: task=1, kpage=000000001004d000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=000000001004d000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134166048, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f35e0
[vm] map_page: task=1, kpage=000000001004e000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=000000001004e000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134165920, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3560
[vm] map_page: task=1, kpage=000000001004f000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=000000001004f000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134165792, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f34e0
[vm] map_page: task=1, kpage=0000000010050000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010050000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134165664, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3460
[vm] map_page: task=1, kpage=0000000010051000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010051000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134165536, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f33e0
[vm] map_page: task=1, kpage=0000000010052000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010052000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134165408, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3360
[vm] map_page: task=1, kpage=0000000010053000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010053000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134165280, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f32e0
[vm] map_page: task=1, kpage=0000000010054000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010054000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134165152, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3260
[vm] map_page: task=1, kpage=0000000010055000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010055000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134165024, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f31e0
[vm] map_page: task=1, kpage=0000000010056000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010056000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134164896, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3160
[vm] map_page: task=1, kpage=0000000010057000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010057000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134164768, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f30e0
[vm] map_page: task=1, kpage=0000000010058000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010058000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134164640, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f3060
[vm] map_page: task=1, kpage=0000000010059000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010059000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134164512, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2fe0
[vm] map_page: task=1, kpage=000000001005a000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=000000001005a000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134164384, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2f60
[vm] map_page: task=1, kpage=000000001005b000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=000000001005b000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134164256, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2ee0
[vm] map_page: task=1, kpage=000000001005c000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=000000001005c000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134164128, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2e60
[vm] map_page: task=1, kpage=000000001005d000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=000000001005d000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134164000, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2de0
[vm] map_page: task=1, kpage=000000001005e000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=000000001005e000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134163872, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2d60
[vm] map_page: task=1, kpage=000000001005f000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=000000001005f000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134163744, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2ce0
[vm] map_page: task=1, kpage=0000000010060000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010060000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134163616, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2c60
[vm] map_page: task=1, kpage=0000000010061000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010061000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134163488, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2be0
[vm] map_page: task=1, kpage=0000000010062000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010062000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134163360, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2b60
[vm] map_page: task=1, kpage=0000000010063000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010063000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134163232, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2ae0
[vm] map_page: task=1, kpage=0000000010064000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010064000
[e1000] malloc_init: heap=0000000004200000, len=0x8000000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134163104, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2a60
[vm] map_page: task=1, kpage=0000000010065000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010065000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134162976, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f29e0
[vm] map_page: task=1, kpage=0000000010066000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010066000
[random] malloc_init: heap=0000000004200000, len=0x8000000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134162848, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2960
[vm] map_page: task=1, kpage=0000000010067000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010067000
[shell] malloc_init: heap=0000000004200000, len=0x8000000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134162720, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f28e0
[vm] map_page: task=1, kpage=0000000010068000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010068000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134162592, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2860
[vm] map_page: task=1, kpage=0000000010069000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010069000
[dm] malloc_init: heap=0000000004200000, len=0x8000000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134162464, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f27e0
[vm] map_page: task=1, kpage=000000001006a000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=000000001006a000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134162336, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2760
[vm] map_page: task=3, kpage=000000001006b000
[e1000] insert: new_chunk=0x4200000
[e1000] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134162208, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f26e0
[vm] map_page: task=1, kpage=000000001006c000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=000000001006c000
[ramdisk] malloc_init: heap=0000000004200000, len=0x8000000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134162080, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2660
[vm] map_page: task=1, kpage=000000001006d000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=000000001006d000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134161952, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f25e0
[vm] map_page: task=5, kpage=000000001006e000
[random] insert: new_chunk=0x4200000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134161824, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2560
[vm] map_page: task=1, kpage=000000001006f000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=000000001006f000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134161696, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f24e0
[vm] map_page: task=6, kpage=0000000010070000
[shell] insert: new_chunk=0x4200000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134161568, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2460
[vm] map_page: task=1, kpage=0000000010071000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010071000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134161440, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f23e0
[vm] map_page: task=1, kpage=0000000010072000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010072000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134161312, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2360
[vm] map_page: task=2, kpage=0000000010073000
[dm] insert: new_chunk=0x4200000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134161184, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f22e0
[vm] map_page: task=1, kpage=0000000010074000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010074000
[e1000] CHUNK: chunk=0x4200000, cap=134217664, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134161056, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2260
[vm] map_page: task=1, kpage=0000000010075000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010075000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134160928, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f21e0
[vm] map_page: task=4, kpage=0000000010076000
[ramdisk] insert: new_chunk=0x4200000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134160800, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2160
[vm] map_page: task=1, kpage=0000000010077000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010077000
[random] malloc: size=0x30, bin_idx=6
[random] CHUNK: chunk=0x4200000, cap=134217664, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134160672, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f20e0
[vm] map_page: task=1, kpage=0000000010078000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010078000
[shell] malloc: size=0x30, bin_idx=6
[shell] CHUNK: chunk=0x4200000, cap=134217664, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134160544, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f2060
[vm] map_page: task=1, kpage=0000000010079000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010079000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134160416, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1fe0
[vm] map_page: task=1, kpage=000000001007a000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=000000001007a000
[dm] malloc: size=0x30, bin_idx=6
[dm] CHUNK: chunk=0x4200000, cap=134217664, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134160288, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1f60
[vm] map_page: task=1, kpage=000000001007b000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=000000001007b000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134160160, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1ee0
[vm] map_page: task=3, kpage=000000001007c000
[e1000] allocated=0xc1fff80
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134160032, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1e60
[vm] map_page: task=1, kpage=000000001007d000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=000000001007d000
[ramdisk] malloc: size=0x30, bin_idx=6
[ramdisk] CHUNK: chunk=0x4200000, cap=134217664, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134159904, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1de0
[vm] map_page: task=1, kpage=000000001007e000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=000000001007e000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134159776, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1d60
[vm] map_page: task=5, kpage=000000001007f000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134159648, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1ce0
[vm] map_page: task=1, kpage=0000000010080000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010080000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134159520, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1c60
[vm] map_page: task=6, kpage=0000000010081000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134159392, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1be0
[vm] map_page: task=1, kpage=0000000010082000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010082000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134159264, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1b60
[vm] map_page: task=1, kpage=0000000010083000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010083000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134159136, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1ae0
[vm] map_page: task=2, kpage=0000000010084000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134159008, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1a60
[vm] map_page: task=1, kpage=0000000010085000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010085000
[e1000] starting...
[e1000] malloc: size=0x4000, bin_idx=14
[e1000] CHUNK: chunk=0x4200000, cap=134217536, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134158880, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f19e0
[vm] map_page: task=1, kpage=0000000010086000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010086000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134158752, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1960
[vm] map_page: task=4, kpage=0000000010087000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134158624, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f18e0
[vm] map_page: task=1, kpage=0000000010088000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010088000
[random] allocated=0xc1fff80
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134158496, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1860
[vm] map_page: task=1, kpage=0000000010089000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010089000
[shell] allocated=0xc1fff80
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134158368, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f17e0
[vm] map_page: task=1, kpage=000000001008a000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=000000001008a000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134158240, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1760
[vm] map_page: task=1, kpage=000000001008b000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=000000001008b000
[dm] allocated=0xc1fff80
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134158112, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f16e0
[vm] map_page: task=1, kpage=000000001008c000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=000000001008c000
[e1000] allocated=0xc1fbf40
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134157984, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1660
[vm] map_page: task=1, kpage=000000001008d000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=000000001008d000
[ramdisk] allocated=0xc1fff80
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134157856, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f15e0
[vm] map_page: task=1, kpage=000000001008e000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=000000001008e000
[random] starting...
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134157728, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1560
[vm] map_page: task=1, kpage=000000001008f000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=000000001008f000
[shell] starting...
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134157600, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f14e0
[vm] map_page: task=1, kpage=0000000010090000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010090000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134157472, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1460
[vm] map_page: task=1, kpage=0000000010091000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010091000
[dm] starting...
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134157344, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f13e0
[vm] map_page: task=1, kpage=0000000010092000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010092000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134157216, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1360
[vm] map_page: task=1, kpage=0000000010093000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010093000
[random] malloc: size=0x4000, bin_idx=14
[random] CHUNK: chunk=0x4200000, cap=134217536, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134157088, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f12e0
[vm] map_page: task=1, kpage=0000000010094000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010094000
[kernel] enabled IRQ: task=shell, vector=4
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134156960, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1260
[vm] map_page: task=1, kpage=0000000010095000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010095000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134156832, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f11e0
[vm] map_page: task=1, kpage=0000000010096000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=0000000010096000
[dm] malloc: size=0x20, bin_idx=5
[dm] CHUNK: chunk=0x4200000, cap=134217536, size=0, magic=0xa110ced0a110ced, next=0x0
[dm] allocated=0xc1fff20
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134156704, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1160
[vm] map_page: task=1, kpage=0000000010097000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010097000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134156576, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f10e0
[vm] map_page: task=1, kpage=0000000010098000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=0000000010098000
[ramdisk] malloc: size=0x4000, bin_idx=14
[ramdisk] CHUNK: chunk=0x4200000, cap=134217536, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134156448, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f1060
[vm] map_page: task=1, kpage=0000000010099000
[vm] main: task=5, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=5, kpage=0000000010099000
[random] allocated=0xc1fbf40
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134156320, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0fe0
[vm] map_page: task=1, kpage=000000001009a000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=000000001009a000
shell> [vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134156192, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0f60
[vm] map_page: task=1, kpage=000000001009b000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=000000001009b000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134156064, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0ee0
[vm] map_page: task=1, kpage=000000001009c000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=000000001009c000
[dm] malloc: size=0x4000, bin_idx=14
[dm] CHUNK: chunk=0x4200000, cap=134217440, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=000000001009c000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134155936, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0e60
[vm] map_page: task=1, kpage=000000001009d000
[vm] main: task=4, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=4, kpage=000000001009d000
[ramdisk] allocated=0xc1fbf40
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134155808, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0de0
[vm] map_page: task=1, kpage=000000001009e000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=000000001009e000
[shell] malloc: size=0x4000, bin_idx=14
[shell] CHUNK: chunk=0x4200000, cap=134217536, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134155680, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0d60
[vm] map_page: task=1, kpage=000000001009f000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=000000001009f000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134155552, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0ce0
[vm] map_page: task=1, kpage=00000000100a0000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=00000000100a0000
[dm] allocated=0xc1fbee0
[vm] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100a0000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134155424, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0c60
[vm] map_page: task=1, kpage=00000000100a1000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=00000000100a1000
[shell] allocated=0xc1fbf40
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134155296, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0be0
[vm] map_page: task=1, kpage=00000000100a2000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100a2000
[vm] malloc: size=0x40, bin_idx=6
[random] ready
[vm] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134155168, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0b60
[vm] map_page: task=1, kpage=00000000100a2000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134155040, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0ae0
[vm] map_page: task=1, kpage=00000000100a3000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100a3000
[vm] malloc: size=0x40, bin_idx=6
[ramdisk] ready
[vm] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134154912, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0a60
[vm] map_page: task=1, kpage=00000000100a3000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134154784, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f09e0
[vm] map_page: task=1, kpage=00000000100a4000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100a4000
[vm] malloc: size=0x40, bin_idx=6
[dm] ready
[vm] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134154656, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0960
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134154528, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f08e0
[vm] map_page: task=1, kpage=00000000100a5000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100a5000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134154400, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0860
[vm] map_page: task=1, kpage=00000000100a6000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=00000000100a6000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134154272, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f07e0
[vm] map_page: task=1, kpage=00000000100a7000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100a7000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134154144, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0760
[vm] map_page: task=1, kpage=00000000100a8000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=00000000100a8000
[dm] malloc: size=0x30, bin_idx=6
[dm] CHUNK: chunk=0x4200000, cap=134200992, size=0, magic=0xa110ced0a110ced, next=0x0
[dm] allocated=0xc1fbe60
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134154016, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f06e0
[vm] map_page: task=1, kpage=00000000100a9000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100a9000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134153888, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0660
[vm] map_page: task=1, kpage=00000000100aa000
[vm] main: task=2, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=2, kpage=00000000100aa000
[dm] malloc: size=0x10, bin_idx=4
[dm] CHUNK: chunk=0x4200000, cap=134200864, size=0, magic=0xa110ced0a110ced, next=0x0
[dm] allocated=0xc1fbe10
[dm] malloc: size=0x30, bin_idx=6
[dm] CHUNK: chunk=0x4200000, cap=134200784, size=0, magic=0xa110ced0a110ced, next=0x0
[dm] allocated=0xc1fbd90
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134153760, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f05e0
[vm] map_page: task=1, kpage=00000000100ab000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100ab000
[vm] malloc: size=0x20, bin_idx=5
[vm] CHUNK: chunk=0x4200000, cap=134153632, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0580
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134153536, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0500
[vm] map_page: task=1, kpage=00000000100ac000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100ac000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134153408, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0480
[vm] map_page: task=1, kpage=00000000100ad000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100ad000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134153280, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0400
[vm] map_page: task=1, kpage=00000000100ae000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100ae000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134153152, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0380
[vm] map_page: task=1, kpage=00000000100af000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100af000
[kernel] enabled IRQ: task=e1000, vector=11
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134153024, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0300
[vm] map_page: task=1, kpage=00000000100b0000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100b0000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134152896, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0280
[vm] map_page: task=1, kpage=00000000100b1000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b1000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134152768, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0200
[vm] map_page: task=1, kpage=00000000100b2000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100b2000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b2000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134152640, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0180
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100b3000
[vm] malloc: size=0x30, bin_idx=6
[e1000] malloc: size=0x20, bin_idx=5
[e1000] CHUNK: chunk=0x4200000, cap=134201088, size=0, magic=0xa110ced0a110ced, next=0x0
[e1000] allocated=0xc1fbee0
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134152512, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0100
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134152384, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0080
[vm] map_page: task=1, kpage=00000000100b4000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100b4000
[vm] malloc: size=0x30, bin_idx=6
[e1000] malloc: size=0x10, bin_idx=4
[e1000] CHUNK: chunk=0x4200000, cap=134200992, size=0, magic=0xa110ced0a110ced, next=0x0
[e1000] allocated=0xc1fbe90
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134152256, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1f0000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134152128, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eff80
[vm] map_page: task=1, kpage=00000000100b6000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100b6000
[vm] malloc: size=0x30, bin_idx=6
[e1000] malloc: size=0x10, bin_idx=4
[e1000] CHUNK: chunk=0x4200000, cap=134200912, size=0, magic=0xa110ced0a110ced, next=0x0
[e1000] allocated=0xc1fbe40
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134152000, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eff00
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134151872, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efe80
[vm] map_page: task=1, kpage=00000000100b8000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100b8000
[vm] malloc: size=0x30, bin_idx=6
[e1000] malloc: size=0x10, bin_idx=4
[e1000] CHUNK: chunk=0x4200000, cap=134200832, size=0, magic=0xa110ced0a110ced, next=0x0
[e1000] allocated=0xc1fbdf0
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134151744, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efe00
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134151616, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efd80
[vm] map_page: task=1, kpage=00000000100c9000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100c9000
[vm] malloc: size=0x30, bin_idx=6
[e1000] malloc: size=0x10, bin_idx=4
[e1000] CHUNK: chunk=0x4200000, cap=134200752, size=0, magic=0xa110ced0a110ced, next=0x0
[e1000] allocated=0xc1fbda0
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134151488, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efd00
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134151360, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efc80
[vm] map_page: task=1, kpage=00000000100da000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100da000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100da000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134151232, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efc00
[vm] map_page: task=3, kpage=00000000100db000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134151104, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efb80
[vm] map_page: task=1, kpage=00000000100dc000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100dc000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100dc000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134150976, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efb00
[vm] map_page: task=1, kpage=00000000100dd000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100dd000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100dd000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134150848, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efa80
[vm] map_page: task=1, kpage=00000000100de000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100de000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100de000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134150720, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1efa00
[vm] map_page: task=1, kpage=00000000100df000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100df000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100df000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134150592, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef980
[vm] map_page: task=1, kpage=00000000100e0000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100e0000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100e0000
[e1000] initialized the device
[e1000] MAC address = 52:54:00:12:34:56
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134150464, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef900
[vm] map_page: task=1, kpage=00000000100e1000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100e1000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100e1000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134150336, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef880
[vm] map_page: task=1, kpage=00000000100e2000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100e2000
[vm] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134150208, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef800
[vm] map_page: task=1, kpage=00000000100e3000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100e3000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134150080, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef780
[vm] map_page: task=1, kpage=00000000100e4000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100e4000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134149952, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef700
[vm] map_page: task=1, kpage=00000000100e5000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100e5000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134149824, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef680
[vm] map_page: task=1, kpage=00000000100e6000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100e6000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134149696, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef600
[vm] map_page: task=1, kpage=00000000100e7000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100e7000
[tcpip] malloc_init: heap=0000000004200000, len=0x8000000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134149568, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef580
[vm] map_page: task=1, kpage=00000000100e8000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100e8000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134149440, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef500
[vm] map_page: task=7, kpage=00000000100e9000
[tcpip] insert: new_chunk=0x4200000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134149312, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef480
[vm] map_page: task=1, kpage=00000000100ea000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100ea000
[tcpip] malloc: size=0x30, bin_idx=6
[tcpip] CHUNK: chunk=0x4200000, cap=134217664, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134149184, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef400
[vm] map_page: task=1, kpage=00000000100eb000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100eb000
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134149056, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef380
[vm] map_page: task=7, kpage=00000000100ec000
[tcpip] allocated=0xc1fff80
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134148928, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef300
[vm] map_page: task=1, kpage=00000000100ed000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100ed000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134148800, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef280
[vm] map_page: task=1, kpage=00000000100ee000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100ee000
[tcpip] starting...
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134148672, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef200
[vm] map_page: task=1, kpage=00000000100ef000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100ef000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134148544, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef180
[vm] map_page: task=1, kpage=00000000100f0000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f0000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134148416, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef100
[vm] map_page: task=1, kpage=00000000100f1000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f1000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134148288, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef080
[vm] map_page: task=1, kpage=00000000100f2000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f2000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134148160, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ef000
[vm] map_page: task=1, kpage=00000000100f3000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f3000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134148032, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eef80
[vm] map_page: task=1, kpage=00000000100f4000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f4000
[tcpip] malloc: size=0x4000, bin_idx=14
[tcpip] CHUNK: chunk=0x4200000, cap=134217536, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134147904, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eef00
[vm] map_page: task=1, kpage=00000000100f5000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f5000
[tcpip] allocated=0xc1fbf40
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100f5000
[vm] malloc: size=0x40, bin_idx=6
[tcpip] ready
[vm] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134147776, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eee80
[vm] map_page: task=1, kpage=00000000100f5000
[vm] malloc: size=0x40, bin_idx=6
[vm] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134147648, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eee00
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134147520, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eed80
[vm] map_page: task=1, kpage=00000000100f6000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f6000
[tcpip] malloc: size=0x40, bin_idx=6
[tcpip] CHUNK: chunk=0x4200000, cap=134201088, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fbec0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134147392, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eed00
[vm] map_page: task=1, kpage=00000000100f7000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f7000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134147264, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eec80
[vm] map_page: task=1, kpage=00000000100f8000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f8000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134147136, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eec00
[vm] map_page: task=1, kpage=00000000100f9000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100f9000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134147008, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eeb80
[vm] map_page: task=1, kpage=00000000100fa000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100fa000
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] CHUNK: chunk=0x4200000, cap=134200960, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fba80
[tcpip] malloc: size=0x30, bin_idx=6
[tcpip] CHUNK: chunk=0x4200000, cap=134199872, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fba00
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] CHUNK: chunk=0x4200000, cap=134199744, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fb5c0
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] CHUNK: chunk=0x4200000, cap=134198656, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fb180
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] CHUNK: chunk=0x4200000, cap=134197568, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134146880, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eeb00
[vm] map_page: task=1, kpage=00000000100fb000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100fb000
[tcpip] allocated=0xc1fad40
[tcpip] malloc: size=0x130, bin_idx=9
[tcpip] CHUNK: chunk=0x4200000, cap=134196480, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fab00
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134146752, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eea80
[vm] map_page: task=1, kpage=00000000100fc000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100fc000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134146624, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1eea00
[vm] map_page: task=1, kpage=00000000100fd000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=00000000100fd000
[tcpip] malloc: size=0x10, bin_idx=4
[tcpip] CHUNK: chunk=0x4200000, cap=134195904, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1faab0
[tcpip] malloc: size=0x120, bin_idx=9
[tcpip] CHUNK: chunk=0x4200000, cap=134195824, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fa870
[tcpip] registered new net device 'net1'
[e1000] ready
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134146496, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ee980
[vm] map_page: task=1, kpage=00000000100fe000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100fe000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100fe000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100fe000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100fe000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134146368, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ee900
[vm] map_page: task=1, kpage=00000000100ff000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100ff000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100ff000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=00000000100ff000
[e1000] sent 302 bytes
[e1000] malloc: size=0x4000, bin_idx=14
[tcpip] WARN: retrying DHCP discover...
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x30, bin_idx=6
[tcpip] CHUNK: chunk=0x4200000, cap=134195248, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fa7f0
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x130, bin_idx=9
[tcpip] malloc: size=0x120, bin_idx=9
[tcpip] CHUNK: chunk=0x4200000, cap=134195120, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fa5b0
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100ff000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100ff000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100ff000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100ff000
[e1000] sent 302 bytes
[e1000] malloc: size=0x4000, bin_idx=14
[e1000] received 316 bytes
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100ff000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100ff000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=00000000100ff000
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134146240, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ee880
[vm] map_page: task=1, kpage=0000000010100000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010100000
[e1000] received 316 bytes
[tcpip] malloc: size=0x210, bin_idx=10
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010100000
[tcpip] malloc: size=0x30, bin_idx=6
[tcpip] CHUNK: chunk=0x4200000, cap=134194544, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fa530
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x30, bin_idx=6
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] CHUNK: chunk=0x4200000, cap=134194416, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1fa0f0
[tcpip] malloc: size=0x140, bin_idx=9
[tcpip] malloc: size=0x120, bin_idx=9
[tcpip] CHUNK: chunk=0x4200000, cap=134193328, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134146112, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ee800
[vm] map_page: task=1, kpage=0000000010101000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010101000
[tcpip] allocated=0xc1f9eb0
[tcpip] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x30, bin_idx=6
[tcpip] CHUNK: chunk=0x4200000, cap=134192752, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1f9e30
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x30, bin_idx=6
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] CHUNK: chunk=0x4200000, cap=134192624, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1f99f0
[tcpip] malloc: size=0x140, bin_idx=9
[tcpip] CHUNK: chunk=0x4200000, cap=134191536, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1f97b0
[tcpip] malloc: size=0x120, bin_idx=9
[tcpip] CHUNK: chunk=0x4200000, cap=134190960, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1f9570
[tcpip] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] main: task=3, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=3, kpage=0000000010101000
[e1000] sent 306 bytes
[e1000] malloc: size=0x4000, bin_idx=14
[e1000] received 316 bytes
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010101000
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x30, bin_idx=6
[tcpip] CHUNK: chunk=0x4200000, cap=134190384, size=0, magic=0xa110ced0a110ced, next=0x0
[tcpip] allocated=0xc1f94f0
[vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134145984, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ee780
[vm] map_page: task=1, kpage=0000000010102000
[vm] main: task=7, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=7, kpage=0000000010102000
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[tcpip] malloc: size=0x4000, bin_idx=14
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010102000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010102000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010102000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010102000
[e1000] sent 306 bytes
[e1000] malloc: size=0x4000, bin_idx=14
[e1000] received 316 bytes
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010102000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010102000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010102000
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=1, kpage=0000000010102000
[tcpip] malloc: size=0x210, bin_idx=10
[tcpip] malloc: size=0x30, bin_idx=6
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[tcpip] malloc: size=0x4000, bin_idx=14

[shell] malloc: size=0x40, bin_idx=6
[shell] CHUNK: chunk=0x4200000, cap=134201088, size=0, magic=0xa110ced0a110ced, next=0x0
[shell] allocated=0xc1fbec0
shell> [vm] malloc: size=0x30, bin_idx=6
[vm] malloc: size=0x30, bin_idx=6
[vm] CHUNK: chunk=0x4200000, cap=134145856, size=0, magic=0xa110ced0a110ced, next=0x0
[vm] allocated=0xc1ee700
[vm] map_page: task=1, kpage=0000000010103000
[vm] main: task=6, pager=1
[vm] malloc: size=0x30, bin_idx=6
[vm] map_page: task=6, kpage=0000000010103000

[shell] malloc: size=0x40, bin_idx=6
[shell] CHUNK: chunk=0x4200000, cap=134200960, size=0, magic=0xa110ced0a110ced, next=0x0
[shell] allocated=0xc1fbe40
shell> QEMU: Terminated
```
