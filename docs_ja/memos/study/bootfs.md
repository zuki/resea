# boot_elf

```
boot_task_name := $(patsubst "%",%,$(CONFIG_BOOT_TASK))     // CONFIG_BOOT_TASK=vm
boot_elf     := $(BUILD_DIR)/$(boot_task_name).elf
CFLAGS += -DBOOTELF_PATH='"$(boot_elf)"'

$(BUILD_DIR)/kernel/%.o: %.S Makefile $(boot_elf)
	$(PROGRESS) "CC" $<
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER) $(KERNEL_CFLAGS) $(INCLUDES) -Ikernel -Ikernel/arch/$(ARCH) -DKERNEL \
		-c -o $@ $< -MD -MF $(@:.o=.deps) -MJ $(@:.o=.json)
```

## resea/servers/vm/bootfs_image.S

```
#include <config.h>

.rodata
#ifdef CONFIG_ARCH_ARM64
.balign 4096
#else
.align 4096
#endif

.global __bootfs, __bootfs_end
__bootfs:
    .incbin BOOTFS_PATH                 // build/vm.elf
__bootfs_end:
```

## libs/resea/arm64/start.S

```
.text
.global _start
_start:                                 // <= entry_point
    // Set the stack pointer.
    ldr  x0, =__stack_end
    mov  sp, x0

    // Initialize the user library and run main().
    bl resea_init

.global halt
halt:
    b halt

.section .bootelf_header, "ax"
.global __bootelf_header, __bootinfo
__bootelf_header:
    .ascii "00BOOT\xe1\xff" // magic
    .space 16               // name
    .quad  0                // uint64_t entry
    .space 128              // mappings
__bootinfo:
    // struct bootinfo bootinfo;
    .space 512
```

## kernel/arch/arm64/machines/raspi3/kernel.ld

```
SECTIONS {
    .boot : {
        __kernel_image = .; /* paddr_t */
        KEEP(*(.boot));
    } > boot_ram
...
    .bss : ALIGN(8) {
        __bss = .;
        *(.bss);
        *(.bss.*);
        __bss_end = .;

        /* Kernel paging table. */
        . = ALIGN(4096);
        __kernel_page_table = . - LMA_OFFSET;
        . += 0x4000;

        /* Per-CPU boot stacks (paddr_t)/ */
        __boot_stack_base = . - LMA_OFFSET;
        . += 0x1000 * 16; /* PAGE_SIZE * CPU_NUM_MAX */

        . = ALIGN(4096);
        __kernel_image_end = . - LMA_OFFSET; /* paddr_t */
    } > ram AT > lma_ram
```

## kernel/arch/arm64/init.c

```c
static struct bootinfo bootinfo;

void arm64_init(void) {
    ...
    bootinfo.memmap[0].base = (vaddr_t) __kernel_image_end;
    bootinfo.memmap[0].len = 128 * 1024 * 1024;  // 128MiB
    bootinfo.memmap[0].type = BOOTINFO_MEMMAP_TYPE_AVAILABLE;
    kmain(&bootinfo);
   ...
}
```

## kernel/boot.c

```c
static struct bootelf_header *locate_bootelf_header(void) {
    const offset_t offsets[] = {
        0x1000,   // x64
        0x10000,  // arm64
    };

    for (size_t i = 0; i < sizeof(offsets) / sizeof(*offsets); i++) {
        struct bootelf_header *header =
            (struct bootelf_header *) &__bootelf[offsets[i]];
        if (!memcmp(header, BOOTELF_MAGIC, sizeof(header->magic))) {
            return header;
        }
    }

    PANIC("failed to locate the boot ELF header");
}

__noreturn void kmain(struct bootinfo *bootinfo) {
    ...
    struct bootelf_header *bootelf = locate_bootelf_header();
    strncpy2(name, (const char *) bootelf->name,
            MIN(sizeof(name), sizeof(bootelf->name)));

    // Copy the bootinfo struct to the boot elf header.
#ifndef CONFIG_NOMMU
    memcpy(&bootelf->bootinfo, bootinfo, sizeof(*bootinfo));
#endif

    // Create the first userland task.
    struct task *task = task_lookup_unchecked(INIT_TASK);
    ASSERT(task);
    error_t err = task_create(task, name, bootelf->entry, NULL, TASK_ALL_CAPS);
    ASSERT_OK(err);
    map_bootelf(bootinfo, bootelf, task);

    mpmain();
```

