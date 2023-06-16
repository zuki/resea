#include "asm.h"
#include <machine/machine.h>
#include <task.h>

/** @ingroup arm64
 * @var cpuvars
 * @brief CPUローカル情報を保持する変数
 */
static struct cpuvar cpuvars[NUM_CPUS_MAX];

/** @ingroup arm64
 * @brief カレントタスクのCPUローカル情報を取得する.
 * @return カレントタスクのCPUローカル情報へのポインタ
 */
struct cpuvar *arm64_get_cpuvar(void) {
    return &cpuvars[mp_self()];
}

/** @ingroup arm64
 * @brief 停止する.
 */
void halt(void) {
    while (true) {
        __asm__ __volatile__("wfi");
    }
}

/** @ingroup arm64
 * @brief CPU1-3を起動する.
 */
void mp_start(void) {
    machine_mp_start();
}

/** @ingroup arm64
 * @brief CPU1-3のスケジューラ（未実装）.
 */
void mp_reschedule(void) {
    // TODO:
}

/** @ingroup arm64
 * @def LOCKED
 * @brief ロック値.
 */
#define LOCKED        0x12ab
/** @ingroup arm64
 * @def UNLOCKED
 * @brief アンロック値.
 */
#define UNLOCKED      0xc0be
/** @ingroup arm64
 * @def NO_LOCK_OWNER
 * @brief ロック所有者なし.
 */
#define NO_LOCK_OWNER -1
/** @ingroup arm64
 * @var big_lock
 * @brief ロック変数.
 */
static int big_lock = UNLOCKED;
/** @ingroup arm64
 * @var lock_owner
 * @brief ロック所有者.
 */
static int lock_owner = NO_LOCK_OWNER;

/** @ingroup arm64
 * @brief ロックを取得する.
 */
void lock(void) {
    // 1. すでにロックの所有者となっている（二重ロック）
    return;     // FIXME;

    if (mp_self() == lock_owner) {
        PANIC("recursive lock (#%d)", mp_self());
    }
    // 2. ロック処理（atomicにロック変数値を変更する）
    while (!__sync_bool_compare_and_swap(&big_lock, UNLOCKED, LOCKED)) {
        //  __asm__ __volatile__("wfe");
    }
    // 3. ロック所有者をセットする
    lock_owner = mp_self();
}

/** @ingroup arm64
 * @brief パニックロックを取得する.
 */
void panic_lock(void) {
    lock_owner = mp_self();
}

/** @ingroup arm64
 * @brief ロックを解放する.
 */
void unlock(void) {
    return;  // FIXME:

    DEBUG_ASSERT(lock_owner == mp_self());
    lock_owner = NO_LOCK_OWNER;
    __sync_bool_compare_and_swap(&big_lock, LOCKED, UNLOCKED);
}

/** @ingroup arm64
 * @brief パニックロックを解放する.
 */
void panic_unlock(void) {
    if (mp_self() == lock_owner) {
        lock_owner = NO_LOCK_OWNER;
    }
}
