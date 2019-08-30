#include "scheduler.hpp"
#include "memory.h"
#include "fork.hpp"
#include "asm.h"
#include "devices/uart/uart.hpp"

// タスク関連データ初期化
TaskContext Scheduler::context[Scheduler::TASK_NUMS];
TaskContext* Scheduler::current;
uint32_t Scheduler::created_task_nums;

/**
 * 初期化処理
 */
void Scheduler::init()
{
    for (uint32_t i = 0 ; i < TASK_NUMS ; i++) {
        Scheduler::context[i].next = 0;
        Scheduler::context[i].stack = 0;
        Scheduler::context[i].preempt = 0;
        MemoryManager::create_el0_table(Scheduler::context[i].l1_ptb, Scheduler::context[i].l2_ptb);
    }
    Scheduler::created_task_nums = 0;
    Scheduler::current = &Scheduler::context[0];
    Scheduler::current->base_stack = MemoryManager::get_page();
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
bool Scheduler::register_task(uint64_t* stack, uint64_t* base)
{
    if (0 == Scheduler::next()) return false; // もう作成できない

    // 起動するスレッドの情報を作成
    uint32_t n = Scheduler::next();
    Scheduler::context[n].stack = stack;
    Scheduler::context[n].base_stack = base;
    Scheduler::context[n].preempt = 1; // エントリーポイントにジャンプ後、解除
    Scheduler::context[n].next = &Scheduler::context[0]; // ヘッドにつなげておく
    Scheduler::created_task_nums = n;

    // 次のスレッドへのポインタを設定
    Scheduler::context[n - 1].next = &Scheduler::context[n];

    return true;
}

/**
 * スケジューラー
 */
void Scheduler::schedule()
{
    // プリエンプト禁止
    Scheduler::disable_preempt();

    // コンテキストスイッチ
    Scheduler::switch_task();

    // プリエンプト許可
    Scheduler::enable_preempt();
}

/**
 * タスク切り替え
 */
void Scheduler::switch_task()
{
    if (!Scheduler::current->next) return; // 次のタスクが存在しない
    if (!Scheduler::current->next->stack) return; // スタック未設定時はスイッチしない
    if (Scheduler::current == Scheduler::current->next) return; // 同じタスクへはスイッチしない

    // 現在のスレッドから次のスレッドへ切り替える
    uint64_t** cur = &Scheduler::current->stack;
    Scheduler::current->status = SUSPEND;
    Scheduler::current = Scheduler::current->next;
    Scheduler::current->status = RUNNING;

    // TTBR切り替え
    __switch_ttbr(reinterpret_cast<uint64_t>(Scheduler::current->l1_ptb));

    // コンテキストスイッチ
    __switch(cur, &Scheduler::current->stack);
}

/**
 * タイマー割り込み通知
 */
void Scheduler::timer()
{
    if (Scheduler::current->preempt > 0) return;

    // 割り込みを有効化し、コンテキストスイッチ
    enable_irq();
    Scheduler::schedule();
    disable_irq();
}

/**
 * タスク情報出力
 */
void Scheduler::show_taskinfo()
{
    UART::send("\nTask Information\n");
    for(uint32_t i = 0 ; i < Scheduler::TASK_NUMS ; i++) {
        UART::send("\tNo: "); UART::sendHex(i);
        UART::send(" Start SP: "); UART::sendHex(reinterpret_cast<uint64_t>(Scheduler::context[i].base_stack));
        UART::send(" SP: "); UART::sendHex(reinterpret_cast<uint64_t>(Scheduler::context[i].stack));
        UART::send(" Status: "); UART::sendHex(Scheduler::context[i].status);
        UART::send(" Preempt: "); UART::sendHex(Scheduler::context[i].preempt);
        UART::send("\n");
    }
}