## libs/common/include/bootinfo.h

```c
#define BOOTINFO_MEMMAP_TYPE_END       0
#define BOOTINFO_MEMMAP_TYPE_AVAILABLE 0xaa
#define BOOTINFO_MEMMAP_TYPE_RESERVED  0xff

#define BOOTELF_MAGIC            "11BOOT\xe1\xff"
#define BOOTELF_NUM_MAPPINGS_MAX 8
#define NUM_BOOTINFO_MEMMAP_MAX 8

struct bootinfo_memmap_entry {
    uint8_t type;
    uint8_t reserved[3];
    uint64_t base;
    uint64_t len;
} __packed;

struct bootinfo {
    struct bootinfo_memmap_entry memmap[NUM_BOOTINFO_MEMMAP_MAX];
} __packed;

struct bootelf_mapping {
    uint64_t vaddr;
    uint32_t offset;
    uint16_t num_pages;
    uint8_t type;    // 'R': readonly, 'W': writable, 'X': executable.
    uint8_t zeroed;  // If it's non-zero value, the pages are filled with zeros.
} __packed;

struct bootelf_header {
    uint8_t magic[8];
    uint8_t name[16];
    uint64_t entry;
    struct bootelf_mapping mappings[BOOTELF_NUM_MAPPINGS_MAX];
    struct bootinfo bootinfo;
} __packed;
```

## build/vm.elf

```
                                                                        // struct bootelf_header
00001000: 3131 424f 4f54 e1ff 766d 0000 0000 0000  11BOOT..vm......     // magic = BOOTELF_MAGIC, name = "vm",
00001010: 0000 0000 0000 0000 1864 1100 0000 0000  .........d......     // entry = 0x116418
                                                                        // struct bootelf_mapping mappings[8];
00001020: 0000 1000 0000 0000 0010 0000 f204 5700  ..............W.     // [0]: 0x100000, 0x1000,   0x4f2, "W",0
00001030: 0020 5f00 0000 0000 0030 4f00 0d00 5700  . _......0O...W.     // [1]: 0x5f2000, 0x4f3000, 0xd0,  "W", 0
00001040: 0000 0003 0000 0000 0000 5000 3304 5701  ..........P.3.W.     // [2]: 0x300000, 0x500000, 0x433, "W", 1
00001050: 0000 0000 0000 0000 0000 0000 0000 0000  ................     // [3]:
00001060: 0000 0000 0000 0000 0000 0000 0000 0000  ................     // [4]:
00001070: 0000 0000 0000 0000 0000 0000 0000 0000  ................     // [5]:
00001080: 0000 0000 0000 0000 0000 0000 0000 0000  ................     // [6]:
00001090: 0000 0000 0000 0000 0000 0000 0000 0000  ................     // [7]:
000010a0: 0000 0000 0000 0000 0000 0000 0000 0000  ................     // struct bootinfobootinfo;
000010b0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000010c0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
```

```
$ aarch64-none-elf-readelf -h vm.elf
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x116418
  Start of program headers:          64 (bytes into file)
  Start of section headers:          5244416 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         3
  Size of section headers:           64 (bytes)
  Number of section headers:         8
  Section header string table index: 7
  ```

# bootfs

```
bootfs_bin   := $(BUILD_DIR)/bootfs.bin

$(bootfs_bin): $(bootfs_files) tools/mkbootfs.py
	$(PROGRESS) "MKBOOTFS" $@
	$(PYTHON3) tools/mkbootfs.py -o $@ $(bootfs_files)

# bootfs_files = build/dm.elf build/hello.elf build/memory_leak_test.elf \
                 build/ramdisk.elf build/random.elf build/shell.elf
```

## bootfs.bin

```
00000000: 0100 0000 1000 0000 0600 0000 0000 0000  ................     // struct bootfs_header
00000010: 646d 0000 0000 0000 0000 0000 0000 0000  dm..............     // struct bootfs_file
00000020: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000030: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000050: 6865 6c6c 6f00 0000 0000 0000 0000 0000  hello...........     // struct bootfs_file
00000060: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000030: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000040: 0010 0000 7847 0200 0000 0000 0000 0000  ....xG..........

00001000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
00026000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
00047000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
00068000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
0048b000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
004ad000: 3131 424f 4f54 e1ff 7368 656c 6c00 0000  11BOOT..shell...
```

