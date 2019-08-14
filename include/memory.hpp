#pragma once

#include <stdint.h>

// ブロックディスクリプタアドレス位置
const uint64_t BLOCK_SHIFT = 21;

// Descriptor種別
enum DESCRIPTOR_BITS {
    DESCRIPTOR_BLOCK = 1 << 0,
    DESCRIPTOR_PAGE = 3 << 0,
};

// AttrIndexフィールド
enum ATTR_INDEX_BITS {
    ATTR_INDEX_BITS_DEVICE_NGNRNE = 0 << 2,
    ATTR_INDEX_BITS_DEVICE_NGNRE = 1 << 2,
    ATTR_INDEX_BITS_DEVICE_GRE = 2 << 2,
    ATTR_INDEX_BITS_NORMAL_NC = 3 << 2,
    ATTR_INDEX_BITS_NORMAL = 4 << 2,
};

// NSフィールド
enum NSBITS {
    NSBITS_NON_SECURE = 1 << 5, // non secure
    NSBITS_SECURE = 0 << 5,     // secure
};

// APフィールド
enum APBITS {
    APBITS_NO_EL0 = 0 << 6,
    APBITS_NO_LIMIT = 1 << 6,
    APBITS_NO_WRITE_ELn_NO_EL0 = 2 << 6,
    APBITS_NO_WRITE_ELn = 3 << 6,
};

// SHフィールド
enum SHBITS {
    SH_OUTER_SHAREABLE = 2 << 8, // Outter shareable
    SH_INNER_SHAREABLE = 3 << 8, // Inner shareable
};

// AFフィールド
enum AFBITS {
    AFBITS_ACCESS = 1 << 10,    // can access
    AFBITS_NO_ACCESS = 0 << 10, // can not access
};

/**
 * メモリ管理クラス
 */
class MemoryManager {
public:
    // 初期化処理
    static void init();

    // メモリ取得
    static uint64_t* get_page();

    // メモリアボート例外
    static uint64_t abort(uint64_t);

private:
    static uint8_t* start_page;
    static const uint64_t PAGE_SIZE = 1024 * 4; // 4KB
    static const uint64_t BLOCK_SIZE = 1024 * 1024 * 2; // 2MB
    static const uint64_t ALIGN_SIZE = 4096;
    static const uint64_t ENTRY_SIZE = 512;

    // ページテーブル
    __attribute__((aligned(ALIGN_SIZE))) static uint64_t l1_ptb[ENTRY_SIZE];
    __attribute__((aligned(ALIGN_SIZE))) static uint64_t l2_ptb[ENTRY_SIZE << 1];

    MemoryManager() = delete;
    ~MemoryManager() = delete;
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(const MemoryManager&&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&&) = delete;
};
