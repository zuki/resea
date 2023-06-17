# BootElfとBootFSについて

```c
BOOTELF_PATH = build/vm.elf
BOOTFS_PATH  = build/bootfs_bin
```

## BootElf

カーネルイメージに最初のタスクであるvmを埋め込んで実行するための機構

```Makefile
boot_task_name := $(patsubst "%",%,$(CONFIG_BOOT_TASK))     // CONFIG_BOOT_TASK=vm
boot_elf     := $(BUILD_DIR)/$(boot_task_name).elf          // boot_elf = vm.elf
CFLAGS += -DBOOTELF_PATH='"$(boot_elf)"'

$(BUILD_DIR)/kernel/%.o: %.S Makefile $(boot_elf)
	$(PROGRESS) "CC" $<
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(SANITIZER) $(KERNEL_CFLAGS) $(INCLUDES) -Ikernel -Ikernel/arch/$(ARCH) -DKERNEL \
		-c -o $@ $< -MD -MF $(@:.o=.deps) -MJ $(@:.o=.json)
```

### カーネルイメージ

```
+--------------------------+
| .boot                    |
|   __kernel_image         |
| .text : ALIGN(8)         |
| .rodata : ALIGN(8)       |
|  __bootelf : ALIGN(4096) |    // vm.elfを埋め込む: offset = 0x1000になる
|    (vm.elf)              |
| . = ALIGN(4096)          |
| .data : ALIGN(8)         |
| .bss : ALIGN(8)          |
|  __bss                   |
|  __bss_end               |
| . = ALIGN(4096)          |
| __kernel_page_table      |
|    (4KiB)                |
| __boot_stack_base        |
|    (16KiB)               |
| __kernel_image_end       |
| bootinfo.memmap[0]       |
|    (128MiB)              |
+--------------------------+
```

### kernel/arch/arm64/machines/raspi3/boot.S

```
.section ".boot", "ax"
.global boot
boot:

[省略]

// 最初のタスク(vm.elf)を.rodataに埋め込む
.rodata
.balign 4096
.global __bootelf
__bootelf:
    .incbin BOOTELF_PATH
.balign 4096
```

### ユーザイメージ

- vm.elfはユーザランド

```
+--------------------------+
| . = 0x10_0000            |
| .text                    |
|    *(.bootelf_header)    |
|    *(.text*)             |
| .rodata                  |
|    (ramdisk, etc)        |    // ramdiskなど大量のデータが埋め込まれる場合がある
| . = ALIGN(4096)          |    // bootfsはvm.elfにだけ存在する
|  __bootfs:               |
|    (bootfs_bin)          |
|  __bootfs_end            |
| .data : ALIGN(4096)      |
|    *(.data*)             |
|  __stack : ALIGN(16)     |
|     (4KiB)               |
|  __stack_end             |
| . = 0x80_0000            |
|  __zeroed_pages          |
| .bss : ALIGN(4096)       |
|  __heap                  |
|    (20KiB)               |
|  __heap_end              |
|  __bss                   |
|  __bss_end               |
| . = 0x2ff_f000           |
|  __cmdline               |
| . = 0x300_0000           |
|  __zeroed_pages_end      |
|  __straight_mapping      |
|  __free_vaddr            |
|    (160MiB)              |
|  __free_vaddr_end        |
+--------------------------+
```

## libs/common/include/bootinfo.h

- BootELFヘッダーの定義

```c
#define BOOTINFO_MEMMAP_TYPE_END       0
#define BOOTINFO_MEMMAP_TYPE_AVAILABLE 0xaa
#define BOOTINFO_MEMMAP_TYPE_RESERVED  0xff

#define BOOTELF_MAGIC               "11BOOT\xe1\xff"
#define BOOTELF_NUM_MAPPINGS_MAX    8
#define NUM_BOOTINFO_MEMMAP_MAX     8

struct bootinfo_memmap_entry {
    uint8_t type;
    uint8_t reserved[3];
    uint64_t base;
    uint64_t len;
} __packed;

struct bootinfo {
    struct bootinfo_memmap_entry memmap[NUM_BOOTINFO_MEMMAP_MAX];   // 20 * 8 = 160 byte
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
    struct bootelf_mapping mappings[BOOTELF_NUM_MAPPINGS_MAX];  // 16 * 8 = 128 byte
    struct bootinfo bootinfo;                                   // 最大512 (160バイト定義済み)
} __packed;
```

### libs/resea/arch/arm64/start.S

- BootELFヘッダー用のスペースを確保して目印(`00BOOT\xe1\xff`)をつけておく
- make build時に`tools/embed-bootelf-header.py`でデータが埋め込まれる

