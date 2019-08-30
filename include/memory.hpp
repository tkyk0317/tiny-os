#pragma once

#include <cstdint>

// ページテーブル
const uint64_t MEMORY_ALIGN_SIZE = 4096;
const uint64_t MEMORY_ENTRY_SIZE = 512;

// ブロックディスクリプタアドレス位置
const uint64_t BLOCK_SHIFT = 9; // 2MB空間

// Descriptor種別
enum class DESCRIPTOR_BITS: uint64_t {
    DESCRIPTOR_BLOCK = 1 << 0,
    DESCRIPTOR_PAGE = 3 << 0,
};

// MemAttrIndexフィールド
enum class MEMATTR_INDEX_BITS: uint64_t {
    MEMATTR_INDEX_BITS_DEVICE_NGNRNE = 0,
    MEMATTR_INDEX_BITS_DEVICE_NGNRE = 1,
    MEMATTR_INDEX_BITS_DEVICE_GRE = 2,
    MEMATTR_INDEX_BITS_NORMAL_NC = 3,
    MEMATTR_INDEX_BITS_NORMAL = 4,
};

// NSフィールド
enum class NSBITS: uint64_t {
    NSBITS_NON_SECURE = 1, // non secure
    NSBITS_SECURE = 0,     // secure
};

// APフィールド
enum class APBITS: uint64_t {
    APBITS_NO_EL0 = 0,
    APBITS_NO_LIMIT = 1,
    APBITS_NO_WRITE_ELn_NO_EL0 = 2,
    APBITS_NO_WRITE_ELn = 3,
};

// SHフィールド
enum class SHBITS: uint64_t {
    SH_OUTER_SHAREABLE = 2, // Outter shareable
    SH_INNER_SHAREABLE = 3, // Inner shareable
};

// AFフィールド
enum class AFBITS: uint64_t {
    AFBITS_ACCESS = 1,    // can access
    AFBITS_NO_ACCESS = 0, // can not access
};

// APTABLEフィールド
enum class APTABLE_BITS: uint64_t {
    APTABLE_NOEFFECT = 0,          // No effect
    APTABLE_NO_EL0   = 1,          // Access at EL0 not permitted, regardless of permissions in subsequent levels of lookup
    APTABLE_NO_WRITE = 2,          // Write access not permitted, at any Exception level, regardless of permissions in subsequent levels of lookup
    APTABLE_NO_WRITE_EL0_READ = 3, // Write access not permitted,at any Exception level, Read access not permitted at EL0.
};

// テーブルディスクリプタ
typedef union {
    struct {
        DESCRIPTOR_BITS EntryType: 2;       // @0-1 Always 3 for a page table
        uint64_t _reserved2_11: 10;  // @2-11 Set to 0
        uint64_t Address: 36;        // @12-47 36 Bits of address
        uint64_t _reserved48_58: 11; // @48-58 Set to 0
        uint64_t PXNTable: 1;        // @59 Privileged eXecute Never limit for subsequent levels of lookup
        uint64_t XNTable: 1;         // @60 eXecute Never limit for subsequent levels of lookup
        APTABLE_BITS APTable: 2;     // @61-62 Access permission limit for subsequent levels of lookup
        uint64_t NSTable: 1;         // @63 Secure state, for accesses from Non-secure state this bit is RES0
                                     // and is ignored
    };
    uint64_t _raw64;
} TABLE_DESCRIPTOR;

// ブロックディスクリプタ
typedef union {
    struct {
        DESCRIPTOR_BITS EntryType: 2;      // @0-1 Always 1 for a block table
        MEMATTR_INDEX_BITS MemAttr: 3;        // @2-4
        NSBITS NS: 1;             // @5
        APBITS AP: 2;               // @6-7     Data Access permission bits
        SHBITS SH: 2;               // @8-9 Shareability field
        AFBITS AF: 1;             // @10 Access flag; if 0 an access to this page leads to an TLB Access fault
        uint64_t nG: 1;             // @11 not Global bit (ASID management for TLB)
        uint64_t Address: 36;       // @12-47 36 Bits of address
        uint64_t _reserved48_50: 3; // @48-51 Set to 0
        uint64_t DBM: 1;            // @51 Dirty Bit Modifier
        uint64_t Contiguous: 1;     // @52 Contiguous bit (for TLB optimization)
        uint64_t PXN: 1;            // @53 Privileged eXecute Never bit; execution is not allowed at EL0 if 1
        uint64_t XN: 1;             // @54 eXecute Never bit; execution is not allowed if 1
        uint64_t _reserved55_63: 9; // @55-63 Set to 0
    };
    uint64_t _raw64;
} BLOCK_DESCRIPTOR;

/**
 * メモリ管理クラス
 */
class MemoryManager {
public:
    // 初期化処理
    static void init(TABLE_DESCRIPTOR*);

    // メモリ取得
    static uint64_t* get_page();

    // メモリアボート例外
    static uint64_t abort(uint64_t);

    // EL0ページテーブル作成
    static void create_el0_table(TABLE_DESCRIPTOR*, BLOCK_DESCRIPTOR*);

private:
    // メモリマップ
    static const uint64_t RAM_SIZE = 1024 * 1024 * 880; // 880MB
    static const uint64_t PAGE_SIZE = 1024 * 1024 * 2; // 2MB
    static const uint64_t MEMORY_MAP_SIZE = RAM_SIZE / PAGE_SIZE;
    static bool memory_map[MEMORY_MAP_SIZE];

    MemoryManager() = delete;
    ~MemoryManager() = delete;
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager(const MemoryManager&&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&&) = delete;
};
