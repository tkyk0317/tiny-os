#pragma once

#include <stdlib.h>

// GPU割り込み関連レジスタ
const uint32_t GPU_INTERRUPTS_ROUTING = 0x4000000C;

// GPIOレジスタ定義
const uint32_t GPIO_BASE   = 0x3F200000;
const uint32_t GPIO_UD     = (GPIO_BASE + 0x94);
const uint32_t GPIO_UDCLK0 = (GPIO_BASE + 0x98);

// UART0レジスタ
const uint32_t UART0_BASE   = 0x3F201000;
const uint32_t UART0_DR     = (UART0_BASE + 0x00);
const uint32_t UART0_RSRECR = (UART0_BASE + 0x04);
const uint32_t UART0_FR     = (UART0_BASE + 0x18);
const uint32_t UART0_ILPR   = (UART0_BASE + 0x20);
const uint32_t UART0_IBRD   = (UART0_BASE + 0x24);
const uint32_t UART0_FBRD   = (UART0_BASE + 0x28);
const uint32_t UART0_LCRH   = (UART0_BASE + 0x2C);
const uint32_t UART0_CR     = (UART0_BASE + 0x30);
const uint32_t UART0_IFLS   = (UART0_BASE + 0x34);
const uint32_t UART0_IMSC   = (UART0_BASE + 0x38);
const uint32_t UART0_RIS    = (UART0_BASE + 0x3C);
const uint32_t UART0_MIS    = (UART0_BASE + 0x40);
const uint32_t UART0_ICR    = (UART0_BASE + 0x44);
const uint32_t UART0_DMACR  = (UART0_BASE + 0x48);
const uint32_t UART0_ITCR   = (UART0_BASE + 0x80);
const uint32_t UART0_ITIP   = (UART0_BASE + 0x84);
const uint32_t UART0_ITOP   = (UART0_BASE + 0x88);
const uint32_t UART0_TDR    = (UART0_BASE + 0x8C);

// CORE0関連レジスタ
const uint32_t CORE0_INTERRUPT_SOURCE = 0x40000060;

// IRQ関連レジスタ
const uint32_t IRQ_BASE          = 0x3F00B200;
const uint32_t IRQ_BASIC         = IRQ_BASE + 0x00;
const uint32_t IRQ_PEND1         = IRQ_BASE + 0x04;
const uint32_t IRQ_PEND2         = IRQ_BASE + 0x08;
const uint32_t IRQ_FIQ           = IRQ_BASE + 0x0C;
const uint32_t IRQ_ENABLE1       = IRQ_BASE + 0x10;
const uint32_t IRQ_ENABLE2       = IRQ_BASE + 0x14;
const uint32_t IRQ_ENABLE_BASIC  = IRQ_BASE + 0x18;
const uint32_t IRQ_DISABLE1      = IRQ_BASE + 0x1C;
const uint32_t IRQ_DISABLE2      = IRQ_BASE + 0x20;
const uint32_t IRQ_DISABLE_BASIC = IRQ_BASE + 0x24;

// CORE0 IRQ
const uint32_t CORE0_IRQ_GPU_INTERRUPT = 1 << 8; // GPU割り込み

// IRQ Basic
const uint32_t IRQ_BASIC_PENDING2 = 1 << 9; // PENDING2割り込み

// IRQ Enable2
const uint32_t IRQ_ENABLE2_UART = 1 << 25; // UART割り込み

// IRQ Disable2
const uint32_t IRQ_DISABLE2_UART = 1 << 25; // UART割り込み

// IRQ Pending2
const uint32_t IRQ_PENDING2_UART = 1 << 25; // UART割り込み

// UART MIS
const uint32_t UART_MIS_RXMIS = 1 << 4; // RXマスク
