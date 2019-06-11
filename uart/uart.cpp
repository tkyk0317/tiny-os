#include "mmio/mmio.hpp"
#include "uart/uart.hpp"

/**
 * コンストラクタ
 */
UART::UART()
{}

/**
 * デストラクタ
 */
UART::~UART()
{}

/**
 * 文字列送信
 */
void UART::sendChar(const char c) const
{
    while (MMIO::read(UART::UART0_FR) & (1 << 5));
    MMIO::write(UART::UART0_DR, c);
}

/**
 * 文字列送信
 */
void UART::send(const char* const str) const
{
    uint32_t i = 0;
    while(str[i] != 0) sendChar(static_cast<char>(str[i++]));
}
