#include "scheduler.hpp"
#include "fork.hpp"

// アセンブラで定義したスレッド切り替え関数
extern "C" void __switch(uint64_t**, uint64_t**);

// タスク関連データ初期化
TaskContext Scheduler::context[Scheduler::TASK_NUMS];
TaskContext* Scheduler::current;
uint32_t Scheduler::created_task_nums;

/**
 * 初期化処理
 */
void Scheduler::init()
{
    Scheduler::created_task_nums = 0;
    Scheduler::current = &Scheduler::context[0];
}

/**
 * 次のスレッドインデックス取得
 */
uint32_t Scheduler::next()
{
    if (Scheduler::created_task_nums >= Scheduler::TASK_NUMS) return 0;
    return Scheduler::created_task_nums + 1;
}

/**
 * スレッド登録
 */
bool Scheduler::register_task(uint64_t* stack)
{
    uint32_t n = Scheduler::next();
    if (n == 0) return false; // もう作成できない

    // 起動するスレッドの情報を作成
    Scheduler::context[n].stack = stack;
    Scheduler::context[n].next = &Scheduler::context[0]; // ヘッドにつなげておく
    Scheduler::created_task_nums = next();

    // 次のスレッドへのポインタを設定
    Scheduler::context[n - 1].next = &Scheduler::context[n];
    return true;
}

/**
 * スレッド切り替え
 */
void Scheduler::schedule()
{
    if (!Scheduler::current->next) return; // 次のタスクが存在しない
    if (Scheduler::current == Scheduler::current->next) return; // 同じタスクへはスイッチしない

    // 現在のスレッドから次のスレッドへ切り替える
    uint64_t** cur = &Scheduler::current->stack;
    Scheduler::current->status = SUSPEND;
    Scheduler::current = Scheduler::current->next;
    Scheduler::current->status = RUNNING;

    // コンテキストスイッチ
    __switch(cur, &Scheduler::current->stack);
}

