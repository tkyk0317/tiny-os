#include <cstdint>
#include "devices/system_timer/system_timer.hpp"
#include "devices/register/regs.hpp"
#include "devices/mmio/mmio.hpp"
#include "devices/uart/uart.hpp"

// クロック周波数
uint32_t SystemTimer::freq = 0;

/**
 * 初期化処理
 */
void SystemTimer::init()
{
    // Clockを読み取り、タイマー周期を設定
    asm volatile ("mrs %0, cntfrq_el0" : "=r" (SystemTimer::freq));
    asm volatile ("msr cntv_tval_el0, %0" :: "r" (SystemTimer::getPeriod()));

    // Core0Timer割り込み有効化
    MMIO::write(CORE0_TIMER_INTERRUPT_CTL, CORE0_TIMER_IRQ_ENABLE);

    // enable cntv
    asm volatile ("msr cntv_ctl_el0, %0" :: "r" (1));
}
