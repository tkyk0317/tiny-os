#include <stdint.h>
#include "devices/interrupts/system_timer.hpp"
#include "devices/system_timer/system_timer.hpp"
#include "scheduler.hpp"

extern "C" void enable_irq();
extern "C" void disable_irq();

/**
 * スタックポインタ取得
 */
uint64_t get_sp()
{
    uint64_t sp;
    asm volatile ("mov %0, sp" : "=r" (sp));
    return sp;
}

/**
 * システムタイマー割り込みハンドラ
 */
void SystemTimerInterrupt::handler()
{
    // タイマー周期を再設定
    asm volatile ("msr cntv_tval_el0, %0" :: "r" (SystemTimer::getPeriod()));

    // 割り込みを有効にし、コンテキストスイッチ
    enable_irq();
    Scheduler::schedule();
    disable_irq();
}
