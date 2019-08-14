#pragma once

#include <cstdint>

/**
 * UARTシステムコールクラス
 */
class UARTSysCall {
public:
    // カーネルからコールされるメソッド
    static void __write(uint64_t);

    UARTSysCall() = delete;
    ~UARTSysCall() = delete;
    UARTSysCall(const UARTSysCall&) = delete;
    UARTSysCall(const UARTSysCall&&) = delete;
    UARTSysCall& operator=(const UARTSysCall&) = delete;
    UARTSysCall& operator=(const UARTSysCall&&) = delete;
};
