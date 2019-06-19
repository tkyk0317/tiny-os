#include "thread.hpp"
#include "devices/uart/uart.hpp"
#include "devices/register/regs.hpp"
#include "devices/mmio/mmio.hpp"

extern "C" void enable_irq();
extern "C" void* _Unwind_Resume() { return 0; }
extern "C" void* __gxx_personality_v0() { return 0; }

/**
 * スタックポインタ取得
 */
uint64_t* get_sp()
{
    uint64_t sp;
    asm volatile ("mov %0, sp" : "=r" (sp));
    return reinterpret_cast<uint64_t*>(sp);
}

void* dummy_task1(void* args)
{
    while(1) {
        //UART::send("dummy task1 \n");
        asm volatile("wfi");
        do_switch();
    }
    return 0;
}

void* dummy_task2(void* args)
{
    while(1) {
        //UART::send("dummy task2 \n");
        asm volatile("wfi");
        do_switch();
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
    start_thread(dummy_task1);
    start_thread(dummy_task2);

    // exec shell
    while(1) {
        //UART::send("main thread \n");
        asm volatile("wfi");
        do_switch();
    }
}

