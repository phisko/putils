#pragma once

// stl
#include <algorithm>

// meta
#include "meta/fwd.hpp"

namespace putils {
    // Remove items fitting a condition
    template<typename Container, typename Predicate>
    void remove_if(Container & container, Predicate && pred) noexcept {
        const auto it = std::remove_if(container.begin(), container.end(), FWD(pred));
        container.erase(it, container.end());
    }
}
