#include "CppUTest/CommandLineTestRunner.h"
#include <iostream>
#include <scheduler.hpp>

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

