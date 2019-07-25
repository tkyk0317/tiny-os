#include <stdlib.h>
#include "asm.h"
#include "devices/uart/uart.hpp"
#include "devices/mmio/mmio.hpp"
#include "devices/register/regs.hpp"
#include "devices/interrupts/uart.hpp"
#include "devices/interrupts/system_timer.hpp"

/**
 * IRQ割り込みハンドラ
 *
 * vector.sからコールされる、割り込みハンドラ
 */
extern "C" void __irq_handler()
{
    // GPU割り込みチェック
    if (MMIO::read(CORE0_INTERRUPT_SOURCE) & CORE0_IRQ_GPU_INTERRUPT) {
        // UART0の受信割り込みチェック
        if (MMIO::read(IRQ_BASIC) & IRQ_BASIC_PENDING2) {
            if (MMIO::read(IRQ_PEND2) & IRQ_PENDING2_UART) {
                // UART割り込みを無効化し、処理。その後、有効化
                MMIO::write(IRQ_DISABLE2, IRQ_DISABLE2_UART);
                UARTInterrupt::handler();
                MMIO::write(IRQ_ENABLE2, IRQ_ENABLE2_UART);
            }
        }
    }
    // Core0 Timer割り込みチェック
    if (MMIO::read(CORE0_INTERRUPT_SOURCE) & CORE0_IRQ_TIMER_INTERRUPT) {
        SystemTimerInterrupt::handler();
    }
}

