#include "devices/uart/uart.hpp"
#include "sys/uart.hpp"

/**
 * UART書き込みシステムコール
 */
void UARTSysCall::__write(uint64_t str)
{
    UART::send(reinterpret_cast<const char* const>(str));
}

