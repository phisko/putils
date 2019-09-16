#pragma once

#include <vector>
#include <functional>
#include <algorithm>

namespace putils {
    //
    // Subject for Observer pattern.
    // No need for an Observer class, observers are simply functions with no return.
    //
    // WARNING: observers should stay valid for as long as the subject is living
    // (since there is no way to remove an observer, calling it will result
    // in undefined behavior if it has been deleted)
    //

    template<typename ...Args>
    class Observable {
        // Constructor
    public:
        Observable() noexcept = default;

        // Coplien
        // Not default as observers shouldn't be copied
    public:
        Observable(const Observable &) noexcept {}

        Observable & operator=(const Observable &) noexcept { return *this; }

        // Move
    public:
        Observable(Observable &&) noexcept = default;

        Observable & operator=(Observable &&) noexcept = default;

        // Destructor
    public:
        ~Observable() noexcept = default;

        // Add an observer, a simple function returning void
    public:
        intptr_t addObserver(const std::function<void(const Args & ...)> & observer) noexcept {
            auto id = (intptr_t) &observer;
            _observers.push_back({ id, observer });
            return id;
        }
        Observable & operator+=(const std::function<void()> & observer) noexcept {
            addObserver(observer);
            return *this;
        }

    public:
        void removeObserver(intptr_t id) noexcept {
            _observers.erase(std::find_if(_observers.begin(), _observers.end(),
                                          [id](const auto & p) { return p.first == id; })
            );
        }

        // Notify observers that the observable has changed
    public:
        void changed(const Args & ...args) noexcept {
            for (const auto & [_, func] : _observers)
                func(args...);
        }
        void operator()(const Args & ...args) noexcept { changed(args...); }

        // Attributes
    private:
        std::vector<std::pair<intptr_t, std::function<void(const Args & ...)>>> _observers;
    };
}
