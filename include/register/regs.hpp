#pragma once

#include <stdlib.h>

// GPIOレジスタ定義
enum class GPIO : uint32_t
{
    // GPIOレジスタ
    BASE   = 0x3F200000,
    UD     = (GPIO::BASE + 0x94),
    UDCLK0 = (GPIO::BASE + 0x98),
};

// UART0レジスタ
enum class UART0 : uint32_t
{
    BASE   = 0x3F201000,
    DR     = (UART0::BASE + 0x00),
    RSRECR = (UART0::BASE + 0x04),
    FR     = (UART0::BASE + 0x18),
    ILPR   = (UART0::BASE + 0x20),
    IBRD   = (UART0::BASE + 0x24),
    FBRD   = (UART0::BASE + 0x28),
    LCRH   = (UART0::BASE + 0x2C),
    CR     = (UART0::BASE + 0x30),
    IFLS   = (UART0::BASE + 0x34),
    IMSC   = (UART0::BASE + 0x38),
    RIS    = (UART0::BASE + 0x3C),
    MIS    = (UART0::BASE + 0x40),
    ICR    = (UART0::BASE + 0x44),
    DMACR  = (UART0::BASE + 0x48),
    ITCR   = (UART0::BASE + 0x80),
    ITIP   = (UART0::BASE + 0x84),
    ITOP   = (UART0::BASE + 0x88),
    TDR    = (UART0::BASE + 0x8C),
};
