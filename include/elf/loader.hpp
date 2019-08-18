#pragma once

#include <cstdint>

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
 * ELFファイルローダークラス
 */
class ELFLoader {
public:
    // ELFデータ解析
    static void analysis(const uint8_t*);
    // ELFヘッダー取得
    static ELF64Header& getHeader() { return ELFLoader::elf.header; }

private:
    // ELFデータ
    static ELF64Data elf;

    // ヘッダーロード
    static void loadHeader(const uint8_t*);

    ELFLoader() = delete;
    ~ELFLoader() = delete;
    ELFLoader(const ELFLoader&) = delete;
    ELFLoader(const ELFLoader&&) = delete;
    ELFLoader& operator=(const ELFLoader&) = delete;
    ELFLoader& operator=(const ELFLoader&&) = delete;
};
