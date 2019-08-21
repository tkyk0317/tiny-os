#include "CppUTest/CommandLineTestRunner.h"
#include "elf/loader.hpp"
#include <iostream>
#include <fstream>
#include <unistd.h>

TEST_GROUP(TestELF)
{
    TEST_SETUP() {}
    TEST_TEARDOWN() {}
};

TEST_GROUP(TestELFProgHeader)
{
    TEST_SETUP() {}
    TEST_TEARDOWN() {}
};

/**
 * 概要：ダミーのELFデータをロード
 * 期待値：すべてのヘッダーが適切にロードできていること
 */
TEST(TestELF, TestLoadHeader)
{
    uint8_t data[] = {
        /**
         * ELFヘッダ
         */
        // ident
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        // type
        0xAA, 0xBB,
        // machine
        0xBB, 0xCC,
        // version
        0x99, 0x88, 0x77, 0x66,
        // entry
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        // phoff
        0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        // shoff
        0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
        // flags
        0xFF, 0xFF, 0xFF, 0xFF,
        // ensize
        0x21, 0x32,
        // phensize
        0x40, 0x00,
        // phnum
        0x01, 0x00,
        // shentsize
        0x87, 0x98,
        // shnum
        0xA9, 0xBA,
        // shstrndx
        0xCB, 0xDC,
        /**
         * プログラムヘッダー
         */
        // type
        0x12, 0x34, 0x56, 0x78,
        // flags
        0x00, 0x00, 0x00, 0x00,
        // offset
        0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
        // v_addr
        0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12,
        // p_addr
        0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
        // size in file
        0x11, 0xAA, 0x22, 0xBB, 0x33, 0xCC, 0x44, 0xDD,
        // size in memory
        0x11, 0xAA, 0x22, 0xBB, 0x33, 0xCC, 0x44, 0xDD,
        // align
        0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    // テスト対象コール
    ELFLoader::load(data);

    // 期待値確認
    CHECK_EQUAL(0x10, ELFLoader::getELF().header.ident[0]);
    CHECK_EQUAL(0x11, ELFLoader::getELF().header.ident[1]);
    CHECK_EQUAL(0x12, ELFLoader::getELF().header.ident[2]);
    CHECK_EQUAL(0x13, ELFLoader::getELF().header.ident[3]);
    CHECK_EQUAL(0x14, ELFLoader::getELF().header.ident[4]);
    CHECK_EQUAL(0x15, ELFLoader::getELF().header.ident[5]);
    CHECK_EQUAL(0x16, ELFLoader::getELF().header.ident[6]);
    CHECK_EQUAL(0x17, ELFLoader::getELF().header.ident[7]);
    CHECK_EQUAL(0x18, ELFLoader::getELF().header.ident[8]);
    CHECK_EQUAL(0x19, ELFLoader::getELF().header.ident[9]);
    CHECK_EQUAL(0x1A, ELFLoader::getELF().header.ident[10]);
    CHECK_EQUAL(0x1B, ELFLoader::getELF().header.ident[11]);
    CHECK_EQUAL(0x1C, ELFLoader::getELF().header.ident[12]);
    CHECK_EQUAL(0x1D, ELFLoader::getELF().header.ident[13]);
    CHECK_EQUAL(0x1E, ELFLoader::getELF().header.ident[14]);
    CHECK_EQUAL(0x1F, ELFLoader::getELF().header.ident[15]);
    CHECK_EQUAL(0xBBAA, ELFLoader::getELF().header.type);
    CHECK_EQUAL(0xCCBB, ELFLoader::getELF().header.machine);
    CHECK_EQUAL(0x66778899, ELFLoader::getELF().header.version);
    CHECK_EQUAL(0x8877665544332211, ELFLoader::getELF().header.entry);
    CHECK_EQUAL(0x0000000000000040, ELFLoader::getELF().header.phoff);
    CHECK_EQUAL(0xBBBBBBBBBBBBBBBB, ELFLoader::getELF().header.shoff);
    CHECK_EQUAL(0xFFFFFFFF, ELFLoader::getELF().header.flags);
    CHECK_EQUAL(0x3221, ELFLoader::getELF().header.ehsize);
    CHECK_EQUAL(0x0040, ELFLoader::getELF().header.phentsize);
    CHECK_EQUAL(0x0001, ELFLoader::getELF().header.phnum);
    CHECK_EQUAL(0x9887, ELFLoader::getELF().header.shentsize);
    CHECK_EQUAL(0xBAA9, ELFLoader::getELF().header.shnum);
    CHECK_EQUAL(0xDCCB, ELFLoader::getELF().header.shstrndx);
}

/**
 * 概要：サンプルELFファイルをロード
 * 期待値：適切にELFヘッダーがロードできていること
 */
TEST(TestELF, TestLoadHeaderFromSampleFile)
{
    // サンプルデータをすべて読み込む
    std::ifstream fin("../../../tests/elf/sample.elf", std::ios::in | std::ios::binary );
    if (!fin) {
        std::cout << "cannot open file" << std::endl;
        return;
    }

    auto begin = static_cast<int>(fin.tellg());
    fin.seekg(0, fin.end);
    auto end = static_cast<int>(fin.tellg());
    auto size = end - begin;
    fin.clear();
    fin.seekg(0, fin.beg);
    uint8_t* data = new uint8_t[size + 1];
    data[size] = 0;
    fin.read(reinterpret_cast<char*>(data), size);

    // テスト対象コール
    ELFLoader::load(data);

    // 期待値確認
    CHECK_EQUAL(0x7F, ELFLoader::getELF().header.ident[0]);
    CHECK_EQUAL(0x45, ELFLoader::getELF().header.ident[1]);
    CHECK_EQUAL(0x4c, ELFLoader::getELF().header.ident[2]);
    CHECK_EQUAL(0x46, ELFLoader::getELF().header.ident[3]);
    CHECK_EQUAL(0x02, ELFLoader::getELF().header.ident[4]);
    CHECK_EQUAL(0x01, ELFLoader::getELF().header.ident[5]);
    CHECK_EQUAL(0x01, ELFLoader::getELF().header.ident[6]);
    CHECK_EQUAL(0x00, ELFLoader::getELF().header.ident[7]);
    CHECK_EQUAL(0x00, ELFLoader::getELF().header.ident[8]);
    CHECK_EQUAL(0x00, ELFLoader::getELF().header.ident[9]);
    CHECK_EQUAL(0x00, ELFLoader::getELF().header.ident[10]);
    CHECK_EQUAL(0x00, ELFLoader::getELF().header.ident[11]);
    CHECK_EQUAL(0x00, ELFLoader::getELF().header.ident[12]);
    CHECK_EQUAL(0x00, ELFLoader::getELF().header.ident[13]);
    CHECK_EQUAL(0x00, ELFLoader::getELF().header.ident[14]);
    CHECK_EQUAL(0x00, ELFLoader::getELF().header.ident[15]);
    CHECK_EQUAL(0x0003, ELFLoader::getELF().header.type);
    CHECK_EQUAL(0x003e, ELFLoader::getELF().header.machine);
    CHECK_EQUAL(0x00000001, ELFLoader::getELF().header.version);
    CHECK_EQUAL(0x0000000000001020, ELFLoader::getELF().header.entry);
    CHECK_EQUAL(0x0000000000000040, ELFLoader::getELF().header.phoff);
    CHECK_EQUAL(0x00000000000038E8, ELFLoader::getELF().header.shoff);
    CHECK_EQUAL(0x00000000, ELFLoader::getELF().header.flags);
    CHECK_EQUAL(0x0040, ELFLoader::getELF().header.ehsize);
    CHECK_EQUAL(0x0038, ELFLoader::getELF().header.phentsize);
    CHECK_EQUAL(0x000B, ELFLoader::getELF().header.phnum);
    CHECK_EQUAL(0x0040, ELFLoader::getELF().header.shentsize);
    CHECK_EQUAL(0x001B, ELFLoader::getELF().header.shnum);
    CHECK_EQUAL(0x001A, ELFLoader::getELF().header.shstrndx);
    CHECK_EQUAL(0x001A, ELFLoader::getELF().header.shstrndx);

    delete[] data;
}

/**
 * 概要：ダミーのELFデータをロード
 * 期待値：すべてのヘッダーが適切にロードできていること
 */
TEST(TestELFProgHeader, TestLoadHeader)
{
    uint8_t data[] = {
        /**
         * ELFプログラムヘッダー
         */
        // type
        0x12, 0x34, 0x56, 0x78,
        // flags
        0x00, 0x00, 0x00, 0x00,
        // offset
        0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
        // v_addr
        0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12,
        // p_addr
        0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0,
        // size in file
        0x11, 0xAA, 0x22, 0xBB, 0x33, 0xCC, 0x44, 0xDD,
        // size in memory
        0xAA, 0x11, 0xBB, 0x22, 0xCC, 0x33, 0xDD, 0x44,
        // align
        0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    // テスト対象コール
    ELF64ProgHeader h = ELFLoaderProgHeader::load(data);

    // 期待値確認
    CHECK_EQUAL(0x78563412, h.type);
    CHECK_EQUAL(0x00000000, h.flags);
    CHECK_EQUAL(0x8899AABBCCDDEEFF, h.offset);
    CHECK_EQUAL(0x12EFCDAB90785634, h.v_addr);
    CHECK_EQUAL(0xF0E0D0C0B0A09080, h.p_addr);
    CHECK_EQUAL(0xDD44CC33BB22AA11, h.size_in_file);
    CHECK_EQUAL(0x44DD33CC22BB11AA, h.size_in_mem);
    CHECK_EQUAL(0x0000000000000008, h.align);
}

/**
 * テストエントリーポイント
 */
int main(int argc, char* argv[])
{
    // CppUTest起動
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
