#include "uart/uart.hpp"

/**
 * エントリーポイント
 */
extern "C" void __start_kernel(uint32_t r0, uint32_t r1, uint32_t atags) {
    // declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    UART uart;
    uart.send("start tiny os\n");
    while(1);
}

// gcc定義関数
extern "C" void* _Unwind_Resume() { return 0; }
extern "C" void* __gxx_personality_v0() { return 0; }
