#include <stddef.h>
#include "devices/mmio/mmio.hpp"
#include "devices/uart/uart.hpp"
#include "devices/register/regs.hpp"
#include "cpu/cpu.hpp"

/**
 * 初期化処理
 *
 * UART0の初期化を行う
 *
 * p.175: https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
 * https://wiki.osdev.org/ARM_RaspberryPi_Tutorial_C
 */
void UART::init()
{
    // UART0無効化
    MMIO::write(UART0_CR, 0x00000000);

    // GPIO pull/upの無効にし、150サイクル待つ
    MMIO::write(GPIO_UD, 0x00000000);
    CPU::delay(150);

    // 14/15pinを無効にし、150サイクル待つ
    MMIO::write(GPIO_UDCLK0, (1 << 14) | (1 << 15));
    CPU::delay(150);

    // GPPUDCLK0を0に設定
    MMIO::write(GPIO_UDCLK0, 0x00000000);

    // Interrupt Clear Register割り込みの無効化
    MMIO::write(UART0_ICR, 0x7FF);

    // ボーレート(シリアル通信に使う)の整数部・小数部の計算
    //
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.
    //
    // Divider = 3000000/(16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    MMIO::write(UART0_IBRD, 1);
    MMIO::write(UART0_FBRD, 40);

    // FIFO，8bit通信を有効にする．
    // 4bit目: FIFOを有効に
    // 5,6bit目: 1フレームの通信量 11なら8 bit/フレーム
    MMIO::write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    // 全ての割り込みを有効に
    // 1:有効 0:無効
    // 1bit目: uUARTCTS modern 割り込み
    // 4bit目: 受信割り込み
    // 5bit目: 送信割り込み
    // 6bit目: 受信タイムアウト割り込み
    // 7bit目: フレームエラー割り込み
    // 8bit目: パリティエラー割り込み
    // 9bit目: Breakエラー割り込み
    // 10bit目: オーバーランエラー割り込み
    MMIO::write(UART0_IMSC, UART_MIS_RXMIS); // 受信割り込みのみ
        //(1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10)

    // 0bit目: UARTを有効に
    // 8bit目: 送信を有効に
    // 9bit目: 受信を有効に
    MMIO::write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

/**
 * 文字列送信
 */
void UART::sendChar(const char c)
{
    while (MMIO::read(UART0_FR) & (1 << 5));
    MMIO::write(UART0_DR, c);
}

/**
 * 文字列送信
 */
void UART::send(const char* const str)
{
    uint32_t i = 0;
    while(str[i] != 0) sendChar(static_cast<char>(str[i++]));
}

/**
 * 受信処理
 */
uint32_t UART::receive()
{
    // データ受信待ち
    while (MMIO::read(UART0_FR) & (1 << 4));
    return MMIO::read(UART0_DR);
}

/**
 * 受信チェック
 *
 * @return true: 受信可能 false: 受信不可能
 */
bool UART::isReceive()
{
    return 0 == (MMIO::read(UART0_FR) & (1 << 4));
}

/**
 * HEX出力
 */
void UART::sendHex(uint64_t n)
{
    const char *hexdigits = "0123456789ABCDEF";
    for (int i = 60; i >= 0; i -= 4) sendChar(hexdigits[(n >> i) & 0xf]);
}

/**
 * 割り込み無効化
 */
void UART::disableInt()
{
    MMIO::write(UART0_CR, 0x00000000);
    MMIO::write(UART0_IMSC, 0); // 割り込み無効
    MMIO::write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

/**
 * 割り込み有効化
 */
void UART::enableInt()
{
    MMIO::write(UART0_CR, 0x00000000);
    MMIO::write(UART0_IMSC, UART_MIS_RXMIS); // 受信割り込みのみ
    MMIO::write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

