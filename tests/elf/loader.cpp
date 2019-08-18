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

/**
 * 概要：ダミーのELFデータをロード
 * 期待値：すべてのヘッダーが適切にロードできていること
 */
TEST(TestELF, TestLoadHeader)
{
    uint8_t data[] = {
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
        0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
        // shoff
        0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
        // flags
        0xFF, 0xFF, 0xFF, 0xFF,
        // ensize
        0x21, 0x32,
        // phensize
        0x43, 0x54,
        // phnum
        0x65, 0x76,
        // shentsize
        0x87, 0x98,
        // shnum
        0xA9, 0xBA,
        // shstrndx
        0xCB, 0xDC,
    };

    // テスト対象コール
    ELFLoader::analysis(data);

    // 期待値確認
    CHECK_EQUAL(0x10, ELFLoader::getHeader().ident[0]);
    CHECK_EQUAL(0x11, ELFLoader::getHeader().ident[1]);
    CHECK_EQUAL(0x12, ELFLoader::getHeader().ident[2]);
    CHECK_EQUAL(0x13, ELFLoader::getHeader().ident[3]);
    CHECK_EQUAL(0x14, ELFLoader::getHeader().ident[4]);
    CHECK_EQUAL(0x15, ELFLoader::getHeader().ident[5]);
    CHECK_EQUAL(0x16, ELFLoader::getHeader().ident[6]);
    CHECK_EQUAL(0x17, ELFLoader::getHeader().ident[7]);
    CHECK_EQUAL(0x18, ELFLoader::getHeader().ident[8]);
    CHECK_EQUAL(0x19, ELFLoader::getHeader().ident[9]);
    CHECK_EQUAL(0x1A, ELFLoader::getHeader().ident[10]);
    CHECK_EQUAL(0x1B, ELFLoader::getHeader().ident[11]);
    CHECK_EQUAL(0x1C, ELFLoader::getHeader().ident[12]);
    CHECK_EQUAL(0x1D, ELFLoader::getHeader().ident[13]);
    CHECK_EQUAL(0x1E, ELFLoader::getHeader().ident[14]);
    CHECK_EQUAL(0x1F, ELFLoader::getHeader().ident[15]);
    CHECK_EQUAL(0xBBAA, ELFLoader::getHeader().type);
    CHECK_EQUAL(0xCCBB, ELFLoader::getHeader().machine);
    CHECK_EQUAL(0x66778899, ELFLoader::getHeader().version);
    CHECK_EQUAL(0x8877665544332211, ELFLoader::getHeader().entry);
    CHECK_EQUAL(0xAAAAAAAAAAAAAAAA, ELFLoader::getHeader().phoff);
    CHECK_EQUAL(0xBBBBBBBBBBBBBBBB, ELFLoader::getHeader().shoff);
    CHECK_EQUAL(0xFFFFFFFF, ELFLoader::getHeader().flags);
    CHECK_EQUAL(0x3221, ELFLoader::getHeader().ehsize);
    CHECK_EQUAL(0x5443, ELFLoader::getHeader().phentsize);
    CHECK_EQUAL(0x7665, ELFLoader::getHeader().phnum);
    CHECK_EQUAL(0x9887, ELFLoader::getHeader().shentsize);
    CHECK_EQUAL(0xBAA9, ELFLoader::getHeader().shnum);
    CHECK_EQUAL(0xDCCB, ELFLoader::getHeader().shstrndx);
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
    ELFLoader::analysis(data);

    // 期待値確認
    CHECK_EQUAL(0x7F, ELFLoader::getHeader().ident[0]);
    CHECK_EQUAL(0x45, ELFLoader::getHeader().ident[1]);
    CHECK_EQUAL(0x4c, ELFLoader::getHeader().ident[2]);
    CHECK_EQUAL(0x46, ELFLoader::getHeader().ident[3]);
    CHECK_EQUAL(0x02, ELFLoader::getHeader().ident[4]);
    CHECK_EQUAL(0x01, ELFLoader::getHeader().ident[5]);
    CHECK_EQUAL(0x01, ELFLoader::getHeader().ident[6]);
    CHECK_EQUAL(0x00, ELFLoader::getHeader().ident[7]);
    CHECK_EQUAL(0x00, ELFLoader::getHeader().ident[8]);
    CHECK_EQUAL(0x00, ELFLoader::getHeader().ident[9]);
    CHECK_EQUAL(0x00, ELFLoader::getHeader().ident[10]);
    CHECK_EQUAL(0x00, ELFLoader::getHeader().ident[11]);
    CHECK_EQUAL(0x00, ELFLoader::getHeader().ident[12]);
    CHECK_EQUAL(0x00, ELFLoader::getHeader().ident[13]);
    CHECK_EQUAL(0x00, ELFLoader::getHeader().ident[14]);
    CHECK_EQUAL(0x00, ELFLoader::getHeader().ident[15]);
    CHECK_EQUAL(0x0003, ELFLoader::getHeader().type);
    CHECK_EQUAL(0x003e, ELFLoader::getHeader().machine);
    CHECK_EQUAL(0x00000001, ELFLoader::getHeader().version);
    CHECK_EQUAL(0x0000000000001020, ELFLoader::getHeader().entry);
    CHECK_EQUAL(0x0000000000000040, ELFLoader::getHeader().phoff);
    CHECK_EQUAL(0x00000000000038E8, ELFLoader::getHeader().shoff);
    CHECK_EQUAL(0x00000000, ELFLoader::getHeader().flags);
    CHECK_EQUAL(0x0040, ELFLoader::getHeader().ehsize);
    CHECK_EQUAL(0x0038, ELFLoader::getHeader().phentsize);
    CHECK_EQUAL(0x000B, ELFLoader::getHeader().phnum);
    CHECK_EQUAL(0x0040, ELFLoader::getHeader().shentsize);
    CHECK_EQUAL(0x001B, ELFLoader::getHeader().shnum);
    CHECK_EQUAL(0x001A, ELFLoader::getHeader().shstrndx);

    delete[] data;
}

/**
 * テストエントリーポイント
 */
int main(int argc, char* argv[])
{
    // CppUTest起動
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
