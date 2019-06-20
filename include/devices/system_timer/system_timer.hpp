#pragma once

#include <stdint.h>

/**
 * Systemタイマークラス
 */
class SystemTimer {
public:
    // 初期化
    static void init();

    // タイマー周期取得
    static uint32_t getPeriod() { return SystemTimer::freq; }

    // クロック周波数
    static uint32_t freq;

    SystemTimer() = delete;
    ~SystemTimer() = delete;
    SystemTimer(const SystemTimer&) = delete;
    SystemTimer(const SystemTimer&&) = delete;
    SystemTimer& operator=(const SystemTimer&) = delete;
    SystemTimer& operator=(const SystemTimer&&) = delete;
};
