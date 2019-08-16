/**
 * カーネルシェル
 */
#include "lib/str.h"
#include "shell.hpp"
#include "fork.hpp"
#include "devices/uart/uart.hpp"

// ダミーUserタスク
extern void* user_task1(void*);

// コマンド受信バッファ
char Shell::buf[Shell::BUFF_SIZE] = {0};
uint64_t Shell::index = 0;
bool Shell::is_boot = false;

/**
 * カーネルシェル起動
 */
void Shell::start()
{
    // ブートフラグON
    Shell::is_boot = true;

    // 起動初めにヘルプメッセージを表示
    Shell::command_help();

    // プロンプト
    UART::send(">> ");

    // 実際のコマンド受信はreceiveメソッドで行う
    while(true) {
        asm volatile("wfi");
        if (false == Shell::is_boot) return;
    }
}

/**
 * コマンド受信処理
 *
 * UART受信割り込みが発生した場合、コールされる
 */
void Shell::receive(uint32_t c)
{
    // 起動していない場合、SKIP
    if (false == Shell::is_boot) return;

    // 改行コードを受信したらコマンド実行
    if (c != 0xD) {
        // コマンドをバッファに貯める
        Shell::buf[Shell::index++] = static_cast<char>(c);
        UART::sendChar(static_cast<char>(c));
    }
    else {
        // コマンドに応じた処理
        if (0 == strncmp("help", Shell::buf, 4)) {
            // ヘルプコマンド
            Shell::command_help();
        }
        else if (0 == strncmp("dummy", Shell::buf, 5)){
            // ダミータスク実行コマンド
            Shell::command_dummy();
        }
        else if (0 == strncmp("quit", Shell::buf, 4)) {
            // 終了コマンド
            UART::send("\n");
            Shell::is_boot = false;
            return;
        }

        // バッファクリア
        Shell::index = 0;
        for(auto&& b: Shell::buf) b = 0;

        // プロンプト
        UART::send("\n>> ");
    }
}

/**
 * rコマンド実行
 *
 * ダミータスクを生成する
 */
void Shell::command_dummy()
{
    UART::send("\n-----------------------------\n");
    UART::send("Start Dummy Task\n");
    UART::send("-----------------------------\n");
    if (false == Process::fork(user_task1, 0)) {
        UART::send("Error user_task1 fork \n");
    }
}

/**
 * hコマンド実行
 *
 * ヘルプメッセージを表示する
 */
void Shell::command_help()
{
    UART::send("\n-----------------------------\n");
    UART::send("Usage:\n");
    UART::send("\thelp: command help\n");
    UART::send("\tdummy: start dummy user task\n");
    UART::send("\tquit: end tiny-os\n");
    UART::send("-----------------------------\n");
}
