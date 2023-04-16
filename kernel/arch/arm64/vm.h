/** @file vm.h */
#ifndef __ARM64_VM_H__
#define __ARM64_VM_H__

/** @ingroup arm64
 * @def NTH_LEVEL_INDEX
 * @brief 仮想アドレスから指定レベルのページインデックスを返す.
 */
#define NTH_LEVEL_INDEX(level, vaddr)                                          \
    (((vaddr) >> ((((level) -1) * 9) + 12)) & 0x1ff)

/** @ingroup arm64
 * @def ENTRY_PADDR
 * @brief エントリの物理アドレス（造成部分は0）を返す.
 */
#define ENTRY_PADDR(entry) ((entry) &0x0000fffffffff000)

/** @ingroup arm64
 * @def ARM64_PAGE_TABLE
 * @brief ページテーブルを示す
 */
#define ARM64_PAGE_TABLE  0x3
/** @ingroup arm64
 * @def ARM64_PAGE_ACCESS
 * @brief アクセスフラグ（アクセスファルトを発生させない）
 */
#define ARM64_PAGE_ACCESS (1ULL << 10)
/** @ingroup arm64
 * @def ARM64_PAGE_MEMATTR_READONLY
 * @brief カーネル、ユーザ共にREADONLY
 */
#define ARM64_PAGE_MEMATTR_READONLY (0b11 << 6)
/** @ingroup arm64
 * @def ARM64_PAGE_MEMATTR_READWRITE
 * @brief カーネル、ユーザ共に読み書き可能
 */
#define ARM64_PAGE_MEMATTR_READWRITE (0b01 << 6)

#endif
