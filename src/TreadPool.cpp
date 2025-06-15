#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t numThreads) {
	for (size_t i = 0; i < numThreads; ++i) {
		mWorkers.emplace_back(&ThreadPool::workerLoop, this);
	}
}

ThreadPool::~ThreadPool() {
	mStop = true;
	mCv.notify_all();
	for (std::thread& const thread : mWorkers) {
		if (thread.joinable()) thread.join();
	}
}

void ThreadPool::workerLoop() {
	while (true) {
		std::function<void()> task;
		{
			std::unique_lock lock(mMutex);
			mCv.wait(lock, [&] { return mStop || !mTasks.empty(); });
			if (mStop && mTasks.empty()) return;
			task = std::move(mTasks.front());
			mTasks.pop();
		}
		task();
	}
}