# macで実行

ARCH = x86_64

```
$ make run

SeaBIOS (version rel-1.15.0-0-g2dd4b9b3f840-prebuilt.qemu.org)


iPXE (http://ipxe.org) 00:03.0 CA00 PCI2.10 PnP PMM+1FF91380+1FEF1380 CA00



Booting from ROM..

Booting Resea v0.9.0 (2bf14db)...
[kernel] Booted CPU #0
[vm] ready
shell> [dm] ready
[random] ready
[datetime] ready

[fatfs] bpb: magic=0xeb3c90
[fatfs] bpb: oem_name=MTOO4038, sector_size=512, sectors_per_cluster=2
[fatfs] bpb: num_reserved_sectors=1, num_fat=2, num_root_entries=512
[fatfs] bpb: num_total_sectors16=8196, media_id=248, sectors_per_fat16=12
[fatfs] bpb: sectors_per_track=63, num_head=16, hidden_sectors=0
[fatfs] bpb: num_total_sectors32=0, sectors_per_fat32=-1809252352, flags=47426
[fatfs] bpb: fat_version=20048, root_entries_cluster=1095639119, fsinfo_sector=7
[fatfs] bpb: sectors_per_backup_boot=8224, fsinfo_sector=17741, sectors_per_bacu
[fatfs] bpb: drive_number=192, winnt_flags=142, signature=216
[fatfs] bpb: volume_id=????, volume_label=, fat32_string=V
[fatfs] bpb: magic=0x55aa

[fatfs] sectors=8196, non_data_sectors=57, bpb.sectors_per_cluster=2, total_data_clus=4069  // Mac
[fatfs] sectors=8064, non_data_sectors=97, bpb.sectors_per_cluster=1, total_data_clus=7967  // ubuntu
[fatfs] WARN: FAT12 not supported
[fatfs] PANIC: failed to locate a FAT file system
[fatfs] WARN: Backtrace:
[fatfs] WARN:     #0: 0000000001005173 main()+0xe53
[fatfs] WARN:     #1: 0000000001001a47 resea_init()+0x57
[fatfs] WARN:     #2: 00000000010002c7 halt()+0x0
[vm] WARN: fatfs: exception occurred, killing the task...
[e1000] initialized the device
[e1000] MAC address = 52:54:00:12:34:56
[tcpip] ready
[tcpip] registered new net device 'net1'
[e1000] ready
[tcpip] WARN: retrying DHCP discover...
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
```

## servers/drivers/blk/ide/build.mkを修正でfatfsが動いた

```
mformat -i mformat -i $@.tmp -c 1       // -c 1 を追加

$ make run

[fatfs] bpb: magic=0xeb3c90
[fatfs] bpb: oem_name=MTOO4038, sector_size=512, sectors_per_cluster=1
[fatfs] bpb: num_reserved_sectors=1, num_fat=2, num_root_entries=512
[fatfs] bpb: num_total_sectors16=8196, media_id=248, sectors_per_fat16=32
[fatfs] bpb: sectors_per_track=63, num_head=16, hidden_sectors=0
[fatfs] bpb: num_total_sectors32=0, sectors_per_fat32=-517406720, flags=31137
[fatfs] bpb: fat_version=20026, root_entries_cluster=1095639119, fsinfo_sector=17741
[fatfs] bpb: sectors_per_backup_boot=8224, fsinfo_sector=17741, sectors_per_backup_boot=8224
[fatfs] bpb: drive_number=192, winnt_flags=142, signature=216
[fatfs] bpb: volume_id=????, volume_label=, fat32_string=V
[fatfs] bpb: magic=0x55aa
[fatfs] sectors=8196, non_data_sectors=97, bpb.sectors_per_cluster=1, total_data_clus=8099
[kernel] enabled IRQ: task=e1000, vector=11
[fatfs] Files ---------------------------------------------
[fatfs] /HELLO   TXT
[fatfs] ---------------------------------------------------
[fatfs] ready
[e1000] initialized the device
[e1000] MAC address = 52:54:00:12:34:56
[tcpip] starting...
[tcpip] ready
[tcpip] registered new net device 'net1'
[e1000] ready
[e1000] sent 302 bytes
[tcpip] WARN: retrying DHCP discover...
[e1000] sent 302 bytes
[e1000] received 316 bytes
[e1000] received 316 bytes
[e1000] sent 306 bytes
[e1000] received 316 bytes
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[e1000] sent 306 bytes
[e1000] received 316 bytes
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2      // ここでストール
```

