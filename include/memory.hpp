#pragma once

#include <stdint.h>

/**
 * メモリ管理クラス
 */
class MemoryManager {
public:
    // メモリ取得
    static uint64_t* get_page();

private:
    static char* start_page;
    static const uint64_t PAGE_SIZE = 1024 * 4; // 4KB

    MemoryManager() = delete;
    ~MemoryManager() = delete;
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(const MemoryManager&&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&&) = delete;
};
