#include "memory.hpp"

char* MemoryManager::start_page = reinterpret_cast<char*>(0x00080000);

/**
 * メモリ領域取得
 */
uint64_t* MemoryManager::get_page()
{
    uint64_t* p = reinterpret_cast<uint64_t*>(MemoryManager::start_page);
    MemoryManager::start_page -= MemoryManager::PAGE_SIZE;
    return p;
}
