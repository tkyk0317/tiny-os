#include "CppUTest/CommandLineTestRunner.h"
#include "lib/str.h"

TEST_GROUP(TestStr)
{
    TEST_SETUP() {}
    TEST_TEARDOWN() {}
};

/**
 * 概要：文字列比較一致（同じ文字列長）
 * 期待値：戻り値が0であること
 */
TEST(TestStr, TestStrCmpSuccess)
{
    CHECK(strcmp("test", "test") == 0);
    CHECK(strcmp("123456", "123456") == 0);

    const char target[] = "dummy";
    CHECK(strcmp("dummy", target) == 0);
}

/**
 * 概要：文字列比較不一致
 * 期待値：戻り値が-1であること
 */
TEST(TestStr, TestStrCmpFailed)
{
    CHECK(strcmp("test", "tes") != 0);
    CHECK(strcmp("123456", "23456") != 0);
    CHECK(strcmp("test", "tet") != 0);
}

/**
 * 概要：文字列比較一致（同じ文字列長）
 * 期待値：戻り値が0であること
 */
TEST(TestStr, TestStrnCmpSuccess)
{
    CHECK(strncmp("test", "test", 4) == 0);
    CHECK(strncmp("test", "test", 3) == 0);
    CHECK(strncmp("123456", "123456", 6) == 0);
    CHECK(strncmp("123456", "123456", 2) == 0);

    const char target[] = "dummy";
    CHECK(strncmp("dummy", target, 5) == 0);
    CHECK(strncmp("dummy", target, 4) == 0);
}

/**
 * 概要：文字列比較不一致
 * 期待値：戻り値が0以外であること
 */
TEST(TestStr, TestStrnCmpFailed)
{
    CHECK(strncmp("test", "abcde", 4) != 0);
    CHECK(strncmp("123456", "987654", 6) != 0);

    const char target[] = "dummy";
    CHECK(strncmp("testtest", target, 5) != 0);
}

/**
 * テストエントリーポイント
 */
int main(int argc, char* argv[])
{
    // CppUTest起動
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
