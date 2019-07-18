#pragma once

#include <stdint.h>

/**
 * UARTシステムコールクラス
 */
class UARTSysCall {
public:
    // writeシステムコール
    static void write_sys_call(const char* const);

    // カーネルからコールされるメソッド
    static void __write(uint64_t);

    UARTSysCall() = delete;
    ~UARTSysCall() = delete;
    UARTSysCall(const UARTSysCall&) = delete;
    UARTSysCall(const UARTSysCall&&) = delete;
    UARTSysCall& operator=(const UARTSysCall&) = delete;
    UARTSysCall& operator=(const UARTSysCall&&) = delete;
};
