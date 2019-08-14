/**
 * プロセス起動関連ヘッダファイル
 */
#pragma once

#include <cstdint>

// エントリー関数ポインタ
using TASK_ENTRY = void*(*)(void*);

/**
 * プロセス起動関連クラス
 */
class Process {
public:
    // プロセス起動
    static bool fork(TASK_ENTRY, void*);

private:
    // エントリーポイント
    static void entry(TASK_ENTRY, void*);

    Process() = delete;
    ~Process() = delete;
    Process(const Process&) = delete;
    Process(const Process&&) = delete;
    Process& operator=(const Process&) = delete;
    Process& operator=(const Process&&) = delete;
};
