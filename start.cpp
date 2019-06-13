#include "uart/uart.hpp"

/**
 * エントリーポイント
 *
 * UARTの初期化処理をコンストラクタで実施している。
 * デバイス関連のシングルトン実装が必要であるが、stdlibなどをリンクしていないので、排他制御処理部分でエラーが発生する。
 * 対応を取る必要がある(https://postd.cc/embedded-cpp/)
 */
extern "C" void __start_kernel(uint32_t r0, uint32_t r1, uint32_t atags) {
    // declare as unused
    (void) r0;
    (void) r1;
    (void) atags;

    UART uart;
    auto msg = "start tiny os\n";
    uart.send(msg);
    while(1);
}

// gcc定義関数
extern "C" void* _Unwind_Resume() { return 0; }
extern "C" void* __gxx_personality_v0() { return 0; }
