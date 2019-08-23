#include "CppUTest/CommandLineTestRunner.h"
#include <iostream>
#include <scheduler.hpp>
#include "memory.hpp"

// Mock
void MemoryManager::create_el0_table(TABLE_DESCRIPTOR*, BLOCK_DESCRIPTOR*) {}
uint64_t* MemoryManager::get_page() { return 0; }
extern "C" void __switch_ttbr(uint64_t) {}

// テストグループを定義
TEST_GROUP(TestSchdule)
{
    TEST_SETUP() {}
    TEST_TEARDOWN() {}
};

/**
 * 概要：タスク登録テスト
 * 期待値：戻り値がtrueであること
 */
TEST(TestSchdule, SuccessSchedulerRegisterTask)
{
    CHECK_EQUAL(Scheduler::register_task(NULL, NULL), true);
}

