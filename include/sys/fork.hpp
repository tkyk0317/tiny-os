#pragma once

#include <stdint.h>

using ENTRY_POINT = void*(*)(void*);

/**
 * プロセス起動システムコールクラス
 */
class ForkSysCall {
public:
    // システムコール
    static int64_t fork_sys_call(ENTRY_POINT, void*);

    // カーネルからコールされるメソッド
    static void __fork(uint64_t, uint64_t);

    ForkSysCall() = delete;
    ~ForkSysCall() = delete;
    ForkSysCall(const ForkSysCall&) = delete;
    ForkSysCall(const ForkSysCall&&) = delete;
    ForkSysCall& operator=(const ForkSysCall&) = delete;
    ForkSysCall& operator=(const ForkSysCall&&) = delete;
};
