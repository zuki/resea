/** @file syscall.h */
#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <message.h>
#include <types.h>

///
/** @ingroup kernel
 * @def __user
 * @brief ユーザにより与えられるポインタ用の属性. 直接、
 * 逆参照していはいけない。memcpy_from_userやmemcpy_to_user
 * などの安全な関数を通してアクセスすること。
 */
#define __user __attribute__((noderef, address_space(1)))

void memcpy_from_user(void *dst, __user const void *src, size_t len);
void memcpy_to_user(__user void *dst, const void *src, size_t len);
long handle_syscall(int n, long a1, long a2, long a3, long a4, long a5);

#ifdef CONFIG_ABI_EMU
void abi_emu_hook(trap_frame_t *frame, enum abi_hook_type type);
#endif

// kernel/arch/arch64/usercopy.S で実装
/** @ingroup arm64
 * ユーザ空間からメモリをコピーするアーキテクチャ固有の関数.
 * @param dst コピー先アドレス
 * @param src コピー元アドレス
 * @param len コピーサイズ
 */
void arch_memcpy_from_user(void *dst, __user const void *src, size_t len);

/** @ingroup arm64
 * メモリをユーザ空間にコピーするアーキテクチャ固有の関数.
 * @param dst コピー先アドレス
 * @param src コピー元アドレス
 * @param len コピーサイズ
 */
void arch_memcpy_to_user(__user void *dst, const void *src, size_t len);

#endif
