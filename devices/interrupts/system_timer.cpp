#include <stdint.h>
#include "devices/interrupts/system_timer.hpp"
#include "devices/system_timer/system_timer.hpp"
#include "devices/uart/uart.hpp"
#include "thread.hpp"

/**
 * スタックポインタ取得
 */
uint64_t* get_sp()
{
    uint64_t sp;
    asm volatile ("mov %0, sp" : "=r" (sp));
    return reinterpret_cast<uint64_t*>(sp);
}

/**
 * システムタイマー割り込みハンドラ
 */
void SystemTimerInterrupt::handler()
{
    // タイマー周期を再設定
    asm volatile ("msr cntv_tval_el0, %0" :: "r" (SystemTimer::getPeriod()));
    UART::send("Timer \n");
}