```
.text
.global _start
_start:

[省略]

.section .bootelf_header, "ax"
.global __bootelf_header, __bootinfo
__bootelf_header:           // struct bootelf_header
    .ascii "00BOOT\xe1\xff" //   magic
    .space 16               //   name
    .quad  0                //   uint64_t entry
    .space 128              //   mappings
__bootinfo:
    .space 512              //   bootinfo
```

### tools/embed-botelf-header,py

```python
PAGE_SIZE = 0x1000  # FIXME: This could be other values.
BOOTELF_NUM_MAPPINGS_MAX = 8
BOOTELF_PRE_MAGIC = b"00BOOT\xe1\xff"
BOOTELF_MAGIC = b"11BOOT\xe1\xff"

def main():
    elf = ELFFile(open(args.elf_file, "r+b"))
    header = struct.pack("8s16sQ", BOOTELF_MAGIC, args.name.encode("ascii"),
                         elf.header.e_entry)

    for segment in elf.iter_segments():
        vaddr = segment.header.p_vaddr
        offset = segment.header.p_offset
        num_pages = (segment.header.p_memsz + PAGE_SIZE - 1) // PAGE_SIZE
        zeroed = segment.header.p_filesz == 0
        header += struct.pack("QIHcb", vaddr, offset, num_pages,
                              b'W',  # TODO: Support so-called W^X
                              1 if zeroed else 0)

    with open(args.elf_file, "r+b") as f:
        for offset in [0x1000, 0x10000]:
            f.seek(offset)
            if f.read(8) == BOOTELF_PRE_MAGIC:
                break

        f.seek(offset)
        assert f.read(8) == BOOTELF_PRE_MAGIC
        f.seek(offset)
        f.write(header)
```

### kernel/arch/arm64/init.c

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

### kernel/boot.c

```c
__noreturn void kmain(struct bootinfo *bootinfo) {
    ...
    struct bootelf_header *bootelf = locate_bootelf_header();
    strncpy2(name, (const char *) bootelf->name,
            MIN(sizeof(name), sizeof(bootelf->name)));

    // 引数として渡されたbootoinfoをbooteflヘッダーにコピーする
#ifndef CONFIG_NOMMU
    memcpy(&bootelf->bootinfo, bootinfo, sizeof(*bootinfo));
#endif

    // 最初のユーザランドタスク (vm) を作成する
    struct task *task = task_lookup_unchecked(INIT_TASK);
    ASSERT(task);
    error_t err = task_create(task, name, bootelf->entry, NULL, TASK_ALL_CAPS);
    ASSERT_OK(err);
    map_bootelf(bootinfo, bootelf, task);

    mpmain();
}

// カーネルイメージからvm.elfのbootelf_headerを取り出す
static struct bootelf_header *locate_bootelf_header(void) {
    const offset_t offsets[] = {
        0x1000,   // x64
        0x10000,  // arm64 （実際はarm64も0x1000）
    };

    for (size_t i = 0; i < sizeof(offsets) / sizeof(*offsets); i++) {
        // __bootelf はカーネルイメージの.rodataにあり、内容は vm.elf
        struct bootelf_header *header =
            (struct bootelf_header *) &__bootelf[offsets[i]];
        if (!memcmp(header, BOOTELF_MAGIC, sizeof(header->magic))) {
            return header;
        }
    }

    PANIC("failed to locate the boot ELF header");
}
```


### build/vm.elf

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
$ aarch64-none-elf-readelf -h build/vm.elf
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
  Entry point address:               0x1166b8
  Start of program headers:          64 (bytes into file)
  Start of section headers:          4978168 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         3
  Size of section headers:           64 (bytes)
  Number of section headers:         8
  Section header string table index: 7

$ aarch64-none-elf-readelf -lW vm.elf
Program Headers:
  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align
  LOAD           0x001000 0x0000000000100000 0x0000000000100000 0x4b0ed4 0x4b0ed4 R E 0x1000
  LOAD           0x4b2000 0x00000000005b1000 0x00000000005b1000 0x00c7f0 0x00c7f0 RW  0x1000
  LOAD           0x4bf000 0x0000000000800000 0x0000000000800000 0x000000 0x4326d0 RW  0x1000

// 実行時ログ
[kernel] new task #1: vm (pager=(null))
[kernel] boot ELF: entry=00000000001166b8                                   // vm.elfのentry point
[kernel] boot ELF: 0000000000100000 -> 0000000000091000 (4804KiB)           // PHD 1
[kernel] boot ELF: 00000000005b1000 -> 0000000000542000 (52KiB)             // PHD 2
[kernel] boot ELF: 0000000000800000 -> 0000000000000000 (4300KiB, zeroed)   // PHD 3
```

### kernel_image

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

### resea.elf.tmp (変換前)

```
00513580: 5359 4db0 0002 0000 0000 0000 0000 0000  SYM.............     // SYMBOL_TABLE_EMPTY, int num_symbols
00513590: 0000 0000 0000 0000 0000 0000 0000 0000  ................

