#include <stdlib.h>
#include "uart/uart.hpp"
#include "mmio/mmio.hpp"
#include "register/regs.hpp"
#include "interrupts/uart.hpp"

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
    if (MMIO::read(static_cast<uint32_t>(CORE0::INTERRUPT_SOURCE)) & static_cast<uint32_t>(CORE0_IRQ::GPU_INTERRUPT)) {
        if (MMIO::read(static_cast<uint32_t>(IRQ::BASIC)) & static_cast<uint32_t>(IRQ_BASIC::PENDING2)) {
            if (MMIO::read(static_cast<uint32_t>(IRQ::PEND2)) & static_cast<uint32_t>(IRQ_PENDING2::UART)) {
                disable_irq();
                UART_Interrupt::handler();
                enable_irq();
            }
        }
   }
    return;
}

