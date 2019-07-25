#include "fork.hpp"
#include "sys/fork.hpp"

/**
 * プロセスforkシステムコール
 */
void ForkSysCall::__fork(uint64_t fn, uint64_t arg)
{
    Process::fork(reinterpret_cast<TASK_ENTRY>(fn), reinterpret_cast<void*>(arg));
}

