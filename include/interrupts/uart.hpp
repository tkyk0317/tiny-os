/**
 * UART関連割り込みクラス
 */
class UART_Interrupt
{
public:
    // 割り込みハンドラ
    static void handler();

private:
    // 受信割り込み
    void receive() const;
};
