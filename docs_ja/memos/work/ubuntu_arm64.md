# Ubuntuでarm64版を実行

## ビルド

```bash
$ make menuconfig
$ make build
```

### エラー1:

arm64の`.align`は2のべき乗を指定する。バイト数で指定する場合は `.barlign`を使う

```bash
$ make build
       GEN  build/servers/drivers/blk/ramdisk/disk.o
build/servers/drivers/blk/ramdisk/disk.S:1:17: error: invalid alignment value
.rodata; .align 4096; .global __image, __image_end; __image: .incbin "build/ramdisk.img"; __image_end:
                ^
make: *** [servers/drivers/blk/ramdisk/build.mk:10: build/servers/drivers/blk/ramdisk/disk.o] Error 1

    $ vi servers/drivers/blk/ramdisk/build.mk
    $ diff
    -       echo ".rodata; .align 4096; .global __image, __image_end; __image: .incbin \"$<\"; __image_end:" > $(@:.o=.S)
    +       echo ".rodata; .balign 4096; .global __image, __image_end; __image: .incbin \"$<\"; __image_end:" > $(@:.o=.S)
```

### エラー2:

.rodata量いいと.bss領域が重なる。.rodataに大きなバイナリデータを埋め込んでいるため。
.bssの開始アドレスを後ろにずらす。

```bash
 $ make build
        LD  build/ramdisk.debug.elf
ld.lld: error: section .rodata virtual address range overlaps with .bss
>>> .rodata range is [0x10C000, 0x51660F]       # ramdisk.img = 0x400000
>>> .bss range is [0x300000, 0x32039F]

    # cat servers/blk/ramdisk/build.mk
    $(BUILD_DIR)/servers/drivers/blk/ramdisk/disk.o: $(RAM_DISK_IMG)
            $(PROGRESS) "GEN" $@
            echo ".rodata; .balign 4096; .global __image, __image_end; __image: .incbin \"$<\"; __image_end:" > $(@:.o=.S)
            $(CC) $(CFLAGS) -o $@ -c $(@:.o=.S)

    $(RAM_DISK_IMG):
            $(PROGRESS) "GEN" $@
            mkdir -p $(@D)
            dd if=/dev/zero of=$@.tmp bs=1024 count=4096    # 4MiB = 0x400000
            mformat -i $@.tmp
            echo "Hello from FAT :D" > $(BUILD_DIR)/hello.txt
            mcopy -i $@.tmp $(BUILD_DIR)/hello.txt ::/HELLO.TXT
            mv $@.tmp $@
```

```bash
        LD  build/vm.debug.elf
ld.lld: error: section .rodata virtual address range overlaps with .bss
>>> .rodata range is [0x117000, 0x616EEF]       # bootfs.bin = 0x4f4000
>>> .bss range is [0x600000, 0xA326CF]

    # BOOTFS_PATH = build/bootfs.bin
    # ls -l build/bootfs.bin => 5193728 (ox4f4000)
    # cat servers/vm/bootfs_images.S
    .rodata
    .balign 4096
    .global __bootfs, __bootfs_end
    __bootfs:
        .incbin BOOTFS_PATH
    __bootfs_end:
```

```bash
    $ vi libs/resea/arch/arm64/user.ld
    $ diff
        __cmdline = 0x02fff000;
    -    . = 0x00300000;            # .bssの開始アドレスを重ならないよう後ろにずらした
    +    . = 0x00800000;
```

### エラー3:

`CONSOLE_IRQ`がarm64では未定義。arm64ではuartでFIFOを使わず、割り込みも使わないので
`ASSERT_OK`を削除

```bash
$ make clean && make build
        CC  servers/shell/main.c
servers/shell/main.c:142:27: error: use of undeclared identifier 'CONSOLE_IRQ'
    ASSERT_OK(irq_acquire(CONSOLE_IRQ));
                          ^
servers/shell/main.c:142:27: error: use of undeclared identifier 'CONSOLE_IRQ'
2 errors generated.
make: *** [Makefile:278: build/servers/shell/main.o] Error 1

$ vi kernel/arch/arm64/machines/raspi3/peripherals.c
    $ diff
    +/*
        if (!kdebug_is_readable()) {
            return -1;
        }
    -
    +*/
    +    while (mmio_read(UART0_FR) & (1 << 4))
    +       ;
    $ vi servers/shell/main.c
    $ diff
    -    ASSERT_OK(irq_acquire(CONSOLE_IRQ));
    +    //ASSERT_OK(irq_acquire(CONSOLE_IRQ));
```