# servers/shell/main.cを修正

```c
void main(void) {
    TRACE("starting...");
-#ifdef ARCH_X86
+#ifdef CONFIG_ARCH_X64
    ASSERT_OK(irq_acquire(CONSOLE_IRQ));
#endif
```

# benchmarkを追加設定

```
        CC  servers/apps/benchmark/main.c
servers/apps/benchmark/main.c:221:9: warning: implicit declaration of function '
        syscall(SYS_NOP, 0, 0, 0, 0, 0);
        ^
1 warning generated.
...
ld.lld: error: undefined symbol: syscall
>>> referenced by main.c:0 (servers/apps/benchmark/main.c:0)
>>>               build/servers/apps/benchmark/main.o:(main)
```

```diff
diff --git a/servers/apps/benchmark/main.c b/servers/apps/benchmark/main.c
index e53dfc5..85edd52 100644
--- a/servers/apps/benchmark/main.c
+++ b/servers/apps/benchmark/main.c
@@ -2,6 +2,7 @@
 #include <resea/ipc.h>
 #include <resea/malloc.h>
 #include <resea/printf.h>
+#include <arch/syscall.h>
 #include <resea/syscall.h>
 #include <string.h>
```



# CONFIG_ABI_EMU=y, CONFIG_TARFS_SERVER=y

