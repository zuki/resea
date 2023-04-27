#include "asm.h"
#include <machine/peripherals.h>
#include <printk.h>
#include <types.h>

/** @ingroup arm64
 * @brief 指定されたクロック数だけ遅延させる.
 * @param clocks 遅延させるクロック数
 */
static inline void delay(unsigned clocks) {
    while (clocks-- > 0) {
        __asm__ __volatile__("nop");
    }
}

/** @ingroup arm64
 * @brief Raspiのmailboxコールを実行する.
 * @param ch チャネル
 * @param mbox メールボックスアドレス
 */
static void mbox_call(uint8_t ch, const uint32_t *mbox) {
    ASSERT(IS_ALIGNED((vaddr_t) mbox, 0x10));

    while (mmio_read(MBOX_STATUS) & MBOX_FULL) {
        delay(1);
    }

    // メッセージを送信する
    unsigned int value =
        (((vaddr_t) mbox) & ~0xf)  // 送信するメールボックスアドレス
        | (ch & 0xf)               // 下位4ビットはチャネル
        ;
    mmio_write(MBOX_WRITE, value);

    // 応答を待つ...
    while (1) {
        while (mmio_read(MBOX_STATUS) & MBOX_EMPTY) {
            delay(1);
        }

        if (value == mmio_read(MBOX_READ)) {
            break;
        }
    }
}

/** @ingroup arm64
 * @brief UARTを初期化する.
 */
void uart_init(void) {
    // クロックを設定する
    uint32_t __aligned(16) m[9];
    m[0] = 9 * 4;  // The size of the meessage.
    m[1] = MBOX_CODE_REQUEST;
    m[2] = MBOX_TAG_SETCLKRATE;  // Set clock rate.
    m[3] = 12;                   // The value length.
    m[4] = 8;                    // The clock ID.
    m[5] = 2;                    // UART clock
    m[6] = 4000000;  // The clock rate = 4MHz. (TODO: is 4MHz too low?)
    m[7] = 0;        // Allow turbo settings.
    m[8] = MBOX_TAG_LAST;
    mbox_call(MBOX_CH_PROP, m);

    // GPIOピン #14 と #15 をALT0 (UART0) に初期化する.
    mmio_write(GPIO_FSEL1,
               (mmio_read(GPIO_FSEL1) & ~((0b111 << 12) | (0b111 << 15)))
                   | (0b100 << 12) | (0b100 << 15));

    // See "GPIO Pull-up/down Clock Registers (GPPUDCLKn)" in "BCM2835 ARM
    // Peripherals" for more details on this initializaiton sequence.
    mmio_write(GPIO_PUD, 0);
    delay(150);
    mmio_write(GPIO_PUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    mmio_write(GPIO_PUDCLK0, 0);

    mmio_write(UART0_CR, 0);            // Disable UART.
    mmio_write(UART0_ICR, 0x7f2);       // Disable interrupts from UART.
    mmio_write(UART0_IBRD, 2);          // baud rate = 115200
    mmio_write(UART0_FBRD, 11);         //
    mmio_write(UART0_LCRH, 0b11 << 5);  // 8 bit, No parity, 1 Stopbit, Disable FIFO
    mmio_write(UART0_IMSC, 0x70);       // RTIM, TXIM, RXIM を有効に
    mmio_write(UART0_CR, 0x301);        // Enable RX, TX, and UART0.
    mmio_write(ENABLE_IRQS_2, 1 << (CONSOLE_IRQ % 32)); // GPU割り込み(uart_int)を有効にする
}

/** @ingroup arm64
 * @brief 読み込むべきデータはあるかチェックする
 * @return あれば true; なければ false
 */
bool kdebug_is_readable(void) {
    return (mmio_read(UART0_FR) & (1 << 4)) == 0;
}

/** @ingroup arm64
 * @brief 1文字読み込む.
 * @return 読み込んだ文字、読み込むべき文字がない場合は -1
 */
int kdebug_readchar(void) {
    if (!kdebug_is_readable()) {
        return -1;
    }
    return mmio_read(UART0_DR);
}

/** @ingroup arm64
 * @brief 1文字書き出す.
 * @param ch 書き出す文字
 */
void arch_printchar(char ch) {
    while (mmio_read(UART0_FR) & (1 << 5))
        ;
    mmio_write(UART0_DR, ch);
}

#ifdef CONFIG_FORCE_REBOOT_BY_WATCHDOG
static void watchdog_init(void) {
    mmio_write(PM_WDOG, PM_PASSWORD | CONFIG_WATCHDOG_TIMEOUT << 16);
    mmio_write(PM_RSTC, PM_PASSWORD | PM_WDOG_FULL_RESET);
}
#endif

/** @ingroup arm64
 * @brief タイマーを初期化する.
 */
void arm64_timer_reload(void) {
    // 1. 周波数(1sのtick数）を取得する
    uint64_t hz = ARM64_MRS(cntfrq_el0);
    ASSERT(hz >= 1000);
    // 2. hz: 1msのtick数
    hz /= 1000;
    // 3. 1msタイマーを設定
    ARM64_MSR(cntv_tval_el0, hz);
}

/** @ingroup arm64
 * @brief コアタイマーを初期化する.
 */
static void timer_init(void) {
    // 1. 1msタイマーを設定
    arm64_timer_reload();
    // 2. タイマーを有効にする
    ARM64_MSR(cntv_ctl_el0, 1ull);
    // 3. coreタイマーのIRQ割り込み (nCNTVIRQ) を有効にする
    mmio_write(TIMER_IRQCNTL(mp_self()), 1 << 3);
}

/** @ingroup arm64
 * @brief 割り込みをを初期化する.
 */
static void irq_init(void) {
    // GPU割り込みをCore0に転送させる
    mmio_write(GPU_INT_ROUTE, 0);
    mmio_write(FIQ_CONTROL, 0);
    mmio_write(DISABLE_IRQS_1, (uint32_t)-1);
    mmio_write(DISABLE_IRQS_2, (uint32_t)-1);
    mmio_write(DISABLE_BASIC_IRQS, (uint32_t)-1);
}

/** @ingroup arm64
 * @brief ペリフェラルを初期化する.
 */
void arm64_peripherals_init(void) {
    irq_init();
    uart_init();
#ifdef CONFIG_FORCE_REBOOT_BY_WATCHDOG
    watchdog_init();
#endif
    timer_init();
}

/** @ingroup arm64
 * @brief irqを有効にする. （未実装）
 * @param irq IRQ番号
 */
void arch_enable_irq(unsigned irq) {
    // TODO:
}

/** @ingroup arm64
 * @brief irqを無効にする. （未実装）
 * @param irq IRQ番号
 */
void arch_disable_irq(unsigned irq) {
    // TODO:
}