## servers/vm/bootfs.h

```c
struct bootfs_header {
    uint32_t version;           // 0x1
    uint32_t files_off;         // 0x10
    uint32_t num_files;         // 0x06
    uint32_t padding;
} __packed;

struct bootfs_file {
    char name[48];              // dm,      hello,   memory_leak_test, ramdisk,  random,   shell
    uint32_t offset;            // 0x1000,  0x26000, 0x47000,          0x68000,  0x48b000, 0x4ad000
    uint32_t len;               // 0x24778, 0x20670, 0x20670,          0x422670, 0x20670,  0x226d0
    uint8_t padding[8];
} __packed;
```

## kernel_image

```
kernel_image := $(BUILD_DIR)/resea.elf

$(kernel_image): $(kernel_objs) $(BUILD_DIR)/kernel/__name__.o $(kernel_ld) \
		tools/nm2symbols.py tools/embed-symbols.py Makefile
	$(PROGRESS) "LD" $@
	$(LD) $(LDFLAGS) --script=$(kernel_ld) -Map $(@:.elf=.map) -o $@.tmp \
		$(kernel_objs) $(BUILD_DIR)/kernel/__name__.o
	$(PROGRESS) "GEN" $(@:.elf=.symbols)
	$(NM) $@.tmp | ./tools/nm2symbols.py > $(@:.elf=.symbols)
	$(PROGRESS) "SYMBOLS" $@
	./tools/embed-symbols.py $(@:.elf=.symbols) $@.tmp
	cp $@.tmp $@

# kernel_objs = build/kernel/kernel/boot.o build/kernel/kernel/task.o build/kernel/kernel/ipc.o build/kernel/kernel/syscall.o build/kernel/kernel/printk.o build/kernel/kernel/kdebug.o build/kernel/kernel/arch/arm64/trap.o build/kernel/kernel/arch/arm64/init.o build/kernel/kernel/arch/arm64/vm.o build/kernel/kernel/arch/arm64/mp.o build/kernel/kernel/arch/arm64/task.o build/kernel/kernel/arch/arm64/switch.o build/kernel/kernel/arch/arm64/interrupt.o build/kernel/kernel/arch/arm64/usercopy.o build/kernel/kernel/arch/arm64/machines/raspi3/boot.o build/kernel/kernel/arch/arm64/machines/raspi3/peripherals.o build/kernel/kernel/arch/arm64/machines/raspi3/mp.o build/kernel/libs/common/string.o build/kernel/libs/common/vprintf.o build/kernel/libs/common/ubsan.o build/kernel/libs/common/bitmap.o
```

## resea.elf.tmp (変換前)

```
00513580: 5359 4db0 0002 0000 0000 0000 0000 0000  SYM.............     // SYMBOL_TABLE_EMPTY, int num_symbols
00513590: 0000 0000 0000 0000 0000 0000 0000 0000  ................

0051b590: 0000 0b00 2769 6e74 3332 5f74 2720 2861  ....'int32_t' (a     // 0x513590 + 0x40 + 0x200
0051b5a0: 6b61 2027 696e 7427 2900 0000 0000 0000  ka 'int').......
```

## resea.elf

```
00000000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
...                                                                     // 以下、シンボル埋め込み
00513580: 5359 4d4c 9300 0000 0000 0000 0000 0000  SYML............     // SYMBOL_TABLE_MAGIC, int len(symbols), long pad
00513590: 0000 0800 0000 0000 626f 6f74 0000 0000  ........boot....     // long addr, char name[56]
005135a0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
005135b0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
005135c0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
005135d0: 2400 0800 0000 0000 656c 315f 6d6f 6465  $.......el1_mode     // long addr, char name[56]
005135e0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
005135f0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00513600: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00513610: 4400 0800 0000 0000 6170 5f62 6f6f 7400  D.......ap_boot.     // long addr, char name[56]
...                                                                     // 以下、オリジナルデータ
0051b590: 0000 0b00 2769 6e74 3332 5f74 2720 2861  ....'int32_t' (a
0051b5a0: 6b61 2027 696e 7427 2900 0000 0000 0000  ka 'int').......
```

## sort resea.symbols

```
0000000000080000 __kernel_image
0000000000080000 boot
0000000000080024 el1_mode
0000000000080044 ap_boot
```
