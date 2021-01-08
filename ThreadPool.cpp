#include "ThreadPool.hpp"

namespace putils {
	// the constructor just launches some amount of workers
	ThreadPool::ThreadPool(size_t threads) noexcept
		: _stop(false)
	{
		for (size_t i = 0; i < threads; ++i)
			_workers.emplace_back([this] {
				putils::set_thread_name(L"ThreadPool worker");
				for (;;) {
					function task;

					{
						std::unique_lock<std::mutex> lock(_queueMutex);
						_condition.wait(lock,
							[this] { return _stop || !_tasks.empty(); });
						if (_stop && _tasks.empty())
							return;
						task = std::move(_tasks.back());
						_tasks.pop_back();
					}

					task();
				}
			});
	}

	// add new work item to the pool
	void ThreadPool::runTask(function && f) noexcept {
		if (_workers.empty()) {
			f();
			return;
		}

		{
			std::unique_lock<std::mutex> lock(_queueMutex);

			// don't allow enqueueing after stopping the pool
			assert("Enqueue on stopped ThreadPool" && !_stop);
			_tasks.emplace_back(std::move(f));
		}

		_condition.notify_one();
	}

	// the destructor joins all threads
	ThreadPool::~ThreadPool() noexcept {
		{
			std::unique_lock<std::mutex> lock(_queueMutex);
			_stop = true;
		}
		_condition.notify_all();
		for (auto & worker : _workers)
			worker.join();
	}
}