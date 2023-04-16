/** @file bootinfo.h */
#ifndef __BOOTINFO_H__
#define __BOOTINFO_H__

#include <types.h>

/** @ingroup kernel
 * @def BOOTINFO_MEMMAP_TYPE_END
 * @brief ブート情報メモリマップタイプ: 終了
 */
#define BOOTINFO_MEMMAP_TYPE_END       0
/** @ingroup kernel
 * @def BOOTINFO_MEMMAP_TYPE_AVAILABLE
 * @brief ブート情報メモリマップタイプ: 利用可能
 */
#define BOOTINFO_MEMMAP_TYPE_AVAILABLE 0xaa
/** @ingroup kernel
 * @def BOOTINFO_MEMMAP_TYPE_RESERVED
 * @brief ブート情報メモリマップタイプ: 予約済み
 */
#define BOOTINFO_MEMMAP_TYPE_RESERVED  0xff

/** @ingroup kernel
 * @struct bootelf_mapping
 * @brief bootelfマッピング構造体 (16バイト)
 */
struct bootelf_mapping {
    uint64_t vaddr;         /**< 仮想アドレス */
    uint32_t offset;        /**< オフセット */
    uint16_t num_pages;     /**< ページ数 */
    uint8_t type;           /**< タイプ: 'R': readonly, 'W': writable, 'X': executable. */
    uint8_t zeroed;         /**< 0詰めの有無: non-zero: 0詰め */
} __packed;

/** @ingroup kernel
 * @struct bootinfo_memmap_entry
 * @brief ブート情報メモリマップエントリ構造体 (20バイト)
 */
struct bootinfo_memmap_entry {
    uint8_t type;           /**< タイプ: BOOTINFO_MEMMAP_TYPE_x */
    uint8_t reserved[3];    /**< 無視 */
    uint64_t base;          /**< ベースアドレス */
    uint64_t len;           /**< 長さ */
} __packed;

/** @ingroup kernel
 * @def NUM_BOOTINFO_MEMMAP_MAX
 * @brief ブート情報メモリマップ要素の最大数
 */
#define NUM_BOOTINFO_MEMMAP_MAX 8

/** @ingroup kernel
 * @struct bootinfo
 * @brief ブート情報構造体
 */
struct bootinfo {
    struct bootinfo_memmap_entry memmap[NUM_BOOTINFO_MEMMAP_MAX];   /**< ブート情報メモリマップ要素の配列 */
} __packed;

// 最大サイズは libs/resea/arch/arm64/start.Sに
// ハードコーディングされている (512)
STATIC_ASSERT(sizeof(struct bootinfo) <= 512);

/** @ingroup kernel
 * @def BOOTELF_MAGIC
 * @brief bootelfを表すマジックナンバー
 */
#define BOOTELF_MAGIC            "11BOOT\xe1\xff"

/** @ingroup kernel
 * @def BOOTELF_NUM_MAPPINGS_MAX
 * @brief bootelfマッピングエントリの最大数
 */
#define BOOTELF_NUM_MAPPINGS_MAX 8

/** @ingroup kernel
 * @struct bootelf_header
 * @brief bootelfヘッダー構造体
 */
struct bootelf_header {
    uint8_t magic[8];   /**< マジックナンバー: BOOTELF_MAGICのはず */
    uint8_t name[16];   /**< プログラム名 */
    uint64_t entry;     /**< エントリポイント */
    struct bootelf_mapping mappings[BOOTELF_NUM_MAPPINGS_MAX];  /**< マッピング配列 */
    struct bootinfo bootinfo;   /**< ブート情報構造体 */
} __packed;

#endif
