#pragma once

#include <stdint.h>

// タスク状態
enum TaskStatus {
    RUNNING, // 実行中
    SUSPEND, // 停止中
};

// タスク管理構造体
typedef struct _TaskContext {
    uint64_t* stack; // スタック開始アドレス
    TaskStatus status; // スレッド起動状態
    _TaskContext* next; // 次のスレッド構造体へのポインタ
} TaskContext;

/**
 * スケジューラークラス
 */
class Scheduler {
public:
    // 初期化処理
    static void init();

    // スケジューラー実行
    static void schedule();

    // タスク登録
    static bool register_task(uint64_t*);

    // カレントタスク情報取得
    static TaskContext* current_task() { return Scheduler::current; }

private:
    // 現在のタスク
    static TaskContext* current;

    // タスクコンテキスト情報
    static const uint32_t TASK_NUMS = 8;
    static TaskContext context[Scheduler::TASK_NUMS];
    static uint32_t created_task_nums;

    // 次タスク
    static uint32_t next();

    Scheduler() = delete;
    ~Scheduler() = delete;
    Scheduler(const Scheduler&) = delete;
    Scheduler(const Scheduler&&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&&) = delete;
};
