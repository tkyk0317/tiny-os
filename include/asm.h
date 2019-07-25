#pragma once

#include <stdint.h>

// CurrentEL取得
extern "C" uint64_t get_el();

// Current SP取得
extern "C" uint64_t get_sp();

// コンテキストスイッチ
extern "C" void __switch(uint64_t**, uint64_t**);

// EL0切り替え
extern "C" void __switch_el0(uint64_t, uint64_t, uint64_t);

// 割り込み有効化
extern "C" void enable_irq();

// 割り込み無効化
extern "C" void disable_irq();

// writeシステムコール
extern "C" void write_sys_call(char*);

// forkシステムコール
typedef void* (*ENTRY_POINT)(void*);
extern "C" void fork_sys_call(ENTRY_POINT, void*);