### ビルド成功

```bash
$ make clean && make build
$ make build
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
       GEN  build/dm/__name__.c
        CC  build/dm/__name__.c
        CC  servers/dm/main.c
        CC  servers/dm/pci.c
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
        CC  libs/driver/dma.c
        CC  libs/driver/io.c
        CC  libs/driver/irq.c
        LD  build/libs/driver.lib.o
        AR  build/libs/driver.a
        LD  build/dm.debug.elf
   SYMBOLS  build/dm.debug.elf
     STRIP  build/dm.elf
       GEN  build/example/__name__.c
        CC  build/example/__name__.c
        CC  servers/example/main.c
        LD  build/example.debug.elf
   SYMBOLS  build/example.debug.elf
     STRIP  build/example.elf
       GEN  build/fatfs/__name__.c
        CC  build/fatfs/__name__.c
        CC  servers/fs/fatfs/main.c
        CC  servers/fs/fatfs/fat.c
        LD  build/fatfs.debug.elf
   SYMBOLS  build/fatfs.debug.elf
     STRIP  build/fatfs.elf
       GEN  build/hello/__name__.c
        CC  build/hello/__name__.c
        CC  servers/apps/hello/main.c
        LD  build/hello.debug.elf
   SYMBOLS  build/hello.debug.elf
     STRIP  build/hello.elf
       GEN  build/ramdisk/__name__.c
        CC  build/ramdisk/__name__.c
        CC  servers/drivers/blk/ramdisk/main.c
       GEN  build/ramdisk.img
4096+0 records in
4096+0 records out
4194304 bytes (4.2 MB, 4.0 MiB) copied, 0.0139565 s, 301 MB/s
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
       GEN  build/shell/__name__.c
        CC  build/shell/__name__.c
        CC  servers/shell/main.c
        CC  servers/shell/commands.c
        CC  servers/shell/http.c
        CC  servers/shell/fs.c
        LD  build/shell.debug.elf
   SYMBOLS  build/shell.debug.elf
     STRIP  build/shell.elf
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

## QEMUで実行

```bash
$ vi kernel/arch/arm64/machines/raspi3/build.mk
-QEMUFLAGS += -M raspi3 -smp 4
+QEMUFLAGS += -M raspi3b -smp 4

$ make run

[kernel] Data Abort: task=fatfs, far=000000000081c000, elr=00000000001048d4, esr=0000000092000007
[fatfs] PANIC: failed to locate a FAT file system
[fatfs] WARN: Backtrace:
[kernel] Data Abort: task=fatfs, far=0000000000114d30, elr=0000000000105e58, esr=0000000092000007
[kernel] Data Abort: task=fatfs, far=0000000000112d30, elr=0000000000105e58, esr=0000000092000007
[kernel] Data Abort: task=fatfs, far=0000000000111d30, elr=0000000000105e58, esr=0000000092000007
[fatfs] WARN:     #0: 0000000000100638 main()+0x118
[fatfs] WARN:     #1: 0000000000106950 resea_init()+0x50
[kernel] Data Abort: task=fatfs, far=0000000000113d30, elr=0000000000105e58, esr=0000000092000007
[fatfs] WARN:     #2: 000000000010f324 halt()+0x0
QEMU: Terminated

# menuconfigでfatfsを外す

$ make clean && make build
$ make run
[example] ready
[hello] Hello, World!
[vm] hello: terminated its execution
shell> [kernel] Instruction Abort: task=shell, far=000000000010a450, elr=000000000010a450, esr=0000000082000007
[random] ready
[ramdisk] ready
[dm] ready
                        # shellからデータを読み込めない
```

## 正常に動くようになった

```bash
$ make run
        CC  kernel/task.c
        CC  servers/shell/main.c
        LD  build/shell.debug.elf
   SYMBOLS  build/shell.debug.elf
     STRIP  build/shell.elf
  MKBOOTFS  build/bootfs.bin
        CC  servers/vm/bootfs_image.S
        LD  build/vm.debug.elf
   SYMBOLS  build/vm.debug.elf
     STRIP  build/vm.elf
        CC  kernel/arch/arm64/trap.S
        CC  kernel/arch/arm64/switch.S
        CC  kernel/arch/arm64/usercopy.S
        CC  kernel/arch/arm64/machines/raspi3/boot.S
        LD  build/resea.elf
       GEN  build/resea.symbols
   SYMBOLS  build/resea.elf
   OBJCOPY  build/kernel8.img
       RUN  build/kernel8.img
