#pragma once

#include <type_traits>
#include <utility>
#include "fwd.hpp"

namespace putils {
    template<typename T, void (*Dtor)(T &), bool = std::is_scalar<T>()>
    class RAII;

    //
    // RAII template
    // Create one by giving it a resource and a function to release that resource
    //
    template<typename T, void (*Dtor)(T &)>
    class RAII<T, Dtor, false> {
    public:
		explicit RAII(T && res)
			: _res(std::forward<T>(res))
        {}

    public:
        RAII(RAII && other)
			: _res(std::move(other._res)) {
            other._release = false;
        }

        RAII & operator=(RAII && other) {
            std::swap(_res, other._res);
            std::swap(_release, other._release);
            return *this;
        }

        RAII & operator=(T && other) {
            if (_release)
                Dtor(_res);
            _res = std::move(other);
            _release = true;
            return *this;
        }

    public:
        RAII(const RAII &) = delete;
        RAII & operator=(const RAII &) = delete;

    public:
        ~RAII() {
            if (_release)
                Ddtor(_res);
        }

    public:
        T & get() noexcept { return _res; }
        const T & get() const noexcept { return _res; }

    private:
        T _res;
        bool _release { true };
    };

    template<typename T, void (*Dtor)(T &)>
    class RAII<T, Dtor, true> {
    public:
        explicit RAII(T res) : _res(res){}

    public:
        RAII(RAII && other)
			: _res(other._res) {
            other._release = false;
        }

        RAII & operator=(RAII && other) {
            std::swap(_res, other._res);
            std::swap(_release, other._release);
            return *this;
        }

        RAII & operator=(T other) {
            if (_release)
                Dtor(_res);
            _res = other;
            _release = true;
            return *this;
        }

    public:
        ~RAII() {
            if (_release)
                Dtor(_res);
        }

    public:
        RAII(const RAII &) = delete;
        RAII & operator=(const RAII &) = delete;

    public:
        T & get() noexcept { return _res; }
        const T & get() const noexcept { return _res; }

    private:
        T _res;
        bool _release { true };
    };
}
