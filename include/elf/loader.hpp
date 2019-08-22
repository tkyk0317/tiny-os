#pragma once

#include <cstdint>

/**
 * プログラムヘッダー定数
 */
enum PTYPE {
    PT_NULL = 0, // 未使用セグメント
    PT_LOAD, // ロード可能セグメント
    PT_DYNAMIC, // 動的リンク情報セグメント
    PT_INTERP, // インタープリターとして起動されるパス名 (ヌル文字終端) の位置とサイズを指定
    PT_NOTE, // 補足情報 (auxiliary information) の位置とサイズ
    PT_SHLIB, // 予約領域
    PT_PHDR, // ファイルおよびプログラムのメモリーイメージ双方における プログラムヘッダーテーブル自身の位置とサイズ
};

/**
 * ELFヘッダー構造体
 */
typedef struct {
    uint8_t ident[16]; // ELF32/64の区別やエンディアン情報を格納
    uint16_t type; // ファイルタイプ
    uint16_t machine; // マシンアーキテクチャ
    uint32_t version; // Version
    uint64_t entry; // エントリーポイント
    uint64_t phoff; // プログラムヘッダ位置
    uint64_t shoff; // セクションヘッダ位置
    uint32_t flags; // unused
    uint16_t ehsize; // ELFヘッダサイズ
    uint16_t phentsize; // プログラムヘッダサイズ
    uint16_t phnum; // プログラムヘッダ個数
    uint16_t shentsize; // セクションヘッダサイズ
    uint16_t shnum; // セクションヘッダ個数
    uint16_t shstrndx; // セクション番号
} __attribute__ ((packed)) ELF64Header;

/**
 * ELF64プログラムヘッダー
 */
typedef struct {
    uint32_t type; // セグメントタイプ
    uint32_t flags; // フラグ(実行可能おう、書き込み可能、読み込み可能)
    uint64_t offset; // オフセット
    uint64_t v_addr; // セグメントロード先仮想アドレス
    uint64_t p_addr; // セグメントロード先物理アドレス（アプリプログラムではVMのみで動作するので使用しない）
    uint64_t size_in_file; // セグメントサイズ
    uint64_t size_in_mem; // メモリ上に展開される際のサイズ。基本的にはsize_in_fileと同じ
    uint64_t align; // アライメント
} __attribute__((packed)) ELF64ProgHeader;

/**
 * ELF64セクション
 */
typedef struct {
} ELF64Section;

/**
 * ELFデータ構造体
 */
typedef struct {
    ELF64Header header;
} ELF64Data;

/**
 * ELFプログラムヘッダーローダークラス
 */
class ELFLoaderProgHeader {
public:
    // プログラムヘッダーロード
    static ELF64ProgHeader load(const uint8_t*);

private:
    ELFLoaderProgHeader() = delete;
    ~ELFLoaderProgHeader() = delete;
    ELFLoaderProgHeader(const ELFLoaderProgHeader&) = delete;
    ELFLoaderProgHeader(const ELFLoaderProgHeader&&) = delete;
    ELFLoaderProgHeader& operator=(const ELFLoaderProgHeader&) = delete;
    ELFLoaderProgHeader& operator=(const ELFLoaderProgHeader&&) = delete;
};

/**
 * ELFファイルローダークラス
 */
class ELFLoader {
public:
    // ELFデータロード
    static void load(const uint8_t*);
    // ELFデータ取得
    static ELF64Data& getELF() { return ELFLoader::elf; }

private:
    // ELFデータ
    static ELF64Data elf;

    // ELFヘッダーロード
    static void loadELFHeader(const uint8_t*);

    // プログラムヘッダロード
    static ELF64ProgHeader loadProgHeader(const uint8_t*);

    ELFLoader() = delete;
    ~ELFLoader() = delete;
    ELFLoader(const ELFLoader&) = delete;
    ELFLoader(const ELFLoader&&) = delete;
    ELFLoader& operator=(const ELFLoader&) = delete;
    ELFLoader& operator=(const ELFLoader&&) = delete;
};
