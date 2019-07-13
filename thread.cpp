#include "thread.hpp"
#include "devices/uart/uart.hpp"

// アセンブラで定義したスレッド切り替え関数
extern "C" void switch_context(uint64_t**, uint64_t**);
extern "C" void switch_el0(uint64_t, uint64_t, uint64_t);

// スレッドコンテキスト
typedef struct _ThreadContext {
    uint64_t* stack; // スタック開始アドレス
    ThreadStatus status; // スレッド起動状態
    _ThreadContext* next; // 次のスレッド構造体へのポインタ
} ThreadContext;

// スレッド情報
const uint32_t THREAD_NUMS = 8;
uint32_t created_thread_num = 0;
ThreadContext context[THREAD_NUMS];

// カレントスレッド
ThreadContext* current = &context[0];

// スレッド用スタック開始アドレス
char* sp_start = reinterpret_cast<char*>(0x00060000);
const uint64_t PER_THREAD = 0x800; // 2048バイト

/**
 * 次のスレッドインデックス取得
 */
uint32_t next()
{
    if (created_thread_num >= THREAD_NUMS) return 0;
    return created_thread_num + 1;
}

/**
 * スレッド情報作成
 */
bool createThreadInfo(uint64_t* stack)
{
    uint32_t n = next();
    if (n == 0) return false; // もう作成できない

    // 起動するスレッドの情報を作成
    context[n].stack = stack;
    context[n].next = &context[0]; // ヘッドにつなげておく
    created_thread_num = next();

    // 次のスレッドへのポインタを設定
    context[n - 1].next = &context[n];
    return true;
}

/**
 * スレッド切り替え
 */
void do_switch()
{
    if (!current->next) return;

    // 現在のスレッドから次のスレッドへ切り替える
    uint64_t** cur = &current->stack;
    current->status = SUSPEND;
    current = current->next;
    current->status = RUNNING;

    // コンテキストスイッチ
    switch_context(cur, &current->stack);
}

/**
 * スレッドエントリーポイント
 */
void entry(THREAD_ENTRY fn, void* args)
{
    // EL0へ切り替え、指定エントリーポイントへ
    switch_el0(
      reinterpret_cast<uint64_t>(fn),
      reinterpret_cast<uint64_t>(current->stack),
      reinterpret_cast<uint64_t>(args)
    );
}

/**
 * スレッド開始
 */
void start_thread(THREAD_ENTRY fn, void* args)
{
    // 新しいスレッド用にスタックを割り当てる
    uint64_t* p = reinterpret_cast<uint64_t*>(sp_start);
    sp_start -= PER_THREAD;

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
    *p = reinterpret_cast<uint64_t>(entry); // x30

    // スレッド情報作成
    createThreadInfo(p);
}

