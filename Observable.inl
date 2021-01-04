#include "Observable.hpp"

#define TObservable Observable<Args...>

namespace putils {
    template<typename ... Args>
    TObservable::Observable(const Observable &) noexcept {
    }

    template<typename ... Args>
	TObservable & TObservable::operator=(const Observable &) noexcept {
        return *this;
    }

	template<typename ... Args>
	intptr_t TObservable::addObserver(const Observer & observer) noexcept {
		auto id = (intptr_t)&observer;
		_observers.push_back({ id, observer });
		return id;
	}

	template<typename ... Args>
	TObservable & TObservable::operator+=(const Observer & observer) noexcept {
		addObserver(observer);
		return *this;
	}

	template<typename ... Args>
	void TObservable::removeObserver(intptr_t id) noexcept {
		_observers.erase(std::find_if(_observers.begin(), _observers.end(),
			[id](const auto & p) { return p.first == id; })
		);
	}

	template<typename ... Args>
	void TObservable::changed(const Args & ...args) noexcept {
		for (const auto & [_, func] : _observers)
			func(args...);
	}

	template<typename ... Args>
	void TObservable::operator()(const Args & ...args) noexcept {
		changed(args...);
	}
}

#undef TObservable