0051b590: 0000 0b00 2769 6e74 3332 5f74 2720 2861  ....'int32_t' (a     // 0x513590 + 0x40 + 0x200
0051b5a0: 6b61 2027 696e 7427 2900 0000 0000 0000  ka 'int').......
```

### resea.elf

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

### sort resea.symbols

```
0000000000080000 __kernel_image
0000000000080000 boot
0000000000080024 el1_mode
0000000000080044 ap_boot
```

## BootFS

- システム起動時に指定のservers/appsを起動するための機構
- vm.elfの.rodata領域に起動ファイルを埋め込んでvmが起動する

```
bootfs_bin   := $(BUILD_DIR)/bootfs.bin

$(bootfs_bin): $(bootfs_files) tools/mkbootfs.py
	$(PROGRESS) "MKBOOTFS" $@
	$(PYTHON3) tools/mkbootfs.py -o $@ $(bootfs_files)

# bootfs_files = build/dm.elf build/hello.elf build/memory_leak_test.elf \
                 build/ramdisk.elf build/random.elf build/shell.elf
```

### servers/vm/bootfs_image.S

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
    .incbin BOOTFS_PATH                 // build/bootfs.bin
__bootfs_end:
```

### bootfs.bin

```
00000000: 0100 0000 1000 0000 0400 0000 0000 0000  ................     // struct bootfs_header
00000010: 646d 0000 0000 0000 0000 0000 0000 0000  dm..............     // dm: struct bootfs_file
00000020: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000030: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000040: 0010 0000 7047 0200 0000 0000 0000 0000  ....pG..........     //   offset: 0x1000, len: 0x24770
00000050: 6865 6c6c 6f00 0000 0000 0000 0000 0000  hello...........     // hello:
00000060: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000070: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000080: 0060 0200 6806 0200 0000 0000 0000 0000  .`..h...........     //   offset: 0x26000, len: 0x20668
00000090: 7261 6d64 6973 6b00 0000 0000 0000 0000  ramdisk.........     // ramdisk:
000000a0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000b0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000c0: 0070 0400 6816 4200 0000 0000 0000 0000  .p..h.B.........     //   offset: 0x47000, len: 0x421668
000000d0: 7368 656c 6c00 0000 0000 0000 0000 0000  shell...........     // shell:
000000e0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
000000f0: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000100: 0090 4600 8047 0200 0000 0000 0000 0000  ..F..G..........     //   offset: 0x469000, len: 0x24780


00001000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
00026000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
00047000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
00469000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
```

### servers/vm/bootfs.h

```c
struct bootfs_header {
    uint32_t version;           // 0x1
    uint32_t files_off;         // 0x10
    uint32_t num_files;         // 0x04
    uint32_t padding;
} __packed;

struct bootfs_file {
    char name[48];
    uint32_t offset;
    uint32_t len;
    uint8_t padding[8];
} __packed;
```

### servers/vm/main.c

```c
void main(void) {
    TRACE("starting...");
    bootfs_init();
    page_alloc_init();
    task_init();
    page_fault_init();
    spawn_servers();
...
}

// 自動起動に指定されているservers/appsを起動する
static void spawn_servers(void) {
    // bootfsにあるサーバを起動する
    int num_launched = 0;
    struct bootfs_file *file;
    for (int i = 0; (file = bootfs_open(i)) != NULL; i++) {
        char *startups = AUTOSTARTS;    // makeに設定: AUTOSTARTS="dm hello ramdisk shell"

        // AUTOSTARTSに設定されているファイルだったら実行
        while (*startups != '\0') {
            size_t len = strlen(file->name);
            if (!strncmp(file->name, startups, len)
                && (startups[len] == '\0' || startups[len] == ' ')) {
                ASSERT_OK(task_spawn(file, ""));
                num_launched++;
                break;
            }
            ...
        }
    }
    ...
}
```

### servers/vm/bootfs.c

```c
extern char __bootfs[];
static struct bootfs_file *files;
static unsigned num_files;

void read_file(struct bootfs_file *file, offset_t off, void *buf, size_t len) {
    void *p = (void *) (((uintptr_t) __bootfs) + file->offset + off);
    memcpy(buf, p, len);
}

struct bootfs_file *bootfs_open(unsigned index) {
    if (index >= num_files) {
        return NULL;
    }

    return &files[index];
}

void bootfs_init(void) {
    struct bootfs_header *header = (struct bootfs_header *) __bootfs;
    num_files = header->num_files;
    files =
        (struct bootfs_file *) (((uintptr_t) &__bootfs) + header->files_off);
}
```
