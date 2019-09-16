#pragma once

#include <random>
#include <limits>

namespace putils {
    template<typename T>
    inline T rand(T start = 0, T end = std::numeric_limits<T>::max) noexcept {
        if constexpr (std::is_floating_point<T>::value) {
            static std::random_device r;
            static std::default_random_engine e(r());
            std::uniform_real_distribution<T> d(start, end);
            return d(e);
        } else {
            static std::random_device r;
            static std::default_random_engine e(r());
            static std::uniform_int_distribution<int> d;
            return start + d(e) % (end - start);
        }
    }
}
