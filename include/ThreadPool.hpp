#pragma once
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>

class ThreadPool {
public:
	explicit ThreadPool(size_t numThreads);
	~ThreadPool();

	template<typename F, typename... Args>
	auto enqueue(F&& f, Args&&... args)->std::future<typename std::_Invoke_result_t<F, Args...>>
	{
		using ReturnType = typename std::invoke_result_t<F, Args...>;
		auto taskPtr = std::make_shared<std::packaged_task<ReturnType()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);
		std::future<ReturnType> response = taskPtr->get_future();
		{
			std::lock_guard<std::mutex> lock(mMutex);
			mTasks.emplace([taskPtr] { (*taskPtr)(); });
		}
		mCv.notify_one();

		return response;
	}

private:
	void workerLoop();

	std::vector<std::thread> mWorkers;
	std::queue<std::function<void()>> mTasks;

	std::mutex mMutex;
	std::condition_variable mCv;
	std::atomic<bool> mStop{ false };
};