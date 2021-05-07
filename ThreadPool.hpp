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

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "function.hpp"

namespace putils {
	class ThreadPool {
	public:
		using function = putils::function<void(), PUTILS_THREADPOOL_FUNCTION_SIZE>;

	public:
		ThreadPool(size_t threads) noexcept;
		~ThreadPool() noexcept;

		void runTask(function && f) noexcept;

		size_t getThreadCount() const noexcept {
			return _workers.size();
		}

	private:
		std::vector<std::thread> _workers;
		std::vector<function> _tasks;

		std::mutex _queueMutex;
		std::condition_variable _condition;
		bool _stop;
	};
}