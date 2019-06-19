#include "thread.hpp"
#include "devices/uart/uart.hpp"

// アセンブラで定義したスレッド切り替え関数
extern "C" void switch_context(uint64_t**, uint64_t**);

// スレッドコンテキスト
typedef struct _ThreadContext {
    uint64_t* stack; // スタック開始アドレス
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
    current = current->next;
    switch_context(cur, &current->stack);
}

/**
 * スレッド開始
 */
void start_thread(THREAD_ENTRY fn)
{
    // 新しいスレッド用にスタックを割り当てる
    uint64_t* p = reinterpret_cast<uint64_t*>(sp_start);
    sp_start -= PER_THREAD;

    // LRレジスタの位置まで移動し、関数ポインタ設定
    p = p - 29;
    *p = reinterpret_cast<uint64_t>(fn);

    // スレッド切り替え
    if (createThreadInfo(p)) do_switch();
}

