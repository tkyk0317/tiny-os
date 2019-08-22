#include <cstdint>
#include "scheduler.hpp"
#include "devices/uart/uart.hpp"

/**
 * 現在のレジスタ情報やスタック情報を出力
 *
 * CurrentELにアクセスするのでEL0では使用できない
 */
extern "C" void show_registers()
{
    UART::send("\n===============================================\n");
    UART::send("Register Information\n");
    uint64_t reg;
    uint64_t sp_el0;
    asm volatile ("mrs %0, CurrentEL" : "=r" (reg)); UART::send("\tCurrentEL: "); UART::sendHex(reg >> 2); UART::send("\n");
    asm volatile ("mrs %0, spsr_el1" : "=r" (reg));  UART::send("\tSPSR_EL1 : ");  UART::sendHex(reg);     UART::send("\n");
    asm volatile ("mrs %0, elr_el1" : "=r" (reg));   UART::send("\tELR_EL1  : ");  UART::sendHex(reg);     UART::send("\n");
    asm volatile ("mrs %0, sp_el0" : "=r" (sp_el0)); UART::send("\tSP_EL0   : ");  UART::sendHex(sp_el0);  UART::send("\n");

    UART::send("\nStack Dump\n");
    uint64_t* sp = reinterpret_cast<uint64_t*>(sp_el0);
    for(uint64_t* st = Scheduler::current_task()->base_stack; st >= sp; sp++) {
        UART::send("\t");
        UART::sendHex(reinterpret_cast<uint64_t>(sp));
        UART::send(": ");
        UART::sendHex(*sp);
        UART::send("\n");
    }

    Scheduler::show_taskinfo();
    UART::send("===============================================\n");
}
