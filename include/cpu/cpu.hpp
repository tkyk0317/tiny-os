#pragma once

#include <stdlib.h>

/**
 * CPUに対する処理を行うクラス
 */
class CPU {
public:
    // 指定されたサイクル数、delayを行う
    static inline void delay(int32_t count)
    {
        asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
                : "=r"(count): [count]"0"(count) : "cc");
    }
private:
    CPU();
    ~CPU();
};
