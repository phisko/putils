#pragma once

// stl
#include <vector>
#include <algorithm>

// putils
#include "function.hpp"

#ifndef PUTILS_OBSERVER_SIZE
#define PUTILS_OBSERVER_SIZE 64
#endif

namespace putils {
	//
	// Subject for observer pattern.
	// No need for an observer class, observers are simply functions with no return.
	//
	// WARNING: observers should stay valid for as long as the subject is living
	// (since there is no way to remove an observer, calling it will result
	// in undefined behavior if it has been deleted)
	//

	template<typename... Args>
	class observable {
	public:
		using observer = putils::function<void(const Args &...), PUTILS_OBSERVER_SIZE>;

		observable() noexcept = default;
		~observable() noexcept = default;

		observable(const observable &) noexcept;
		observable & operator=(const observable &) noexcept;
		observable(observable &&) noexcept = default;
		observable & operator=(observable &&) noexcept = default;

	public:
		intptr_t add_observer(const observer & observer) noexcept;
		observable & operator+=(const observer & observer) noexcept;

		void remove_observer(intptr_t id) noexcept;

	public:
		void changed(const Args &... args) noexcept;
		void operator()(const Args &... args) noexcept;

	private:
		std::vector<std::pair<intptr_t, observer>> _observers;
	};
}

#include "observable.inl"
