#pragma once

#include <cstdint>

/**
 * プロセス起動システムコールクラス
 */
class ForkSysCall {
public:
    // カーネルからコールされるメソッド
    static void __fork(uint64_t, uint64_t);

    ForkSysCall() = delete;
    ~ForkSysCall() = delete;
    ForkSysCall(const ForkSysCall&) = delete;
    ForkSysCall(const ForkSysCall&&) = delete;
    ForkSysCall& operator=(const ForkSysCall&) = delete;
    ForkSysCall& operator=(const ForkSysCall&&) = delete;
};
