/**
 * シリアルデータ転送クラス実装ファイル
 */
#include "devices/uart/transfer.hpp"
#include "devices/uart/uart.hpp"

// 受信データ格納バッファ
uint64_t UARTTransfer::index = 0;
uint8_t UARTTransfer::buf[UARTTransfer::BUFF_SIZE] = {0};

/**
 * 受信バッファダンプ
 */
void UARTTransfer::dump()
{
    UART::send("\nTransfer Receive Buffer: ");
    UART::sendHex(UARTTransfer::index);
    UART::send("\n");
    for (uint64_t i = 0; i < BUFF_SIZE; i++) {
        if (i % 4 == 0) UART::send("\n");
        UART::sendHex(UARTTransfer::buf[i]);
        UART::send(" ");
    }
}

/**
 * 転送データ受信処理
 */
bool UARTTransfer::load()
{
    // NAK送信し、受信準備
    UARTTransfer::ready();

    // データ受信中
    while(true) {
        uint32_t c = UART::receive();
        switch (c) {
        case XMODEM_SOH:
            // パケットデータを解析し、問題なければACK
            if (UARTTransfer::receive()) {
                UARTTransfer::ack(); // 応答
            }
            else {
                UARTTransfer::nak(); // 再送
            }
            break;
        case XMODEM_EOT:
            // 送信終了
            UARTTransfer::ack();
            return true;
        case XMODEM_CAN:
        default:
            return false;
        }
    }
    return true;
}

/**
 * ready処理
 *
 * NAKパケットを送信し、受信準備を行う
 */
void UARTTransfer::ready()
{
    // バッファ初期化
    UARTTransfer::index = 0;
    for (auto&& b: UARTTransfer::buf) b = 0x0;

    // 受信バッファにデータが来るまでNAK送信
    uint64_t cnt = 0;
    volatile bool is_receive = UART::isReceive();
    while(false == is_receive) {
        // 一定間隔毎にNAK送信
        if (++cnt >= 200000) {
            cnt = 0;
            UARTTransfer::nak();
        }
        is_receive = UART::isReceive();
    }
}

/**
 * NAKパケット送信
 */
void UARTTransfer::nak()
{
    UART::sendChar(XMODEM_NAK);
}

/**
 * ACKパケット送信
 */
void UARTTransfer::ack()
{
    UART::sendChar(XMODEM_ACK);
}

/**
 * パケットデータ受信
 *
 * @return true: 受信成功 false: 受信失敗
 */
bool UARTTransfer::receive()
{
    // ブロック番号
    uint8_t blk = UART::receive();
    uint8_t com_blk = UART::receive(); // ブロック番号を反転したもの

    // ブロック番号不一致
    if (0xFF != (blk ^ com_blk)) return false;

    // データ取得
    uint64_t checksum = 0;
    for (uint64_t i = 0; i < XMODEM_BLOCK_SIZE; i++) {
        uint8_t data = UART::receive();
        UARTTransfer::buf[UARTTransfer::index++] = data;
        checksum += data;
    }

    // チェックサム(加算データ下位8bitと比較)
    uint8_t recv_checksum = UART::receive();
    if ((checksum & 0xFF) != recv_checksum) return false;

    return true;
}
