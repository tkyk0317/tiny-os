#include "sys.hpp"
#include "fork.hpp"
#include "devices/uart/uart.hpp"

/**
 * プロセスforkシステムコール
 */
extern "C" void __fork(uint64_t fn, uint64_t arg)
{
    Process::fork(reinterpret_cast<TASK_ENTRY>(fn), reinterpret_cast<void*>(arg));
}

int64_t SysCall::fork(TASK_ENTRY fn, void* arg)
{
    asm volatile(
    "mov x0, %0; mov x1, %1; mov x8, #0; svc #0; ret;"
     :: "r"(fn), "r"(arg)
    );
    return 0;
}

