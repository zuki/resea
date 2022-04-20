# ディレクトリツリー

```
.
├── Cargo.lock
├── Cargo.toml
├── Kconfig
├── LICENSE.md
├── Makefile
├── README.md
├── interface.idl
├── kernel
│   ├── Kconfig
│   ├── arch
│   │   ├── arm64
│   │   │   ├── Kconfig
│   │   │   ├── arch.h
│   │   │   ├── asm.h
│   │   │   ├── build.mk
│   │   │   ├── init.c
│   │   │   ├── interrupt.c
│   │   │   ├── machines
│   │   │   │   └── raspi3
│   │   │   │       ├── Kconfig
│   │   │   │       ├── boot.S
│   │   │   │       ├── build.mk
│   │   │   │       ├── config.txt
│   │   │   │       ├── include
│   │   │   │       │   └── machine
│   │   │   │       │       ├── machine.h
│   │   │   │       │       └── peripherals.h
│   │   │   │       ├── kernel.ld
│   │   │   │       ├── mp.c
│   │   │   │       └── peripherals.c
│   │   │   ├── mp.c
│   │   │   ├── switch.S
│   │   │   ├── task.c
│   │   │   ├── trap.S
│   │   │   ├── usercopy.S
│   │   │   ├── vm.c
│   │   │   └── vm.h
│   │   ├── example
│   │   │   ├── Kconfig
│   │   │   ├── arch.h
│   │   │   ├── boot.S
│   │   │   ├── build.mk
│   │   │   ├── init.c
│   │   │   ├── interrupt.c
│   │   │   ├── kernel.ld
│   │   │   ├── mp.c
│   │   │   ├── peripherals.c
│   │   │   ├── task.c
│   │   │   └── vm.c
│   │   └── x64
│   │       ├── Kconfig
│   │       ├── arch.h
│   │       ├── boot.S
│   │       ├── build.mk
│   │       ├── hv.c
│   │       ├── hv.h
│   │       ├── init.c
│   │       ├── interrupt.c
│   │       ├── interrupt.h
│   │       ├── kernel.ld
│   │       ├── mp.c
│   │       ├── mp.h
│   │       ├── multiboot.h
│   │       ├── screen.c
│   │       ├── screen.h
│   │       ├── serial.c
│   │       ├── serial.h
│   │       ├── task.c
│   │       ├── task.h
│   │       ├── trap.S
│   │       ├── trap.h
│   │       ├── vm.c
│   │       └── vm.h
│   ├── boot.c
│   ├── boot.h
│   ├── build.mk
│   ├── ipc.c
│   ├── ipc.h
│   ├── kdebug.c
│   ├── kdebug.h
│   ├── printk.c
│   ├── printk.h
│   ├── syscall.c
│   ├── syscall.h
│   ├── task.c
│   └── task.h
├── libs
│   ├── common
│   │   ├── arch
│   │   │   ├── arm64
│   │   │   │   ├── arch_types.h
│   │   │   │   └── build.mk
│   │   │   ├── example
│   │   │   │   ├── arch_types.h
│   │   │   │   └── build.mk
│   │   │   └── x64
│   │   │       ├── arch_types.h
│   │   │       ├── build.mk
│   │   │       └── memcpy.S
│   │   ├── bitmap.c
│   │   ├── build.mk
│   │   ├── include
│   │   │   ├── bitmap.h
│   │   │   ├── bootinfo.h
│   │   │   ├── endian.h
│   │   │   ├── list.h
│   │   │   ├── message.h
│   │   │   ├── print_macros.h
│   │   │   ├── string.h
│   │   │   ├── types.h
│   │   │   ├── ubsan.h
│   │   │   └── vprintf.h
│   │   ├── string.c
│   │   ├── ubsan.c
│   │   └── vprintf.c
│   ├── driver
│   │   ├── build.mk
│   │   ├── dma.c
│   │   ├── include
│   │   │   └── driver
│   │   │       ├── dma.h
│   │   │       ├── io.h
│   │   │       └── irq.h
│   │   ├── io.c
│   │   └── irq.c
│   ├── elf
│   │   ├── build.mk
│   │   ├── dummy.c
│   │   └── include
│   │       └── elf
│   │           └── elf.h
│   ├── resea
│   │   ├── Kconfig
│   │   ├── arch
│   │   │   ├── arm64
│   │   │   │   ├── arch
│   │   │   │   │   ├── io.h
│   │   │   │   │   └── syscall.h
│   │   │   │   ├── build.mk
│   │   │   │   ├── start.S
│   │   │   │   └── user.ld
│   │   │   ├── example
│   │   │   │   ├── arch
│   │   │   │   │   ├── io.h
│   │   │   │   │   └── syscall.h
│   │   │   │   ├── build.mk
│   │   │   │   ├── start.S
│   │   │   │   └── user.ld
│   │   │   └── x64
│   │   │       ├── arch
│   │   │       │   └── syscall.h
│   │   │       ├── build.mk
│   │   │       ├── start.S
│   │   │       └── user.ld
│   │   ├── async.c
│   │   ├── build.mk
│   │   ├── cmdline.c
│   │   ├── datetime.c
│   │   ├── handle.c
│   │   ├── include
│   │   │   └── resea
│   │   │       ├── async.h
│   │   │       ├── cmdline.h
│   │   │       ├── ctype.h
│   │   │       ├── datetime.h
│   │   │       ├── handle.h
│   │   │       ├── ipc.h
│   │   │       ├── malloc.h
│   │   │       ├── printf.h
│   │   │       ├── syscall.h
│   │   │       ├── task.h
│   │   │       └── timer.h
│   │   ├── init.c
│   │   ├── ipc.c
│   │   ├── malloc.c
│   │   ├── printf.c
│   │   ├── rust
│   │   │   ├── Cargo.lock
│   │   │   ├── Cargo.toml
│   │   │   ├── arch
│   │   │   │   └── x64
│   │   │   │       └── x64.json
│   │   │   └── src
│   │   │       ├── allocator.rs
│   │   │       ├── capi.rs
│   │   │       ├── lang_items.rs
│   │   │       ├── lib.rs
│   │   │       └── print.rs
│   │   ├── syscall.c
│   │   ├── task.c
│   │   └── timer.c
│   ├── third_party
│   │   ├── cairo
│   │   │   ├── build.mk
│   │   │   ├── cairo-1.17.4
│   │   │   ├── freetype-2.11.0
│   │   │   ├── freetype_objs.mk
│   │   │   ├── internal_headers
│   │   │   ├── libpng-1.6.37
│   │   │   └── pixman-0.40.0
│   │   ├── libc
│   │   │   ├── Dockerfile
│   │   │   ├── build.mk
│   │   │   ├── compile_rules.mk
│   │   │   ├── internal_headers
│   │   │   │   ├── _newlib_version.h
│   │   │   │   └── newlib.h
│   │   │   ├── missing_headers
│   │   │   │   ├── README.md
│   │   │   │   ├── float.h
│   │   │   │   ├── limits.h
│   │   │   │   ├── stdarg.h
│   │   │   │   ├── stdbool.h
│   │   │   │   ├── stddef.h
│   │   │   │   └── sys
│   │   │   │       └── mman.h
│   │   │   ├── newlib-4.1.0
│   │   │   ├──newlib_objs.m
│   │   │   ├──newlib_syscalls.c
│   │   │   └──symbol_overrides.txt
│   │   └── zlib
│   │       ├── build.mk
│   │       └── zlib-1.2.11
│   ├── unittest
│   └── virtio
│       ├── build.mk
│       ├── include
│       │   └── virtio
│       │       └── virtio.h
│       ├── virtio.c
│       ├── virtio_legacy.c
│       ├── virtio_legacy.h
│       ├── virtio_modern.c
│       └── virtio_modern.h
├── servers
│   ├── apps
│   │   ├── benchmark
│   │   │   ├── build.mk
│   │   │   └── main.c
│   │   ├── benchmark_server
│   │   │   ├── build.mk
│   │   │   └── main.c
│   │   ├── hello
│   │   │   ├── Kconfig
│   │   │   ├── build.mk
│   │   │   └── main.c
│   │   ├── hello_from_rust
│   │   │   ├── Cargo.lock
│   │   │   ├── Cargo.toml
│   │   │   ├── build.mk
│   │   │   └── main.rs
│   │   ├── memory_leak_test
│   │   │   ├── build.mk
│   │   │   └── main.c
│   │   ├── nommu_test
│   │   │   ├── Kconfig
│   │   │   ├── build.mk
│   │   │   └── main.c
│   │   ├── shm_test
│   │   │   ├── build.mk
│   │   │   └── main.c
│   │   ├── test
│   │   │   ├── Kconfig
│   │   │   ├── build.mk
│   │   │   ├── datetime_test.c
│   │   │   ├── ipc_test.c
│   │   │   ├── libcommon_test.c
│   │   │   ├── libresea_test.c
│   │   │   ├── main.c
│   │   │   ├── malloc_test.c
│   │   │   ├── shm_test.c
│   │   │   ├── test.h
│   │   │   └── unittest_test.c
│   │   └── webapi
│   │       ├── build.mk
│   │       ├── main.c
│   │       └── webapi.h
│   ├── dm
│   │   ├── build.mk
│   │   ├── device.h
│   │   ├── interface.idl
│   │   ├── main.c
│   │   ├── pci.c
│   │   └── pci.h
│   ├── drivers
│   │   ├── blk
│   │   │   ├── ide
│   │   │   │   ├── build.mk
│   │   │   │   ├── ide.h
│   │   │   │   └── main.c
│   │   │   └── ramdisk
│   │   │       ├── build.mk
│   │   │       └── main.c
│   │   ├── gpu
│   │   │   └── virtio_gpu
│   │   │       ├── Kconfig
│   │   │       ├── build.mk
│   │   │       ├── cairo_demo.c
│   │   │       ├── main.c
│   │   │       ├── virtio_gpu.h
│   │   │       └── wallpaper.png
│   │   ├── input
│   │   │   └── ps2
│   │   │       ├── build.mk
│   │   │       ├── keymap.c
│   │   │       ├── keymap.h
│   │   │       ├── main.c
│   │   │       └── ps2.h
│   │   ├── net
│   │   │   ├── e1000
│   │   │   │   ├── build.mk
│   │   │   │   ├── e1000.c
│   │   │   │   ├── e1000.h
│   │   │   │   └── main.c
│   │   │   └── virtio_net
│   │   │       ├── build.mk
│   │   │       ├── main.c
│   │   │       └── virtio_net.h
│   │   └── rtc
│   │       └── rtc
│   │           ├── build.mk
│   │           ├── main.c
│   │           └── rtc.h
│   ├── example
│   │   ├── build.mk
│   │   └── main.c
│   ├── experimental
│   │   ├── datetime
│   │   │   ├── build.mk
│   │   │   └── main.c
│   │   └── hdaudio
│   │       ├── build.mk
│   │       ├── hdaudio.c
│   │       ├── hdaudio.h
│   │       ├── main.c
│   │       └── wav2c.py
│   ├── fs
│   │   ├── fatfs
│   │   │   ├── build.mk
│   │   │   ├── fat.c
│   │   │   ├── fat.h
│   │   │   └── main.c
│   │   └── tarfs
│   │       ├── build.mk
│   │       └── main.c
│   ├── hv
│   │   ├── build.mk
│   │   ├── guest.c
│   │   ├── guest.h
│   │   ├── ioport.c
│   │   ├── ioport.h
│   │   ├── main.c
│   │   ├── mm.c
│   │   ├── mm.h
│   │   ├── pci.c
│   │   ├── pci.h
│   │   ├── samples
│   │   │   ├── common.mk
│   │   │   ├── hlt
│   │   │   │   ├── Makefile
│   │   │   │   ├── boot.S
│   │   │   │   └── kernel.ld
│   │   │   └── longmode
│   │   │       ├── Makefile
│   │   │       ├── boot.S
│   │   │       ├── kernel.c
│   │   │       └── kernel.ld
│   │   ├── virtio_blk.c
│   │   ├── virtio_blk.h
│   │   ├── x64.c
│   │   └── x64.h
│   ├── minlin
│   │   ├── __pycache__
│   │   │   └── build.cpython-310.pyc
│   │   ├── abi.h
│   │   ├── build.mk
│   │   ├── build.py
│   │   ├── elf.h
│   │   ├── fs.c
│   │   ├── fs.h
│   │   ├── main.c
│   │   ├── mm.c
│   │   ├── mm.h
│   │   ├── packages
│   │   │   ├── __pycache__
│   │   │   │   ├── busybox.cpython-310.pyc
│   │   │   │   └── files.cpython-310.pyc
│   │   │   ├── busybox.py
│   │   │   └── files.py
│   │   ├── proc.c
│   │   ├── proc.h
│   │   ├── syscall.c
│   │   ├── syscall.h
│   │   ├── tty.c
│   │   ├── tty.h
│   │   ├── waitqueue.c
│   │   └── waitqueue.h
│   ├── random
│   │   ├── build.mk
│   │   ├── interface.idl
│   │   └── main.c
│   ├── shell
│   │   ├── build.mk
│   │   ├── commands.c
│   │   ├── commands.h
│   │   ├── fs.c
│   │   ├── fs.h
│   │   ├── http.c
│   │   ├── http.h
│   │   └── main.c
│   ├── tcpip
│   │   ├── arp.c
│   │   ├── arp.h
│   │   ├── build.mk
│   │   ├── checksum.h
│   │   ├── device.c
│   │   ├── device.h
│   │   ├── dhcp.c
│   │   ├── dhcp.h
│   │   ├── dns.c
│   │   ├── dns.h
│   │   ├── ethernet.c
│   │   ├── ethernet.h
│   │   ├── icmp.c
│   │   ├── icmp.h
│   │   ├── interface.idl
│   │   ├── ipv4.c
│   │   ├── ipv4.h
│   │   ├── main.c
│   │   ├── main.h
│   │   ├── mbuf.c
│   │   ├── mbuf.h
│   │   ├── stats.c
│   │   ├── stats.h
│   │   ├── sys.h
│   │   ├── tcp.c
│   │   ├── tcp.h
│   │   ├── tcpip.h
│   │   ├── udp.c
│   │   └── udp.h
│   └── vm
│       ├── Kconfig
│       ├── bootfs.c
│       ├── bootfs.h
│       ├── bootfs_image.S
│       ├── build.mk
│       ├── main.c
│       ├── ool.c
│       ├── ool.h
│       ├── page_alloc.c
│       ├── page_alloc.h
│       ├── page_fault.c
│       ├── page_fault.h
│       ├── shm.c
│       ├── shm.h
│       ├── task.c
│       └── task.h
└── tools
    ├── bochsrc
    ├── config.py
    ├── download-file.py
    ├── dump2line.py
    ├── embed-bootelf-header.py
    ├── embed-symbols.py
    ├── genidl.py
    ├── genkconfig.py
    ├── grub.cfg
    ├── make-bootable-on-qemu.py
    ├── memory-leaks-in-log.py
    ├── merge-compile-commands.py
    ├── merge-config.py
    ├── mkbootfs.py
    ├── nm2symbols.py
    ├── requirements.txt
    ├── run-and-check.py
    ├── scan-libs-dir.py
    ├── scan-servers-dir.py
    ├── textedit.py
    ├── visualize-benchmark-log.py
    └── xorriso-wrapper
```
