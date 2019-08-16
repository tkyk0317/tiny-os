#include "shell.hpp"
#include "devices/mmio/mmio.hpp"
#include "devices/register/regs.hpp"
#include "devices/uart/uart.hpp"
#include "devices/interrupts/uart.hpp"

/**
 * UART割り込みハンドラ
 */
void UARTInterrupt::handler()
{
    // 各割り込みに応じた関数をコール
    UARTInterrupt uart;
    if (MMIO::read(UART0_MIS) & UART_MIS_RXMIS) uart.receive();
}

/**
 * 受信割り込み
 */
void UARTInterrupt::receive() const
{
    // 受信したデータをシェルへ
    uint32_t c = UART::receive();
    Shell::receive(c);
}
