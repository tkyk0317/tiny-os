#include "thread.hpp"
#include "devices/uart/uart.hpp"
#include "devices/system_timer/system_timer.hpp"
#include "devices/register/regs.hpp"
#include "devices/mmio/mmio.hpp"

extern "C" void enable_irq();
extern "C" void* _Unwind_Resume() { return 0; }
extern "C" void* __gxx_personality_v0() { return 0; }

void* dummy_task1(void* args)
{
    (void) args;
    while(1) {
        UART::send("dummy task1 \n");
    }
    return 0;
}

void* dummy_task2(void* args)
{
    (void) args;
    while(1) {
        UART::send("dummy task2 \n");
    }
    return 0;
}

/**
 * Initialize Function
 */
void init()
{
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
    start_thread(dummy_task1, 0);
    start_thread(dummy_task2, 0);

    // exec shell
    while(1) {
        UART::send("main thread \n");
        do_switch();
    }
}

