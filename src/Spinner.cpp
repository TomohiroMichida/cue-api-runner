#include "Spinner.hpp"
#include <iostream>
#include <chrono>

Spinner::Spinner(const std::string& ticks, int intervalMs)
    : mTicks(ticks), mIntervalMs(intervalMs), mActive(false) {}

Spinner::~Spinner() {
    stop();
}

void Spinner::start() {
    if (mActive) return;
    mActive = true;
    mWorker = std::thread(&Spinner::run, this);
}

void Spinner::stop() {
    if (!mActive) return;
    mActive = false;
    if (mWorker.joinable()) {
        mWorker.join();
        std::cout << "\r \r" << std::flush;
    }
}

void Spinner::run() {
    size_t idx = 0;
    while (mActive) {
        char c = mTicks[idx++ % mTicks.size()];
        std::cout << "\r" << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(mIntervalMs));
    }
}
