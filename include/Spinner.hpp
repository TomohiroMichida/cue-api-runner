#pragma once

#include <atomic>
#include <thread>
#include <string>

class Spinner {
public:
    // アニメーションに使う文字列と間隔(ms)を指定
    Spinner(const std::string& ticks = "|/-\\", int intervalMs = 100);
    ~Spinner();

    // スピナー開始・停止
    void start();
    void stop();

private:
    std::string mTicks;
    int mIntervalMs;
    std::atomic<bool> mActive;
    std::thread mWorker;

    void run();
};
