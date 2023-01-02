#include "observable.hpp"

// putils
#include "putils/profiling.hpp"

#define TObservable observable<Args...>

namespace putils {
	template<typename... Args>
	TObservable::observable(const observable &) noexcept {
	}

	template<typename... Args>
	TObservable & TObservable::operator=(const observable &) noexcept {
		return *this;
	}

	template<typename... Args>
	intptr_t TObservable::add_observer(const observer & observer) noexcept {
		PUTILS_PROFILING_SCOPE;

		_observers.push_back({ 0, observer });
		auto & new_observer = _observers.back();
		const auto id = (intptr_t)&new_observer;
		new_observer.first = id;
		return id;
	}

	template<typename... Args>
	TObservable & TObservable::operator+=(const observer & observer) noexcept {
		add_observer(observer);
		return *this;
	}

	template<typename... Args>
	void TObservable::remove_observer(intptr_t id) noexcept {
		PUTILS_PROFILING_SCOPE;

		_observers.erase(std::find_if(_observers.begin(), _observers.end(),
			[id](const auto & p) { return p.first == id; }));
	}

	template<typename... Args>
	void TObservable::changed(const Args &... args) noexcept {
		PUTILS_PROFILING_SCOPE;

		for (const auto & [_, func] : _observers)
			func(args...);
	}

	template<typename... Args>
	void TObservable::operator()(const Args &... args) noexcept {
		changed(args...);
	}
}

#undef TObservable