bcm2835_property: 0x00038002 set clock rate NYI

Booting Resea v0.9.0 (ac85038)...
[kernel] Booting CPU #1...
[kernel] Booting CPU #2...
[kernel] Booting CPU #3...
[kernel] new task #1: vm (pager=(null))
[kernel] boot ELF: entry=0000000000116ee8
[kernel] boot ELF: 0000000000100000 -> 0000000000092000 (4940KiB)
[kernel] boot ELF: 00000000005d3000 -> 0000000000565000 (52KiB)
[kernel] boot ELF: 0000000000800000 -> 0000000000000000 (4300KiB, zeroed)
[kernel] new task #0: (idle) (pager=(null))
[kernel] Booted CPU #0
[vm] starting...
[vm] available RAM region #0: 0000000003000000-000000000865e000 (86MiB)
[vm] launching dm...
[kernel] new task #2: dm (pager=vm)
[vm] launching hello...
[kernel] new task #3: hello (pager=vm)
[vm] launching ramdisk...
[kernel] new task #4: ramdisk (pager=vm)
[vm] launching random...
[kernel] new task #5: random (pager=vm)
[vm] launching shell...
[kernel] new task #6: shell (pager=vm)
[vm] ready
[random] starting...
[hello] Hello, World!
[vm] hello: terminated its execution
[kernel] destroying hello...
[dm] starting...
[random] ready
[shell] starting...
[kernel] enabled IRQ: task=shell, vector=57
shell> [ramdisk] ready
[dm] ready
[shell] help              -  Print this message.
[shell] <task> cmdline... -  Launch a task.
[shell] ps                -  List tasks.
[shell] q                 -  Halt the computer.
[shell] fs-read path      -  Read a file.
[shell] fs-write path str -  Write a string into a file.
[shell] http-get url      -  Peform a HTTP GET request.
[kernel] #1 vm: state=blocked, src=0
[kernel] #2 dm: state=blocked, src=0
[kernel] #4 ramdisk: state=blocked, src=0
[kernel] #5 random: state=blocked, src=0
[kernel] #6 shell: state=runnable, src=0
shell> q
$
```

```diff
$ git diff resea.diff
diff --git a/kernel/arch/arm64/arch.h b/kernel/arch/arm64/arch.h
index 41feb1f..37656b0 100644
--- a/kernel/arch/arm64/arch.h
+++ b/kernel/arch/arm64/arch.h
@@ -7,7 +7,7 @@

 #define STACK_SIZE        4096
 #define TICK_HZ           1000
-#define IRQ_MAX           32
+#define IRQ_MAX           64
 #define KERNEL_BASE_ADDR  0xffff000000000000
 #define STRAIGHT_MAP_ADDR 0x03000000
 #define STRAIGHT_MAP_END  0x3f000000
