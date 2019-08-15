#include "CppUTest/CommandLineTestRunner.h"

/**
 * テストエントリーポイント
 */
int main(int argc, char* argv[])
{
    // CppUTest起動
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
