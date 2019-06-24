#pragma once

#include <stdint.h>

// スレッドエントリーポイント
using THREAD_ENTRY = void*(*)(void*);

// スレッド開始
void start_thread(THREAD_ENTRY, void*);

// スレッド切り替え
void do_switch();