diff --git a/kernel/arch/arm64/interrupt.c b/kernel/arch/arm64/interrupt.c
index 7afeae1..c6ed0ac 100644
--- a/kernel/arch/arm64/interrupt.c
+++ b/kernel/arch/arm64/interrupt.c
@@ -12,8 +12,18 @@ extern char arm64_usercopy2[];
 extern char arm64_usercopy3[];

 void arm64_handle_interrupt(void) {
-    arm64_timer_reload();
-    handle_timer_irq();
+    int src = mmio_read(IRQ_SRC_CORE(mp_self()));
+    if (src & IRQ_SRC_CNTVIRQ) {
+        arm64_timer_reload();
+        handle_timer_irq();
+    }
+    if (src & IRQ_SRC_GPU) {
+        uint64_t irq = mmio_read(PENDING_IRQS_1) | (((uint64_t)mmio_read(PENDING_IRQS_2)) << 32);
+        if (irq & (1UL << IRQ_57_UART)) {
+            handle_irq(IRQ_57_UART);
+            mmio_write(UART0_ICR, 0x3ff);
+        }
+    }
 }

 void arm64_handle_exception(void) {
diff --git a/kernel/arch/arm64/machines/raspi3/build.mk b/kernel/arch/arm64/machines/raspi3/build.mk
index f106478..b70eeaf 100644
--- a/kernel/arch/arm64/machines/raspi3/build.mk
+++ b/kernel/arch/arm64/machines/raspi3/build.mk
@@ -1,7 +1,7 @@
 objs-$(CONFIG_MACHINE_RASPI3) += boot.o peripherals.o mp.o

 CFLAGS += -mcpu=cortex-a53
-QEMUFLAGS += -M raspi3 -smp 4
+QEMUFLAGS += -M raspi3b -smp 4

 # Raspberry Pi's kernel8.img.
 .PHONY: image
diff --git a/kernel/arch/arm64/machines/raspi3/include/machine/peripherals.h b/kernel/arch/arm64/machines/raspi3/include/machine/peripherals.h
index 36d627e..fdfabfa 100644
--- a/kernel/arch/arm64/machines/raspi3/include/machine/peripherals.h
+++ b/kernel/arch/arm64/machines/raspi3/include/machine/peripherals.h
@@ -7,6 +7,7 @@
 // TODO: Let me know if you know the location of the BCM2837 datasheet!

 #define MMIO_BASE  0x3f000000
+#define LOCAL_BASE 0x40000000
 #define UART0_DR   (MMIO_BASE + 0x00201000)  // Data register.
 #define UART0_FR   (MMIO_BASE + 0x00201018)  // Flag register.
 #define UART0_IBRD (MMIO_BASE + 0x00201024)  // Integer baud rate divisor.
@@ -14,6 +15,7 @@
     (MMIO_BASE + 0x00201028)  // Fractional part of the baud rate divisor.
 #define UART0_LCRH (MMIO_BASE + 0x0020102c)  // Line control registe.
 #define UART0_CR   (MMIO_BASE + 0x00201030)  // Control register.
+#define UART0_IMSC (MMIO_BASE + 0x00201038)  // Interrupt Mask Set Clear register.
 #define UART0_ICR  (MMIO_BASE + 0x00201044)  // Interrupt clear register.
 #define GPIO_FSEL1 (MMIO_BASE + 0x00200004)  // GPIO Function Select 1.
 #define GPIO_PUD   (MMIO_BASE + 0x00200094)  // GPIO Pin Pull-up/down Enable.
@@ -28,6 +30,26 @@
 #define MBOX_CONFIG (MMIO_BASE + 0xb880 + 0x1C)
 #define MBOX_WRITE  (MMIO_BASE + 0xb880 + 0x20)

+// BCM2837割り込みレジスタ
+#define IRQ_BASIC_PENDING       (MMIO_BASE + 0xb200)
+#define PENDING_IRQS_1          (MMIO_BASE + 0xb204)
+#define PENDING_IRQS_2          (MMIO_BASE + 0xb208)
+#define FIQ_CONTROL             (MMIO_BASE + 0xb20C)
+#define ENABLE_IRQS_1           (MMIO_BASE + 0xb210)
+#define ENABLE_IRQS_2           (MMIO_BASE + 0xb214)
+#define ENABLE_BASIC_IRQS       (MMIO_BASE + 0xb218)
+#define DISABLE_IRQS_1          (MMIO_BASE + 0xb21C)
+#define DISABLE_IRQS_2          (MMIO_BASE + 0xb220)
+#define DISABLE_BASIC_IRQS      (MMIO_BASE + 0xb224)
+
+// BCM2837 ARMローカルペリフェラル
+#define GPU_INT_ROUTE		(LOCAL_BASE + 0x0C)
+#define IRQ_SRC_CORE(core)	(LOCAL_BASE + 0x60 + 4 * (core))
+#define IRQ_SRC_TIMER		(1 << 11)
+#define IRQ_SRC_GPU		(1 << 8)
+#define IRQ_SRC_CNTVIRQ		(1 << 3)
+#define IRQ_SRC_CNTPNSIRQ	(1 << 1)
+#define IRQ_57_UART		(57)
 //
 //  Mailbox - An interface to communicate with VideoCore (GPU) on Raspberry Pi.
 //  https://github.com/raspberrypi/firmware/wiki/Mailboxes
diff --git a/kernel/arch/arm64/machines/raspi3/peripherals.c b/kernel/arch/arm64/machines/raspi3/peripherals.c
index 4c00abc..b8a842a 100644
--- a/kernel/arch/arm64/machines/raspi3/peripherals.c
+++ b/kernel/arch/arm64/machines/raspi3/peripherals.c
@@ -63,12 +63,15 @@ void uart_init(void) {
     delay(150);
     mmio_write(GPIO_PUDCLK0, 0);

+    // FIFOは無効、割り込みは有効(UART0_IMSC=0x0)
     mmio_write(UART0_CR, 0);            // Disable UART.
-    mmio_write(UART0_ICR, 0x7ff);       // Disable interrupts from UART.
+    mmio_write(UART0_ICR, 0x7ff);       // UARTからの割り込みをクリア
     mmio_write(UART0_IBRD, 2);          // baud rate = 115200
     mmio_write(UART0_FBRD, 11);         //
     mmio_write(UART0_LCRH, 0b11 << 5);  // 8n1
+    mmio_write(UART0_IMSC, 1 << 4);     // Enable RX interrupt
     mmio_write(UART0_CR, 0x301);        // Enable RX, TX, and UART0.
+    mmio_write(ENABLE_IRQS_2, 1 << (CONSOLE_IRQ % 32));
 }

 bool kdebug_is_readable(void) {
diff --git a/kernel/syscall.c b/kernel/syscall.c
index 9de149c..d802bbb 100644
--- a/kernel/syscall.c
+++ b/kernel/syscall.c
@@ -258,11 +258,11 @@ static int sys_console_read(__user char *buf, int max_len) {
     if (!max_len) {
         return 0;
     }
-
+
     int i = 0;
     for (; i < max_len - 1; i++) {
-        char ch;
-        if ((ch = kdebug_readchar()) <= 0) {
+        int ch;
+        if ((ch = (int)kdebug_readchar()) <= 0) {
             break;
         }

diff --git a/libs/common/arch/arm64/arch_types.h b/libs/common/arch/arm64/arch_types.h
index e67e440..5b705d6 100644
--- a/libs/common/arch/arm64/arch_types.h
+++ b/libs/common/arch/arm64/arch_types.h
@@ -1,6 +1,8 @@
 #ifndef __ARCH_TYPES_H__
 #define __ARCH_TYPES_H__

+#define CONSOLE_IRQ 57
+
 typedef struct {
 } trap_frame_t;

diff --git a/libs/resea/arch/arm64/user.ld b/libs/resea/arch/arm64/user.ld
index 787131a..b557281 100644
--- a/libs/resea/arch/arm64/user.ld
+++ b/libs/resea/arch/arm64/user.ld
@@ -27,7 +27,7 @@ SECTIONS {

     __cmdline = 0x02fff000;

-    . = 0x00300000;
+    . = 0x00800000;
     __zeroed_pages = .;

     .bss : ALIGN(0x1000) {
diff --git a/servers/drivers/blk/ramdisk/build.mk b/servers/drivers/blk/ramdisk/build.mk
index f278451..88cadf0 100644
--- a/servers/drivers/blk/ramdisk/build.mk
+++ b/servers/drivers/blk/ramdisk/build.mk
@@ -6,7 +6,7 @@ RAM_DISK_IMG ?= $(BUILD_DIR)/ramdisk.img

 $(BUILD_DIR)/servers/drivers/blk/ramdisk/disk.o: $(RAM_DISK_IMG)
 	$(PROGRESS) "GEN" $@
-	echo ".rodata; .align 4096; .global __image, __image_end; __image: .incbin \"$<\"; __image_end:" > $(@:.o=.S)
+	echo ".rodata; .balign 4096; .global __image, __image_end; __image: .incbin \"$<\"; __image_end:" > $(@:.o=.S)
 	$(CC) $(CFLAGS) -o $@ -c $(@:.o=.S)

 $(RAM_DISK_IMG):
diff --git a/servers/shell/main.c b/servers/shell/main.c
index a4a4e70..0171675 100644
--- a/servers/shell/main.c
+++ b/servers/shell/main.c
@@ -101,6 +101,7 @@ static void read_input(void) {
         PRINTF("%c", buf[i]);
         switch (buf[i]) {
             case '\n':
+            case '\r':
                 cmdline[cmdline_index] = '\0';
                 printf("\e[0m");
                 printf_flush();
```
