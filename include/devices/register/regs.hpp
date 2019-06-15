#pragma once

#include <stdlib.h>

// GPU関連レジスタ
enum class GPU : uint32_t
{
    INTERRUPTS_ROUTING = 0x4000000C,
};

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

// CORE0関連レジスタ
enum class CORE0 : uint32_t
{
    INTERRUPT_SOURCE = 0x40000060,
};

// IRQ関連レジスタ
enum class IRQ : uint32_t
{
    BASE          = 0x3F00B200,
    BASIC         = IRQ::BASE + 0x00,
    PEND1         = IRQ::BASE + 0x04,
    PEND2         = IRQ::BASE + 0x08,
    FIQ           = IRQ::BASE + 0x0C,
    ENABLE1       = IRQ::BASE + 0x10,
    ENABLE2       = IRQ::BASE + 0x14,
    ENABLE_BASIC  = IRQ::BASE + 0x18,
    DISABLE1      = IRQ::BASE + 0x1C,
    DISABLE2      = IRQ::BASE + 0x20,
    DISABLE_BASIC = IRQ::BASE + 0x24,
};

// CORE0 IRQ
enum class CORE0_IRQ
{
    GPU_INTERRUPT = 1 << 8, // GPU割り込み
};

// IRQ Basic
enum class IRQ_BASIC
{
    PENDING2 = 1 << 9, // PENDING2割り込み
};

// IRQ Enable2
enum class IRQ_ENABLE2
{
    UART = 1 << 25, // UART割り込み
};

// IRQ Pending2
enum class IRQ_PENDING2
{
    UART = 1 << 25, // UART割り込み
};

// UART MIS
enum class UART_MIS
{
    RXMIS = 1 << 4, // RXマスク
};
