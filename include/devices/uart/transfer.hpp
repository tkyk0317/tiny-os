/**
 * シリアル転送クラス
 */
#pragma once

#include <cstdint>

/**
 * シリアル転送クラス
 *
 * 使用するプロトコルは、XMODEM
 */
class UARTTransfer {
public:
    // 転送データ受信処理
    static bool load();
    // バッファダンプ
    static void dump();
    // 受信データ取得
    static const uint8_t* getData() { return buf; }

private:
    // ready処理
    static void ready();
    // NAK送信
    static void nak();
    // ACK送信
    static void ack();
    // パケットデータ受信
    static bool receive();

    // 転送データバッファ
    static const uint64_t BUFF_SIZE = 1024 * 1024; // 1MB
    static uint64_t index;
    static uint8_t buf[BUFF_SIZE];

    // XMODEMプロトコル定義
    static const char XMODEM_SOH = 0x01;
    static const char XMODEM_STX = 0x02;
    static const char XMODEM_EOT = 0x04;
    static const char XMODEM_ACK = 0x06;
    static const char XMODEM_NAK = 0x15;
    static const char XMODEM_CAN = 0x18;
    static const char XMODEM_EOF = 0x1a;
    static const uint64_t XMODEM_BLOCK_SIZE = 128;

    UARTTransfer() = delete;
    ~UARTTransfer() = delete;
    UARTTransfer(const UARTTransfer&) = delete;
    UARTTransfer(const UARTTransfer&&) = delete;
    UARTTransfer& operator=(const UARTTransfer&) = delete;
    UARTTransfer& operator=(const UARTTransfer&&) = delete;
};