```
       GEN  build/tarfs/__name__.c
        CC  build/tarfs/__name__.c
        CC  servers/fs/tarfs/main.c
FROM ubuntu:20.04
RUN apt-get update && apt-get install -qy build-essential curl sed
ADD add_files/etc/banner /build/etc/banner
[+] Building 48.0s (8/8) FINISHED
 => [internal] load build definition from Dockerfile                       0.1s
 => => transferring dockerfile: 169B                                       0.1s
 => [internal] load .dockerignore                                          0.0s
 => => transferring context: 2B                                            0.0s
 => [internal] load metadata for docker.io/library/ubuntu:20.04            3.4s
 => [1/3] FROM docker.io/library/ubuntu:20.04@sha256:9101220a875cee98b016  5.0s
 => => resolve docker.io/library/ubuntu:20.04@sha256:9101220a875cee98b016  0.0s
 => => sha256:9101220a875cee98b016668342c489ff0674f247f6c 1.42kB / 1.42kB  0.0s
 => => sha256:31cd7bbfd36421dfd338bceb36d803b3663c1bfa87dfe6a 529B / 529B  0.0s
 => => sha256:825d55fb6340083b06e69e02e823a02918f3ffb575e 1.46kB / 1.46kB  0.0s
 => => sha256:e0b25ef516347a097d75f8aea6bc0f42a4e8e70b0 28.57MB / 28.57MB  2.9s
 => => extracting sha256:e0b25ef516347a097d75f8aea6bc0f42a4e8e70b057e84d8  1.8s
 => [internal] load build context                                          0.0s
 => => transferring context: 339B                                          0.0s
 => [2/3] RUN apt-get update && apt-get install -qy build-essential curl  37.2s
 => [3/3] ADD add_files/etc/banner /build/etc/banner                       0.0s
 => exporting to image                                                     2.2s
 => => exporting layers                                                    2.2s
 => => writing image sha256:e863f476cbd82fc02f21f2d24ab629dd78e65efa91c5a  0.0s
 => => naming to docker.io/library/minlin-files                            0.0s

Use 'docker scan' to run Snyk tests against images to find vulnerabilities and l
Error: No such container: minlin-files-container
minlin-files-container
FROM ubuntu:20.04
RUN apt-get update && apt-get install -qy build-essential curl sed
RUN apt-get install -qy musl-tools
RUN curl -fsSL --output tarball.tar.bz2 "https://busybox.net/downloads/busybox-1
RUN mkdir /build && tar xf tarball.tar.bz2 --strip-components=1 -C /build
WORKDIR /build
RUN ln -s /usr/bin/ar /usr/bin/musl-ar
RUN ln -s /usr/bin/strip /usr/bin/musl-strip
RUN make -j16 allnoconfig
RUN sh -c "sed -i -e 's/# CONFIG_CROSS_COMPILER_PREFIX is not set/CONFIG_CROSS_C
RUN sh -c "sed -i -e 's/# CONFIG_STATIC is not set/CONFIG_STATIC=y/' .config;sed
RUN sh -c "sed -i -e 's/# CONFIG_ECHO is not set/CONFIG_ECHO=y/' .config;sed -i
RUN sh -c "sed -i -e 's/# CONFIG_CAT is not set/CONFIG_CAT=y/' .config;sed -i -e
RUN sh -c "sed -i -e 's/# CONFIG_UNAME is not set/CONFIG_UNAME=y/' .config;sed -
RUN sh -c "sed -i -e 's/# CONFIG_ASH is not set/CONFIG_ASH=y/' .config;sed -i -e
RUN sh -c "sed -i -e 's/# CONFIG_ASH_OPTIMIZE_FOR_SIZE is not set/CONFIG_ASH_OPT
RUN make -j16
[+] Building 31.0s (21/21) FINISHED
 => [internal] load build definition from Dockerfile                       0.0s
 => => transferring dockerfile: 1.55kB                                     0.0s
 => [internal] load .dockerignore                                          0.0s
 => => transferring context: 2B                                            0.0s
 => [internal] load metadata for docker.io/library/ubuntu:20.04            0.9s
 => [ 1/17] FROM docker.io/library/ubuntu:20.04@sha256:9101220a875cee98b0  0.0s
 => CACHED [ 2/17] RUN apt-get update && apt-get install -qy build-essent  0.0s
 => [ 3/17] RUN apt-get install -qy musl-tools                             5.4s
 => [ 4/17] RUN curl -fsSL --output tarball.tar.bz2 "https://busybox.net/  3.7s
 => [ 5/17] RUN mkdir /build && tar xf tarball.tar.bz2 --strip-components  1.1s
 => [ 6/17] WORKDIR /build                                                 0.0s
 => [ 7/17] RUN ln -s /usr/bin/ar /usr/bin/musl-ar                         0.3s
 => [ 8/17] RUN ln -s /usr/bin/strip /usr/bin/musl-strip                   0.3s
 => [ 9/17] RUN make -j16 allnoconfig                                      4.4s
 => [10/17] RUN sh -c "sed -i -e 's/# CONFIG_CROSS_COMPILER_PREFIX is not  0.3s
 => [11/17] RUN sh -c "sed -i -e 's/# CONFIG_STATIC is not set/CONFIG_STA  0.3s
 => [12/17] RUN sh -c "sed -i -e 's/# CONFIG_ECHO is not set/CONFIG_ECHO=  0.3s
 => [13/17] RUN sh -c "sed -i -e 's/# CONFIG_CAT is not set/CONFIG_CAT=y/  0.3s
 => [14/17] RUN sh -c "sed -i -e 's/# CONFIG_UNAME is not set/CONFIG_UNAM  0.3s
 => [15/17] RUN sh -c "sed -i -e 's/# CONFIG_ASH is not set/CONFIG_ASH=y/  0.3s
 => [16/17] RUN sh -c "sed -i -e 's/# CONFIG_ASH_OPTIMIZE_FOR_SIZE is not  0.4s
 => [17/17] RUN make -j16                                                 12.0s
 => exporting to image                                                     0.6s
 => => exporting layers                                                    0.5s
 => => writing image sha256:382748f5d4f83c41b4ac4d95e5b31a74be40b86366c32  0.0s
 => => naming to docker.io/library/minlin-busybox                          0.0s

Use 'docker scan' to run Snyk tests against images to find vulnerabilities and l
Error: No such container: minlin-busybox-container
minlin-busybox-container
       GEN  build/servers/fs/tarfs/tarball.o
```

## 起動

