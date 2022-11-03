#pragma once

// stl
#include <algorithm>

// meta
#include "meta/fwd.hpp"

namespace putils {
    template<typename Container, typename Val>
    void erase(Container & container, Val && val) noexcept;
}

#include "erase.inl"