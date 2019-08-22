/**
 * メモリ管理クラス実装ファイル
 *
 * MMUの設定やメモリ確保などの管理を責務とする
 *
 * https://developer.arm.com/architectures/learn-the-architecture/memory-management
 * https://armv8-ref.codingbelief.com/en/
 * https://github.com/s-matyukevich/raspberry-pi-os/tree/master/docs
 * https://github.com/Maldus512/uARM_pienv
 * https://github.com/LdB-ECM/Raspberry-Pi/tree/master/10_virtualmemory
 * https://github.com/bztsrc/raspi3-tutorial/tree/master/10_virtualmemory
 */
#include "memory.hpp"
#include "devices/uart/uart.hpp"

// ページテーブル
TABLE_DESCRIPTOR MemoryManager::l1_ptb[MemoryManager::ENTRY_SIZE] = {0};
BLOCK_DESCRIPTOR MemoryManager::l2_ptb[MemoryManager::ENTRY_SIZE << 1] = {0};

// メモリマップ
bool MemoryManager::memory_map[MemoryManager::MEMORY_MAP_SIZE] = {false};

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
    // ※12(Addressフィールド位置)+9bit(2MBセクションにする為のシフト値)=21bitsシフトした数値がインデックス
    MemoryManager::l2_ptb[far >> 21].AP = APBITS_NO_LIMIT;
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

    // SCTLRレジスタ
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

    // テーブル作成
    MemoryManager::create_el0_table();

    // 各種レジスタ設定
    asm volatile("dsb sy");
    asm volatile("msr mair_el1, %0; isb" :: "r" (cp));
    asm volatile("msr tcr_el1, %0; isb" :: "r" (tcr));
    asm volatile("msr ttbr0_el1, %0; isb" :: "r" (reinterpret_cast<uint64_t>(&MemoryManager::l1_ptb[0])));
    asm volatile("msr sctlr_el1, %0; isb" :: "r" (sctlr));
}

/**
 * EL0テーブル作成
 *
 * 2段のテーブル構成で、一段目で1GB、二段目で2MBの範囲をカバー
 * L1テーブルへ設定するL2テーブルのアドレスは、12bits右シフトしたものを設定しないと動作しない（QEMU4.0.0で確認）
 */
void MemoryManager::create_el0_table()
{
    // RAM領域880MB
    for (uint64_t i = 0; i < 440; i++) {
        MemoryManager::l2_ptb[i].Address = i << BLOCK_SHIFT;
        MemoryManager::l2_ptb[i].AF = AFBITS_ACCESS;
        MemoryManager::l2_ptb[i].SH = SH_INNER_SHAREABLE;
        MemoryManager::l2_ptb[i].AP = APBITS_NO_EL0; // エントリー登録時はAP=0で登録（MMUを有効にした時点でハングする）
        MemoryManager::l2_ptb[i].NS = NSBITS_NON_SECURE;
        MemoryManager::l2_ptb[i].MemAttr = MEMATTR_INDEX_BITS_NORMAL;
        MemoryManager::l2_ptb[i].EntryType = DESCRIPTOR_BLOCK;
    }
    // GPU領域128MB
    for (uint64_t i = 440; i < 504; i++) {
        MemoryManager::l2_ptb[i].Address = i << BLOCK_SHIFT;
        MemoryManager::l2_ptb[i].AF = AFBITS_ACCESS;
        MemoryManager::l2_ptb[i].AP = APBITS_NO_LIMIT;
        MemoryManager::l2_ptb[i].NS = NSBITS_NON_SECURE;
        MemoryManager::l2_ptb[i].MemAttr = MEMATTR_INDEX_BITS_DEVICE_GRE;
        MemoryManager::l2_ptb[i].EntryType = DESCRIPTOR_BLOCK;
    }
    // ペリフェラル領域
    for (uint64_t i = 504; i < 512; i++) {
        MemoryManager::l2_ptb[i].Address = i << BLOCK_SHIFT;
        MemoryManager::l2_ptb[i].AF = AFBITS_ACCESS;
        MemoryManager::l2_ptb[i].AP = APBITS_NO_LIMIT;
        MemoryManager::l2_ptb[i].NS = NSBITS_NON_SECURE;
        MemoryManager::l2_ptb[i].MemAttr = MEMATTR_INDEX_BITS_DEVICE_NGNRNE;
        MemoryManager::l2_ptb[i].EntryType = DESCRIPTOR_BLOCK;
    }
    // mailbox領域
    MemoryManager::l2_ptb[512].Address = 512LL << BLOCK_SHIFT;
    MemoryManager::l2_ptb[512].AF = AFBITS_ACCESS;
    MemoryManager::l2_ptb[512].AP = APBITS_NO_LIMIT;
    MemoryManager::l2_ptb[512].NS = NSBITS_NON_SECURE;
    MemoryManager::l2_ptb[512].EntryType = DESCRIPTOR_BLOCK;

    // L1テーブルへエントリー
    MemoryManager::l1_ptb[0].NSTable = 1;
    MemoryManager::l1_ptb[0].Address = reinterpret_cast<uint64_t>(&MemoryManager::l2_ptb[0]) >> 12; // 12bitsシフトしないと動作しない
    MemoryManager::l1_ptb[0].EntryType = DESCRIPTOR_PAGE;
    MemoryManager::l1_ptb[1].NSTable = 1;
    MemoryManager::l1_ptb[1].Address = reinterpret_cast<uint64_t>(&MemoryManager::l2_ptb[512]) >> 12; // 12bitsシフトしないと動作しない
    MemoryManager::l1_ptb[1].EntryType = DESCRIPTOR_PAGE;
}

/**
 * メモリ領域取得
 */
uint64_t* MemoryManager::get_page()
{
    // 未使用領域のメモリを渡す(0x3700_0000から下位アドレスに向けて使用する)
    uint64_t* p = 0;
    for (uint64_t i = MEMORY_MAP_SIZE - 1; i > 0; i++) {
        if (false == MemoryManager::memory_map[i]) {
            uint64_t addr = (i + 1) * PAGE_SIZE;
            p = reinterpret_cast<uint64_t*>(addr);
            MemoryManager::memory_map[i] = true;
            break;
        }
    }
    return p;
}

