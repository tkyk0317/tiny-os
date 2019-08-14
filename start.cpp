#include "asm.h"
#include "fork.hpp"
#include "scheduler.hpp"
#include "memory.hpp"
#include "devices/uart/uart.hpp"
#include "devices/system_timer/system_timer.hpp"
#include "devices/register/regs.hpp"
#include "devices/mmio/mmio.hpp"

extern "C" void* _Unwind_Resume() { return 0; }
extern "C" void* __gxx_personality_v0() { return 0; }

extern void* user_task1(void*);
extern void* user_task2(void*);
extern void* user_task3(void*);

/**
 * Initialize Function
 */
void init()
{
    // メモリーユニット初期化
    MemoryManager::init();

    // スケジューラー初期化
    Scheduler::init();

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

    // スレッドスタート
    if (false == Process::fork(user_task1, 0)) {
        UART::send("Error user_task1 fork \n");
    }

    // exec shell
    UART::send("main task start\n");
    while(1) {
        UART::send("main task \n");
        Scheduler::schedule();
    }
}

