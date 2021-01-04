#pragma once

#include <string>
#include "string.hpp"

namespace putils {
    template<size_t N = 0>
    auto chop(std::string_view str) noexcept {
        using Ret = std::conditional_t<
            N == 0,
            std::string,
            putils::string<N>
        >;

        return Ret(str.substr(0, str.find_last_not_of(" \t\n\r") + 1));
    }
}
