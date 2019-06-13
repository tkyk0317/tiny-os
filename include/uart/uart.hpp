#pragma once

#include <stdint.h>


/**
 * UARTクラス
 */
class UART {
public:
    // 文字列送信
    void send(const char* const) const;

    // コンストラクタ、デストラクタ
    UART();
    ~UART();
    UART(const UART&) = delete;
    UART& operator=(const UART&) = delete;
    UART(UART&&) = delete;
    UART& operator=(UART&&) = delete;
private:
    // 文字送信
    void sendChar(const char) const;
};
