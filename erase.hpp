#pragma once

#include <algorithm>
#include "fwd.hpp"

namespace putils {
    template<typename Container, typename Val>
    void erase(Container & container, Val && val) {
        const auto it = std::find(container.begin(), container.end(), FWD(val));
        if (it == container.end())
            return;
        container.erase(it);
    }
}