```
SeaBIOS (version rel-1.15.0-0-g2dd4b9b3f840-prebuilt.qemu.org)


iPXE (http://ipxe.org) 00:03.0 CA00 PCI2.10 PnP PMM+1FF91380+1FEF1380 CA00



Booting from ROM..

Booting Resea v0.9.0 (8eb2360)...
[kernel] new task #1: vm (pager=(null))
[kernel] boot ELF: entry=0000000001016520
[kernel] boot ELF: 0000000001000000 -> 0000000000116000 (11064KiB)
[kernel] boot ELF: 0000000003000000 -> 0000000000be4000 (36KiB)
[kernel] boot ELF: 0000000004000000 -> 0000000000000000 (137292KiB, zeroed)
[kernel] new task #0: (idle) (pager=(null))
[kernel] Booted CPU #0
[vm] starting...
[vm] available RAM region #3: 0000000010000000-0000000020c30000 (268MiB)
[vm] launching benchmark_server...
[kernel] new task #2: benchmark_serve (pager=vm)
[vm] launching datetime...
[kernel] new task #3: datetime (pager=vm)
[vm] launching dm...
[kernel] new task #4: dm (pager=vm)
[vm] launching e1000...
[kernel] new task #5: e1000 (pager=vm)
[vm] launching fatfs...
[kernel] new task #6: fatfs (pager=vm)
[vm] launching ide...
[kernel] new task #7: ide (pager=vm)
[vm] launching minlin...
[kernel] new task #8: minlin (pager=vm)
[vm] launching ps2...
[kernel] new task #9: ps2 (pager=vm)
[vm] launching ramdisk...
[kernel] new task #10: ramdisk (pager=vm)
[vm] launching random...
[kernel] new task #11: random (pager=vm)
[vm] launching rtc...
[kernel] new task #12: rtc (pager=vm)
[vm] launching shell...
[kernel] new task #13: shell (pager=vm)
[vm] launching shm_test...
[kernel] new task #14: shm_test (pager=vm)
[vm] launching tarfs...
[kernel] new task #15: tarfs (pager=vm)
[vm] launching tcpip...
[kernel] new task #16: tcpip (pager=vm)
[vm] ready
[benchmark_server] starting benchmark server...
[datetime] starting...
[dm] starting...
[e1000] starting...
[fatfs] starting...
[random] starting...
[shell] starting...
[tarfs] starting...
[ps2] starting...
[kernel] enabled IRQ: task=shell, vector=4
[kernel] enabled IRQ: task=ps2, vector=12
[kernel] enabled IRQ: task=ps2, vector=1
shell> [tarfs] tarfs: bin/sh (len=0, type=2) 203424152 0000000000000000
[tarfs] tarfs: bin/echo (len=0, type=2) 203424152 0000000000000000
[tarfs] tarfs: bin/cat (len=0, type=2) 203424152 0000000000000000
[tarfs] tarfs: bin/uname (len=0, type=2) 203424152 0000000000000000
[tarfs] tarfs: ././@PaxHeader (len=28, type=x) 203424152 0000000000000000
[tarfs] tarfs:  (len=0, type=5) 203424152 0000000000000000
[minlin] starting...
[tarfs] tarfs: ././@PaxHeader (len=28, type=x) 203424152 0000000000000000
[tarfs] tarfs: bin/ (len=0, type=5) 203424152 0000000000000000
[tarfs] tarfs: ././@PaxHeader (len=22, type=x) 203424152 0000000000000000
[ide] ready
[ramdisk] ready
[rtc] ready
[random] ready
[dm] ready
[datetime] ready
[ps2] ready
[fatfs] Files ---------------------------------------------
[kernel] enabled IRQ: task=e1000, vector=11
[ps2] keyboard: scan_code=fa, key_code=80, ascii='' [release]
[ps2] unknown message notifications
[minlin] servers/minlin/tty.c:57: unexpected error (Already Exists)
[minlin] WARN: Backtrace:
[fatfs] /HELLO   TXT
[fatfs] ---------------------------------------------------
[minlin] WARN:     #0: 00000000010093cd acquire()+0xcd
[minlin] WARN:     #1: 0000000001007c8d devfs_open()+0xed
[minlin] WARN:     #2: 000000000100826d fs_open()+0x13d
[minlin] WARN:     #3: 000000000100804d fs_proc_open()+0x28d
[minlin] WARN:     #4: 00000000010086e9 fs_fork()+0xb9
[minlin] WARN:     #5: 0000000001002be1 proc_fork()+0x41
[minlin] WARN:     #6: 00000000010043b9 proc_init()+0x39
[minlin] WARN:     #7: 00000000010002f9 main()+0x49
[minlin] WARN:     #8: 0000000001011eb7 resea_init()+0x57
[minlin] WARN:     #9: 000000000101a263 halt()+0x0
[kernel] WARN: Exception #13

[kernel] RIP = 000000000101a263 CS  = 000000000000002b  RFL = 0000000000000202
[kernel] SS  = 0000000000000023 RSP = 00000000041ffbf8  RBP = 00000000041ffc20
[kernel] RAX = 000000000101a263 RBX = 0000000001026d2d  RCX = 000000000101675a
[kernel] RDX = 0000000000000027 RSI = 000000000101dde0  RDI = 000000000101ddd0
[kernel] R8  = 0000000000000000 R9  = 0000000000000000  R10 = 0000000000000000
[kernel] R11 = 0000000000000246 R12 = 0000000001027301  R13 = 00000000041ffca0
[kernel] R14 = 00000000fffffff6 R15 = 000000000c200070  ERR = 0000000000000032
[vm] WARN: minlin: exception occurred, killing the task...
[kernel] destroying minlin...
[fatfs] ready
[e1000] initialized the device
[e1000] MAC address = 52:54:00:12:34:56
[tcpip] starting...
[tcpip] ready
[tcpip] registered new net device 'net1'
[e1000] ready
[e1000] sent 302 bytes
[tarfs] tarfs: bin/busybox (len=132928, type=0) 203424152 0000000000000000
[tarfs] tarfs: ././@PaxHeader (len=28, type=x) 203424152 0000000000000000
[tarfs] tarfs: etc/ (len=0, type=5) 203424152 0000000000000000
[tarfs] tarfs: ././@PaxHeader (len=22, type=x) 203424152 0000000000000000
[tarfs] tarfs: etc/banner (len=224, type=0) 203424152 0000000000000000
[tarfs] ready
[tcpip] WARN: retrying DHCP discover...
[e1000] sent 302 bytes
[e1000] received 316 bytes
[e1000] received 316 bytes
[e1000] sent 306 bytes
[e1000] received 316 bytes
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[e1000] sent 306 bytes
[e1000] received 316 bytes
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
```

