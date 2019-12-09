#pragma once

/*
Copyright(c) 2012 Jakob Progsch, Václav Zeman

This software is provided 'as-is', without any express or implied
warranty.In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software.If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

// Simplified version, changed attribute names and removed use of std::future

#ifndef PUTILS_THREADPOOL_FUNCTION_SIZE
# define PUTILS_THREADPOOL_FUNCTION_SIZE 128
#endif

#include <atomic>
#include <cassert>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "fwd.hpp"
#include "function.hpp"
#include "set_thread_name.hpp"

namespace putils {
	class ThreadPool {
	public:
		using function = putils::function<void(), PUTILS_THREADPOOL_FUNCTION_SIZE>;

	public:
		ThreadPool(size_t);
		template<class F>
		void enqueue(F&& f);
		template<class F>
		void runTask(F && f) { enqueue(FWD(f)); }
		~ThreadPool();

		void complete() const {
			while (_jobsLeft > 0);
		}
		void completeTasks() const { complete(); }

		size_t getThreadCount() const {
			return _workers.size();
		}

	private:
		// need to keep track of threads so we can join them
		std::vector<std::thread> _workers;
		// the task queue
		std::vector<function> _tasks;
		std::atomic_int _jobsLeft = 0;

		// synchronization
		std::mutex _queueMutex;
		std::condition_variable _condition;
		bool _stop;
	};

	// the constructor just launches some amount of workers
	inline ThreadPool::ThreadPool(size_t threads)
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
					--_jobsLeft;
				}
			});
	}

	// add new work item to the pool
	template<class F>
	void ThreadPool::enqueue(F&& f) {
		{
			std::unique_lock<std::mutex> lock(_queueMutex);

			// don't allow enqueueing after stopping the pool
			assert("Enqueue on stopped ThreadPool" && !_stop);
			_tasks.emplace_back(FWD(f));
			++_jobsLeft;
		}
		_condition.notify_one();
	}

	// the destructor joins all threads
	inline ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(_queueMutex);
			_stop = true;
		}
		_condition.notify_all();
		for (auto & worker : _workers)
			worker.join();
	}
}

/*
    Example:

    // create thread pool with 4 worker threads
    ThreadPool pool(4);

    // enqueue and store future
    auto result = pool.enqueue([](int answer) { return answer; }, 42);

    // get result from future
    std::cout << result.get() << std::endl;
*/
