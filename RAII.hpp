#pragma once

// stl
#include <type_traits>
#include <utility>

// meta
#include "meta/fwd.hpp"

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
        explicit RAII(T && res) noexcept;
        RAII(RAII && other) noexcept;
        RAII & operator=(RAII && other) noexcept;
        RAII & operator=(T && other) noexcept;
        ~RAII() noexcept;

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
        RAII() noexcept;
        explicit RAII(T res) noexcept;
        RAII(RAII && other) noexcept;
        RAII & operator=(RAII && other) noexcept;
        RAII & operator=(T other) noexcept;

        ~RAII() noexcept;

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

#include "RAII.inl"
