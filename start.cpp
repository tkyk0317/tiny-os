#include "uart/uart.hpp"

void init();
void shell();

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
    auto msg = "start tiny os\n";
    UART::send(msg);

    // exec shell
    shell();
}

/**
 * Initialize Function
 */
void init()
{
    UART::init();
}

/**
 * start shell
 */
void shell()
{
    while(true) {
        uint32_t c = UART::receive();
        if (c == 0xD) c = '\n';
        UART::sendChar(static_cast<char>(c));
    }
}

// gcc定義関数
extern "C" void* _Unwind_Resume() { return 0; }
extern "C" void* __gxx_personality_v0() { return 0; }
