/** @file kdebug.h */
#ifndef __KDEBUG_H__
#define __KDEBUG_H__

#include <types.h>

/** @ingroup kernel
 * @def STACK_CANARY_VALUE
 * @brief STACK_CANARY値
*/
#define STACK_CANARY_VALUE 0xdeadca71

__mustuse error_t kdebug_run(const char *cmdline, char *buf, size_t len);
void stack_check(void);
void stack_set_canary(void);

// アーキテクチャで実装
int kdebug_readchar(void);
bool kdebug_is_readable(void);
void arch_semihosting_halt(void);

#endif
