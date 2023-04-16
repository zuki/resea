/** @file boot.S */
#include "boot.h"
#include "kdebug.h"
#include "printk.h"
#include "syscall.h"
#include "task.h"
#include <bootinfo.h>
#include <config.h>
#include <string.h>

// Defined in arch.
extern uint8_t __bootelf[];
extern uint8_t __bootelf_end[];

/** @ingroup kernel
 * @brief bootelfヘッダーを取得する
 * @return bootelfヘッダーへのポインタ. 規定の位置にヘッダーがない場合はpanic.
 */
static struct bootelf_header *locate_bootelf_header(void) {
    const offset_t offsets[] = {
        0x1000,   // x64
        0x10000,  // arm64
    };

    for (size_t i = 0; i < sizeof(offsets) / sizeof(*offsets); i++) {
        struct bootelf_header *header =
            (struct bootelf_header *) &__bootelf[offsets[i]];
        if (!memcmp(header, BOOTELF_MAGIC, sizeof(header->magic))) {
            // FIXME: arm64でもoffsets[0]にヒットしている模様
            //INFO("locate_bootelf[%d]: %s 0x%p\n", i, header->name, header);
            return header;
        }
    }

    PANIC("failed to locate the boot ELF header");
}

#if !defined(CONFIG_NOMMU)
/** @ingroup kernel
 * @brief 最初のユーザタスク用にメモリページを割り当てる.
 * @param bootinfo ブート情報構造体へのポインタ
 * @return メモリページへのポインタ（仮想アドレス）
 */
static void *alloc_page(struct bootinfo *bootinfo) {
    for (int i = 0; i < NUM_BOOTINFO_MEMMAP_MAX; i++) {
        struct bootinfo_memmap_entry *m = &bootinfo->memmap[i];
        if (m->type != BOOTINFO_MEMMAP_TYPE_AVAILABLE) {
            continue;
        }
        // 利用可能なメモリマップ要素の現在のbaseから1ページ割り当てる
        if (m->len >= PAGE_SIZE) {
            ASSERT(IS_ALIGNED(m->base, PAGE_SIZE));
            void *ptr = paddr2ptr(m->base);
            m->base += PAGE_SIZE;
            m->len -= PAGE_SIZE;
            return ptr;
        }
    }

    PANIC("run out of memory for the initial task's memory space");
}

/** @ingroup kernel
 * @brief boot ELFのELFセグメントを仮想メモリにマッピングする.
 * @param bootinof ブート情報へのポインタ
 * @param task タスクへのポインタ
 * @param vaddr 仮想アドレス
 * @param paddr 物理アドレス
 * @param flags MAP_TYPEフラグ
 * @return 成功した場合はアドレス（NULLもあり）、
 *         それ以外はerror_t型のエラーコード
 */
static error_t map_page(struct bootinfo *bootinfo, struct task *task,
                        vaddr_t vaddr, paddr_t paddr, unsigned flags) {
    static paddr_t unused_kpage = 0;
    while (true) {
        paddr_t kpage =
            unused_kpage ? unused_kpage : ptr2paddr(alloc_page(bootinfo));
        error_t err = vm_map(task, vaddr, paddr, kpage, MAP_TYPE_READWRITE);
        // TODO: Free the unused `kpage`.
        if (err == ERR_TRY_AGAIN) {
            unused_kpage = 0;
            continue;
        }

        unused_kpage = kpage;
        return err;
    }
}
#endif

/** @ingroup kernel
 * @brief boot ELFのELFセグメントを仮想メモリにマッピングする.
 * @param bootinof ブート情報へのポインタ
 * @param header ブートELFヘッダーへのポインタ
 * @param task タスクへのポインタ
 */
