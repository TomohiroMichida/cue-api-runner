#pragma once

#include <atomic>
#include <thread>
#include <string>

class Spinner {
public:
    // �A�j���[�V�����Ɏg��������ƊԊu(ms)���w��
    Spinner(const std::string& ticks = "|/-\\", int intervalMs = 100);
    ~Spinner();

    // �X�s�i�[�J�n�E��~
    void start();
    void stop();

private:
    std::string mTicks;
    int mIntervalMs;
    std::atomic<bool> mActive;
    std::thread mWorker;

    void run();
};
