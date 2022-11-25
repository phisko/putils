#pragma once

// stl
#include <limits>

namespace putils {
    template<typename T>
    T rand(T start = 0, T end = std::numeric_limits<T>::max()) noexcept;
}

#include "rand.inl"