static void map_bootelf(struct bootinfo *bootinfo,
                        struct bootelf_header *header, struct task *task) {
    TRACE("boot ELF: entry=%p", header->entry);
    for (unsigned i = 0; i < BOOTELF_NUM_MAPPINGS_MAX; i++) {
        struct bootelf_mapping *m = &header->mappings[i];
        vaddr_t vaddr = m->vaddr;
        paddr_t paddr = ptr2paddr(&__bootelf[m->offset]);

        if (!m->vaddr) {
            continue;
        }

        TRACE("boot ELF: %p -> %p (%dKiB%s)", vaddr, (m->zeroed) ? 0 : paddr,
              m->num_pages * PAGE_SIZE / 1024, (m->zeroed) ? ", zeroed" : "");

#ifdef CONFIG_NOMMU
        if (m->zeroed) {
            memset((void *) vaddr, 0, m->num_pages * PAGE_SIZE);
        } else {
            if (vaddr == paddr) {
                continue;
            }
            memcpy((void *) vaddr, (void *) paddr, m->num_pages * PAGE_SIZE);
        }
#else
        ASSERT(IS_ALIGNED(vaddr, PAGE_SIZE));
        ASSERT(IS_ALIGNED(paddr, PAGE_SIZE));

        if (m->zeroed) {
            // (.bss, __zeroed_pages_endまで) ページを割り当て、0詰めして、マッピング
            for (size_t j = 0; j < m->num_pages; j++) {
                void *page = alloc_page(bootinfo);
                ASSERT(page);
                memset(page, 0, PAGE_SIZE);
                error_t err = map_page(bootinfo, task, vaddr, ptr2paddr(page),
                                       MAP_TYPE_READWRITE);
                ASSERT_OK(err);
                vaddr += PAGE_SIZE;
            }
        } else {
            // マッピングだけ (.text, .rodata, .data)
            for (size_t j = 0; j < m->num_pages; j++) {
                error_t err =
                    map_page(bootinfo, task, vaddr, paddr, MAP_TYPE_READWRITE);
                ASSERT_OK(err);
                vaddr += PAGE_SIZE;
                paddr += PAGE_SIZE;
            }
        }
#endif
    }
}

/** @ingroup kernel
 * @brief カーネルを初期化して最初のタスクを開始する.
 * @param bootinfo
 */
__noreturn void kmain(struct bootinfo *bootinfo) {
    // 1. Resea起動表示
    printf("\nBooting Resea " VERSION " (" GIT_REVISION ")...\n");
    // 2. タスクサブシステムの初期化
    task_init();
    // 3. cpu1-3を起動する
    mp_start();

    // 4. boot elf headerを探す.
    char name[CONFIG_TASK_NAME_LEN];
    struct bootelf_header *bootelf = locate_bootelf_header();
    strncpy2(name, (const char *) bootelf->name,
            MIN(sizeof(name), sizeof(bootelf->name)));

    // 5. bootinfoをboot elf headerにコピーする
#ifndef CONFIG_NOMMU
    // FIXME: Support NOMMU: bootelf->bootinfo could exist in ROM.
    memcpy(&bootelf->bootinfo, bootinfo, sizeof(*bootinfo));
#endif

    // 6. 最初のユーザランドタスクを作成する
    // 6.1 task構造体を取得する
    struct task *task = task_lookup_unchecked(INIT_TASK);
    ASSERT(task);
    // 6.2 taskを作成する
    error_t err = task_create(task, name, bootelf->entry, NULL, TASK_ALL_CAPS);
    ASSERT_OK(err);
    // 6.3 bootelfを仮想空間にマッピングする
    map_bootelf(bootinfo, bootelf, task);
    // 7. cpuのメイン処理
    mpmain();
}

/** @ingroup kernel
 * @brief 各CPUのメイン処理.
 */
__noreturn void mpmain(void) {
    // 1. スタックが溢れていないかチェック
    stack_set_canary();

    // 2. このCPU用のアイドルタスクを初期化する
    // 2.1 アイドルタスクのタスクIDは0
    IDLE_TASK->tid = 0;
    // 2.2 アイドルタスクを作成
    error_t err = task_create(IDLE_TASK, "(idle)", 0, NULL, 0);
    ASSERT_OK(err);
    // 2.3 カレントタスクをアイドルタスクとする
    CURRENT = IDLE_TASK;

    // 3. ブートは完了
    INFO("Booted CPU #%d", mp_self());
    // 4. 現在のところ、cpu1-3はmpmain()は呼び出されないので
    //    panicにはならないはず
    if (!mp_is_bsp()) {
        PANIC("TODO: Support context switching in SMP");
    }
    // 5. コンテキストスイッチを可能にし、割り込みを有効にして割り込みを待つ
    arch_idle();
}
