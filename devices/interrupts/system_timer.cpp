#include "devices/interrupts/system_timer.hpp"
#include "devices/system_timer/system_timer.hpp"
#include "asm.h"
#include "scheduler.hpp"

/**
 * システムタイマー割り込みハンドラ
 */
void SystemTimerInterrupt::handler()
{
    // タイマー周期を再設定
    asm volatile ("msr cntv_tval_el0, %0" :: "r" (SystemTimer::getPeriod()));

    // スケジューラーへタイマー通知
    Scheduler::timer();
}
