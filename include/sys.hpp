#pragma once

#include <stdint.h>
#include "fork.hpp"

/**
 * システムコールクラス
 */
class SysCall {
public:
    // プロセスfork
    static int64_t fork(TASK_ENTRY, void*);

    SysCall() = delete;
    ~SysCall() = delete;
    SysCall(const SysCall&) = delete;
    SysCall(const SysCall&&) = delete;
    SysCall& operator=(const SysCall&) = delete;
    SysCall& operator=(const SysCall&&) = delete;
};
