#include "memory.hpp"
#include "devices/uart/uart.hpp"

// スタック開始位置
uint8_t* MemoryManager::start_page = reinterpret_cast<uint8_t*>(0x3A000000);

// ページテーブル等
uint64_t MemoryManager::l1_ptb[MemoryManager::ENTRY_SIZE] = {0};
uint64_t MemoryManager::l2_ptb[MemoryManager::ENTRY_SIZE << 1] = {0};

/**
 * メモリアボート
 */
extern "C" int do_mem_abort(uint64_t far, uint64_t esr)
{
    (void) esr;
    return MemoryManager::abort(far);
}

/**
 * メモリアボート例外
 */
uint64_t MemoryManager::abort(uint64_t far)
{
    // DataAbortを検知した段階で、APを1へ書き換える（書き換えないとEL0タスクが動作しない）
    // AP=Data Access Permission(EL0: 0=None、1=Write/Read)
    MemoryManager::l2_ptb[far >> 21] |= APBITS_NO_LIMIT;
    return 0;
}

/**
 * 初期化処理
 */
void MemoryManager::init()
{
    // キャッシュポリシー
    uint64_t cp =
        (0xCC << 0) |    // AttrIdx=0: normal, IWBWA, OWBWA, NTR
        (0x04 << 8) |    // AttrIdx=1: device, nGnRE (must be OSH too)
        (0x00 <<16);     // AttrIdx=2: non cacheable

    // RAM領域880MB
    // エントリー登録時はAP=0で登録（MMUを有効にした時点でハングする）
    for (uint64_t i = 0; i < 440; i++) {
        MemoryManager::l2_ptb[i] =
            i << BLOCK_SHIFT | AFBITS_ACCESS | SH_INNER_SHAREABLE | APBITS_NO_EL0 | NSBITS_NON_SECURE | ATTR_INDEX_BITS_NORMAL | DESCRIPTOR_BLOCK;
    }
    // GPU領域128MB
    for (uint64_t i = 440; i < 504; i++) {
        MemoryManager::l2_ptb[i] = i << BLOCK_SHIFT | AFBITS_ACCESS | APBITS_NO_LIMIT | NSBITS_NON_SECURE | ATTR_INDEX_BITS_DEVICE_GRE | DESCRIPTOR_BLOCK;
    }

    // ペリフェラル領域
    for (uint64_t i = 504; i < 512; i++) {
        MemoryManager::l2_ptb[i] = i << BLOCK_SHIFT | AFBITS_ACCESS | APBITS_NO_LIMIT | NSBITS_NON_SECURE | ATTR_INDEX_BITS_DEVICE_NGNRNE | DESCRIPTOR_BLOCK;
    }

    // mailbox領域
    MemoryManager::l2_ptb[512] = 512LL << BLOCK_SHIFT | AFBITS_ACCESS | APBITS_NO_LIMIT | NSBITS_NON_SECURE | DESCRIPTOR_BLOCK;

    // l2 page table
    MemoryManager::l1_ptb[0] = 1LL << 63 | reinterpret_cast<uint64_t>(&MemoryManager::l2_ptb[0]) | DESCRIPTOR_PAGE; // first 1GB
    MemoryManager::l1_ptb[1] = 1LL << 63 | reinterpret_cast<uint64_t>(&MemoryManager::l2_ptb[512]) | DESCRIPTOR_PAGE; // second 1GB

    // TCRレジスタ値
    uint64_t tcr =
        (0b00LL << 37) | // TBI=0, no tagging
        (0b000LL << 32)| // IPS= 32 bit ... 000 = 32bit, 001 = 36bit, 010 = 40bit
        (0b10LL << 30) | // TG1=4k ... options are 10=4KB, 01=16KB, 11=64KB ... take care differs from TG0
        (0b11LL << 28) | // SH1=3 inner ... options 00 = Non-shareable, 01 = INVALID, 10 = Outer Shareable, 11 = Inner Shareable
        (0b01LL << 26) | // ORGN1=1 write back .. options 00 = Non-cacheable, 01 = Write back cacheable, 10 = Write thru cacheable, 11 = Write Back Non-cacheable
        (0b01LL << 24) | // IRGN1=1 write back .. options 00 = Non-cacheable, 01 = Write back cacheable, 10 = Write thru cacheable, 11 = Write Back Non-cacheable
        (0b0LL  << 23) | // EPD1 ... Translation table walk disable for translations using TTBR1_EL1  0 = walk, 1 = generate fault
        (25LL   << 16) | // T1SZ=25 (512G) ... The region size is 2 POWER (64-T1SZ) bytes
        (0b00LL << 14) | // TG0=4k  ... options are 00=4KB, 01=64KB, 10=16KB,  ... take care differs from TG1
        (0b11LL << 12) | // SH0=3 inner ... .. options 00 = Non-shareable, 01 = INVALID, 10 = Outer Shareable, 11 = Inner Shareable
        (0b01LL << 10) | // ORGN0=1 write back .. options 00 = Non-cacheable, 01 = Write back cacheable, 10 = Write thru cacheable, 11 = Write Back Non-cacheable
        (0b01LL << 8)  | // IRGN0=1 write back .. options 00 = Non-cacheable, 01 = Write back cacheable, 10 = Write thru cacheable, 11 = Write Back Non-cacheable
        (0b0LL  << 7)  | // EPD0  ... Translation table walk disable for translations using TTBR0_EL1  0 = walk, 1 = generate fault
        (25LL   << 0);   // T0SZ=25 (512G)  ... The region size is 2 POWER (64-T0SZ) bytes

    // 各種レジスタ設定
    asm volatile("dsb sy");
    asm volatile("msr mair_el1, %0; isb" :: "r" (cp));
    asm volatile("msr tcr_el1, %0; isb" :: "r" (tcr));
    asm volatile("msr ttbr0_el1, %0; isb" :: "r" (reinterpret_cast<uint64_t>(&MemoryManager::l1_ptb[0])));

    uint64_t sctlr = 0;
    asm volatile ("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (sctlr));
    sctlr |= 0xC00800;   // set mandatory reserved bits
    sctlr |=
        (0<<25) |   // clear EE
        (0<<23) |   // clear SPAN
        (0<<19) |   // clear WXN
        (1<<12) |   // I, Instruction cache enable.
        (1<<4)  |   // SA0, Stack Alignment Check Enable for EL0
        (1<<3)  |   // SA, Stack Alignment Check Enable
        (1<<2)  |   // C, Data cache enable.
        (1<<1)  |   // A, Alignment check enable bit
        (1<<0);     // set M, enable MMU
    asm volatile("msr sctlr_el1, %0; isb" :: "r" (sctlr));
}

/**
 * メモリ領域取得
 */
uint64_t* MemoryManager::get_page()
{
    uint64_t* p = reinterpret_cast<uint64_t*>(MemoryManager::start_page);
    MemoryManager::start_page -= MemoryManager::PAGE_SIZE;
    return p;
}
