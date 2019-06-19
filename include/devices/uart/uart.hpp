#pragma once

#include <stdint.h>

/**
 * UARTクラス
 */
class UART {
public:
    // 初期化処理
    static void init();

    // 文字列送信
    static void send(const char* const);

    // 文字送信
    static void sendChar(const char);

    // 16進表示
    static void sendHex(uint64_t);

    // UARTからの受信
    static uint32_t receive();

    UART() = delete ;
    ~UART() = delete;
    UART(const UART&) = delete;
    UART& operator=(const UART&) = delete;
    UART(UART&&) = delete;
    UART& operator=(UART&&) = delete;
};
