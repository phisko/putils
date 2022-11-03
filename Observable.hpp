#pragma once

// stl
#include <vector>
#include <algorithm>

// putils
#include "function.hpp"

#ifndef PUTILS_OBSERVER_SIZE
# define PUTILS_OBSERVER_SIZE 64
#endif

namespace putils {
    //
    // Subject for Observer pattern.
    // No need for an Observer class, observers are simply functions with no return.
    //
    // WARNING: observers should stay valid for as long as the subject is living
    // (since there is no way to remove an observer, calling it will result
    // in undefined behavior if it has been deleted)
    //

    template<typename ... Args>
    class Observable {
    public:
        using Observer = putils::function<void(const Args & ...), PUTILS_OBSERVER_SIZE>;

        Observable() noexcept = default;
        ~Observable() noexcept = default;

        Observable(const Observable &) noexcept;
        Observable & operator=(const Observable &) noexcept;
        Observable(Observable &&) noexcept = default;
        Observable & operator=(Observable &&) noexcept = default;

    public:
        intptr_t addObserver(const Observer & observer) noexcept;
        Observable & operator+=(const Observer & observer) noexcept;

        void removeObserver(intptr_t id) noexcept;

    public:
        void changed(const Args & ...args) noexcept;
        void operator()(const Args & ...args) noexcept;

    private:
        std::vector<std::pair<intptr_t, Observer>> _observers;
    };
}

#include "Observable.inl"
