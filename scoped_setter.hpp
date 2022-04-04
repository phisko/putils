#pragma once

#include <cassert>
#include "meta/fwd.hpp"

namespace putils {
    template<typename T>
    struct ScopedSetter {
        template<typename U>
        ScopedSetter(T & dest, U && src) noexcept
            : dest(dest), oldValue(std::move(dest))
        {
            dest = FWD(src);
        }

        ~ScopedSetter()
        {
            dest = std::move(oldValue);
        }

    private:
        T & dest;
        T oldValue;
    };

    template<typename T, typename U>
    ScopedSetter<T> setForScope(T & dest, U && src) noexcept {
        return ScopedSetter<T>(dest, FWD(src));
    }
}