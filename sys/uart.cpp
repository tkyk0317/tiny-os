#include "devices/uart/uart.hpp"
#include "sys/uart.hpp"

/**
 * UART書き込みシステムコール
 */
void UARTSysCall::__write(uint64_t str)
{
    UART::send(reinterpret_cast<const char* const>(str));
}

/**
 * UART書き込みシステムコール
 */
void UARTSysCall::write_sys_call(const char* const str)
{
    asm volatile(
    "mov x0, %0; mov x8, #1; svc #0; ret;"
     :: "r"(reinterpret_cast<uint64_t>(str))
    );
}

