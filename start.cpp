#include "asm.h"
#include "scheduler.hpp"
#include "memory.hpp"
#include "shell.hpp"
#include "devices/uart/uart.hpp"
#include "devices/system_timer/system_timer.hpp"
#include "devices/register/regs.hpp"
#include "devices/mmio/mmio.hpp"

extern "C" void* _Unwind_Resume() { return 0; }
extern "C" void* __gxx_personality_v0() { return 0; }

/**
 * Initialize Function
 */
void init()
{
    // スケジューラー初期化
    Scheduler::init();

    // メモリーユニット初期化
    MemoryManager::init(Scheduler::current_task()->l1_ptb);

    // UART割り込み有効
    MMIO::write(IRQ_ENABLE2, IRQ_ENABLE2_UART);

    // Core0につなぐ
    MMIO::write(GPU_INTERRUPTS_ROUTING, 0x0);

    // SystemTimer初期化
    SystemTimer::init();

    // UART初期化
    UART::init();

    // enable irq
    enable_irq();
}

/**
 * EntryPoint
 */
extern "C" void __start_kernel(uint32_t r0, uint32_t r1, uint32_t atags)
{
    // declare as unused
    (void) r0; (void) r1; (void) atags;

    // initialize
    init();

    // send message
    UART::send("******************************************\n");
    UART::send("Start Tiny OS\n");
    UART::send("******************************************\n");

    // カーネルシェル起動
    Shell::start();

    UART::send("******************************************\n");
    UART::send("End Tiny OS\n");
    UART::send("******************************************\n");
}

