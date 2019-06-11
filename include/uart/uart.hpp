#include <stdint.h>

/**
 * UARTクラス
 */
class UART {
public:
    // コンストラクタ
    UART();
    // デストラクタ
    ~UART();

    // 文字列送信
    void send(const char* const);

private:
    // 各種レジスタ定義
    enum
    {
        // The base address for UART.
        UART0_BASE = 0x3F201000,

        // The offsets for reach register for the UART.
        UART0_DR     = (UART0_BASE + 0x00),
        UART0_FR     = (UART0_BASE + 0x18),
    };

    // 文字送信
    void sendChar(char);
};
