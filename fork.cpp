#include "fork.hpp"
#include "scheduler.hpp"
#include "asm.h"
#include "memory.hpp"
#include "devices/uart/uart.hpp"

/**
 * スレッドエントリーポイント
 */
void Process::entry(TASK_ENTRY fn, void* args)
{
    // EL0へ切り替え、指定エントリーポイントへ
    Scheduler::enable_preempt();
    __switch_el0(
        reinterpret_cast<uint64_t>(fn),
        reinterpret_cast<uint64_t>(Scheduler::current_task()->stack),
        reinterpret_cast<uint64_t>(args)
    );
}

/**
 * プロセス起動関連関数実装ファイル
 */
bool Process::fork(TASK_ENTRY fn, void* args)
{
    Scheduler::disable_preempt();

    // 新しいスレッド用にスタックを割り当てる
    uint64_t* p = MemoryManager::get_page();
    uint64_t* b = p;

    // スレッドエントリーポイントと引数を設定
    *p-- = reinterpret_cast<uint64_t>(fn); // x0
    *p-- = reinterpret_cast<uint64_t>(args); // x1
    *p-- = 0x0; // x2
    *p-- = 0x0; // x3
    *p-- = 0x0; // x4
    *p-- = 0x0; // x5
    *p-- = 0x0; // x6
    *p-- = 0x0; // x7
    *p-- = 0x0; // x8
    *p-- = 0x0; // x9
    *p-- = 0x0; // x10
    *p-- = 0x0; // x11
    *p-- = 0x0; // x12
    *p-- = 0x0; // x13
    *p-- = 0x0; // x14
    *p-- = 0x0; // x15
    *p-- = 0x0; // x16
    *p-- = 0x0; // x17
    *p-- = 0x0; // x18
    *p-- = 0x0; // x19
    *p-- = 0x0; // x20
    *p-- = 0x0; // x21
    *p-- = 0x0; // x22
    *p-- = 0x0; // x23
    *p-- = 0x0; // x24
    *p-- = 0x0; // x25
    *p-- = 0x0; // x26
    *p-- = 0x0; // x27
    *p-- = 0x0; // x28
    *p-- = 0x0; // x29
    *p = reinterpret_cast<uint64_t>(Process::entry); // x30

    // スレッド情報作成
    bool ret = Scheduler::register_task(p, b);

    Scheduler::enable_preempt();
    return ret;
}