## benchmarkを実行

```
shell> benchmark
[vm] launching benchmark...
[kernel] new task #8: benchmark (pager=vm)
shell> [benchmark] starting IPC benchmark...
{"type":"metric","key":"reading cycle counter","value":269}
[benchmark] reading cycle counter: cycles: avg=1196, min=269, max=535834
[benchmark] reading cycle counter: l1d_cache_access: avg=0, min=0, max=0
[benchmark] reading cycle counter: l2d_cache_access: avg=0, min=0, max=0
[benchmark] reading cycle counter: mem_access: avg=0, min=0, max=0
[benchmark] reading cycle counter: l1_tlb_refill: avg=0, min=0, max=0
[benchmark] reading cycle counter: num_exceptions: avg=0, min=0, max=0
{"type":"metric","key":"memcpy (8-bytes)","value":429}
[benchmark] memcpy (8-bytes): cycles: avg=614, min=429, max=143893
[benchmark] memcpy (8-bytes): l1d_cache_access: avg=0, min=0, max=0
[benchmark] memcpy (8-bytes): l2d_cache_access: avg=0, min=0, max=0
[benchmark] memcpy (8-bytes): mem_access: avg=0, min=0, max=0
[benchmark] memcpy (8-bytes): l1_tlb_refill: avg=0, min=0, max=0
[benchmark] memcpy (8-bytes): num_exceptions: avg=0, min=0, max=0
{"type":"metric","key":"memcpy (64-bytes)","value":1023}
[benchmark] memcpy (64-bytes): cycles: avg=1293, min=1023, max=148077
[benchmark] memcpy (64-bytes): l1d_cache_access: avg=0, min=0, max=0
[benchmark] memcpy (64-bytes): l2d_cache_access: avg=0, min=0, max=0
[benchmark] memcpy (64-bytes): mem_access: avg=0, min=0, max=0
[benchmark] memcpy (64-bytes): l1_tlb_refill: avg=0, min=0, max=0
[benchmark] memcpy (64-bytes): num_exceptions: avg=0, min=0, max=0
{"type":"metric","key":"memcpy (512-bytes)","value":5721}
[benchmark] memcpy (512-bytes): cycles: avg=6175, min=5721, max=91816
[benchmark] memcpy (512-bytes): l1d_cache_access: avg=0, min=0, max=0
[benchmark] memcpy (512-bytes): l2d_cache_access: avg=0, min=0, max=0
[benchmark] memcpy (512-bytes): mem_access: avg=0, min=0, max=0
[benchmark] memcpy (512-bytes): l1_tlb_refill: avg=0, min=0, max=0
[benchmark] memcpy (512-bytes): num_exceptions: avg=0, min=0, max=0
{"type":"metric","key":"nop syscall","value":2276}
[benchmark] nop syscall: cycles: avg=2677, min=2276, max=172016
[benchmark] nop syscall: l1d_cache_access: avg=0, min=0, max=0
[benchmark] nop syscall: l2d_cache_access: avg=0, min=0, max=0
[benchmark] nop syscall: mem_access: avg=0, min=0, max=0
[benchmark] nop syscall: l1_tlb_refill: avg=0, min=0, max=0
[benchmark] nop syscall: num_exceptions: avg=0, min=0, max=0
{"type":"metric","key":"IPC round-trip (simple)","value":404002}
[benchmark] IPC round-trip (simple): cycles: avg=519873, min=404002, max=6019761
[benchmark] IPC round-trip (simple): l1d_cache_access: avg=0, min=0, max=0
[benchmark] IPC round-trip (simple): l2d_cache_access: avg=0, min=0, max=0
[benchmark] IPC round-trip (simple): mem_access: avg=0, min=0, max=0
[benchmark] IPC round-trip (simple): l1_tlb_refill: avg=0, min=0, max=0
[benchmark] IPC round-trip (simple): num_exceptions: avg=0, min=0, max=0
{"type":"metric","key":"IPC round-trip (with 1-byte ool)","value":4399914}
[benchmark] IPC round-trip (with 1-byte ool): cycles: avg=4813318, min=4399914, max=13860788
[benchmark] IPC round-trip (with 1-byte ool): l1d_cache_access: avg=0, min=0, max=0
[benchmark] IPC round-trip (with 1-byte ool): l2d_cache_access: avg=0, min=0, max=0
[benchmark] IPC round-trip (with 1-byte ool): mem_access: avg=0, min=0, max=0
[benchmark] IPC round-trip (with 1-byte ool): l1_tlb_refill: avg=0, min=0, max=0
[benchmark] IPC round-trip (with 1-byte ool): num_exceptions: avg=0, min=0, max=0
{"type":"metric","key":"IPC round-trip (with PAGE_SIZE-sized ool)","value":4474109}
[benchmark] IPC round-trip (with PAGE_SIZE-sized ool): cycles: avg=4867594, min=4474109, max=14313127
[benchmark] IPC round-trip (with PAGE_SIZE-sized ool): l1d_cache_access: avg=0, min=0, max=0
[benchmark] IPC round-trip (with PAGE_SIZE-sized ool): l2d_cache_access: avg=0, min=0, max=0
[benchmark] IPC round-trip (with PAGE_SIZE-sized ool): mem_access: avg=0, min=0, max=0
[benchmark] IPC round-trip (with PAGE_SIZE-sized ool): l1_tlb_refill: avg=0, min=0, max=0
[benchmark] IPC round-trip (with PAGE_SIZE-sized ool): num_exceptions: avg=0, min=0, max=0
[vm] benchmark: terminated its execution
[kernel] destroying benchmark...
```

## help

```
shell> help
[shell] help              -  Print this message.
[shell] <task> cmdline... -  Launch a task.
[shell] ps                -  List tasks.
[shell] q                 -  Halt the computer.
[shell] fs-read path      -  Read a file.
[shell] fs-write path str -  Write a string into a file.
[shell] http-get url      -  Peform a HTTP GET request.
shell> ps
[kernel] #1 vm: state=blocked, src=0
[kernel] #2 benchmark_serve: state=blocked, src=0
[kernel] #3 datetime: state=blocked, src=0
[kernel] #4 dm: state=blocked, src=0
[kernel] #5 e1000: state=blocked, src=0
[kernel] #6 fatfs: state=blocked, src=0
[kernel] #7 ide: state=blocked, src=0
[kernel] #9 ps2: state=blocked, src=0
[kernel] #10 ramdisk: state=blocked, src=0
[kernel] #11 random: state=blocked, src=0
[kernel] #12 rtc: state=blocked, src=0
[kernel] #13 shell: state=runnable, src=0
[kernel] #14 shm_test: state=blocked, src=0
[kernel] #15 tarfs: state=blocked, src=0
[kernel] #16 tcpip: state=blocked, src=0
shell> q
$
```
