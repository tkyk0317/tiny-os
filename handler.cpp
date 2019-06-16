#include <stdlib.h>
#include "devices/uart/uart.hpp"
#include "devices/mmio/mmio.hpp"
#include "devices/register/regs.hpp"
#include "devices/interrupts/uart.hpp"

extern "C" void enable_irq();
extern "C" void disable_irq();

/**
 * IRQ割り込みハンドラ
 *
 * vector.sからコールされる、割り込みハンドラ
 */
extern "C" void __irq_handler()
{
    // UART0の受信割り込みチェック
    if (MMIO::read(CORE0_INTERRUPT_SOURCE) & CORE0_IRQ_GPU_INTERRUPT) {
        if (MMIO::read(IRQ_BASIC) & IRQ_BASIC_PENDING2) {
            if (MMIO::read(IRQ_PEND2) & IRQ_PENDING2_UART) {
                disable_irq();
                UART_Interrupt::handler();
                enable_irq();
            }
        }
   }
    return;
}

