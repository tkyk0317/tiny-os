#include "fork.hpp"
#include "sys/fork.hpp"

/**
 * プロセスforkシステムコール
 */
void ForkSysCall::__fork(uint64_t fn, uint64_t arg)
{
    Process::fork(reinterpret_cast<TASK_ENTRY>(fn), reinterpret_cast<void*>(arg));
}

/**
 * forkシステムコール
 */
int64_t ForkSysCall::fork_sys_call(ENTRY_POINT fn, void* arg)
{
    asm volatile(
    "mov x0, %0; mov x1, %1; mov x8, #0; svc #0; ret;"
     :: "r"(fn), "r"(arg)
    );
    return 0;
}

