/** @file vm.c */
#include "vm.h"
#include <arch.h>
#include <printk.h>
#include <string.h>
#include <syscall.h>
#include <task.h>

/** @ingroup arm64
 * @brief ページテーブルを走査してPTEを返す.
 * @param table ページテーブルへのポインタ
 * @param vaddr 仮想アドレスへのポインタ
 * @param kpage ページアドレス（物理アドレス）
 * @param attrs ページ属性
 * @return PTE（レベル1ページテーブル）へのアドレス。設定されていない場合は NULL
 */
static uint64_t *traverse_page_table(uint64_t *table, vaddr_t vaddr,
                                     paddr_t kpage, uint64_t attrs) {
    ASSERT(vaddr < KERNEL_BASE_ADDR);
    ASSERT(IS_ALIGNED(vaddr, PAGE_SIZE));
    ASSERT(IS_ALIGNED(kpage, PAGE_SIZE));

    // * +-----9-----+-----9-----+-----9-----+-----9-----+---------12---------+
    // * |  L4 index |  L3 index |  L3 index |  L1 index | Offset within Page |
    // * +-----------+-----------+-----------+-----------+--------------------+

    for (int level = 4; level > 1; level--) {
        int index = NTH_LEVEL_INDEX(level, vaddr);
        // 当該レベルのテーブルがない
        if (!table[index]) {
            // 属性が指定されていなかったら何もせずNULLを返す
            if (!attrs) {
                return NULL;
            }
            // 属性が指定されてされていたらkpageを0クリアしてテーブル表に登録
            memset(paddr2ptr(kpage), 0, PAGE_SIZE);
            table[index] = kpage;
            return NULL;
        }

        // 属性が指定されていたら更新する
        table[index] |= attrs | ARM64_PAGE_ACCESS | ARM64_PAGE_TABLE;

        // 次のレベルのページテーブルに入る
        table = (uint64_t *) paddr2ptr(ENTRY_PADDR(table[index]));
    }
    // Level 1のテーブル表のアドレスを返す
    return &table[NTH_LEVEL_INDEX(1, vaddr)];
}

/** @ingroup arm64
 * @brief 仮想アドレスに物理アドレスをマッピングする.
 * @param task タスクへのポインタ
 * @param vaddr 仮想アドレスへのポインタ
 * @param paddr 物理アドレスへのポインタ
 * @param kpage ページアドレス（物理アドレス）
 * @param flags フラグ
 * @return 正常に処理できたら ::OK; それ以外は ::error_t 型のエラーコード
 */
error_t arch_vm_map(struct task *task, vaddr_t vaddr, paddr_t paddr,
                    paddr_t kpage, unsigned flags) {
    ASSERT(IS_ALIGNED(paddr, PAGE_SIZE));

    uint64_t attrs;
    // 1. マップ種別（bit[1:0]）に基づき属性値を設定する
    switch (MAP_TYPE(flags)) {
        case MAP_TYPE_READONLY:
            attrs = ARM64_PAGE_MEMATTR_READONLY;
            break;
        case MAP_TYPE_READWRITE:
            attrs = ARM64_PAGE_MEMATTR_READWRITE;
            break;
        default:
            UNREACHABLE();
    }

    // 2. PTEを取得する
    uint64_t *entry =
        traverse_page_table(task->arch.page_table, vaddr, kpage, attrs);
    if (!entry) {
        return (kpage) ? ERR_TRY_AGAIN : ERR_EMPTY;
    }

    // 3. PTEに値をセットする
    *entry = paddr | attrs | ARM64_PAGE_ACCESS | ARM64_PAGE_TABLE;

    // FIXME: Flush only the affected page.
    __asm__ __volatile__("dsb ish");
    __asm__ __volatile__("isb");
    __asm__ __volatile__("tlbi vmalle1is");
    __asm__ __volatile__("dsb ish");
    __asm__ __volatile__("isb");
    return OK;
}

/** @ingroup arm64
 * @brief 仮想アドレスをアンマップする.
 * @param task タスクへのポインタ
 * @param vaddr 仮想アドレスへのポインタ
 * @return 正常に処理できたら ::OK; それ以外は ::error_t 型のエラーコード
 */
error_t arch_vm_unmap(struct task *task, vaddr_t vaddr) {
    uint64_t *entry = traverse_page_table(task->arch.page_table, vaddr, 0, 0);
    if (!entry) {
        return ERR_NOT_FOUND;
    }

    // 3. PTEの値をクリアする
    *entry = 0;
    // FIXME: Flush only the affected page.
    __asm__ __volatile__("dsb ish");
    __asm__ __volatile__("isb");
    __asm__ __volatile__("tlbi vmalle1is");
    __asm__ __volatile__("dsb ish");
    __asm__ __volatile__("isb");
    return OK;
}

/** @ingroup arm64
 * @brief 仮想アドレスを物理アドレスに解決する.
 * @param task タスクへのポインタ
 * @param vaddr 仮想アドレスへのポインタ
 * @return 正常に処理できたら ::OK; それ以外は ::error_t 型のエラーコード
 */
paddr_t vm_resolve(struct task *task, vaddr_t vaddr) {
    uint64_t *entry = traverse_page_table(task->arch.page_table, vaddr, 0, 0);
    return (entry) ? ENTRY_PADDR(*entry) : 0;
}
