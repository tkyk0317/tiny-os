#include "devices/mmio/mmio.hpp"
#include "devices/register/regs.hpp"
#include "devices/uart/uart.hpp"
#include "devices/interrupts/uart.hpp"

/**
 * UART割り込みハンドラ
 */
void UART_Interrupt::handler()
{
    // 各割り込みに応じた関数をコール
    UART_Interrupt uart;
    if (MMIO::read(static_cast<uint32_t>(UART0::MIS)) & static_cast<uint32_t>(UART_MIS::RXMIS)) uart.receive();
}

/**
 * 受信割り込み
 */
void UART_Interrupt::receive() const
{
    uint32_t c = UART::receive();
    if (c == 0xD) c = '\n';
    UART::sendChar(static_cast<char>(c));
}
