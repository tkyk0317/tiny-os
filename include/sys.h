#pragma once

// writeシステムコール
extern "C" void write_sys_call(const char*);

// forkシステムコール
typedef void* (*ENTRY_POINT)(void*);
extern "C" void fork_sys_call(ENTRY_POINT, void*);
