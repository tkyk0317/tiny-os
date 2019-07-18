#include "sys/fork.hpp"
#include "sys/uart.hpp"

// システムコールテーブル
extern "C" void* const sys_call_tbl[] = {
    reinterpret_cast<void*>(ForkSysCall::__fork),
    reinterpret_cast<void*>(UARTSysCall::__write)
};

