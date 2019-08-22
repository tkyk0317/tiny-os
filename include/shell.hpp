#pragma once

#include <cstdint>

/**
 * カーネルシェルクラス
 */
class Shell {
public:
    // カーネルシェル起動
    static void start();
    // コマンド受信
    static void receive(uint32_t);

private:
    // コマンド受信バッファ
    static const uint64_t BUFF_SIZE = 128;
    static char buf[BUFF_SIZE];
    static uint64_t index;
    static bool is_boot;

    // ダミータスク開始コマンド
    static void command_dummy();
    // helpコマンド
    static void command_help();
    // xmodemコマンド
    static void command_xmodem();
    // loadコマンド
    static void command_load();
    // dumpコマンド
    static void command_dump();

    Shell() = delete;
    ~Shell() = delete;
    Shell(const Shell&) = delete;
    Shell(const Shell&&) = delete;
    Shell& operator=(const Shell&) = delete;
    Shell& operator=(const Shell&&) = delete;
};
