#pragma once

#include <string>
#include "string.hpp"

namespace putils {
    std::string chop(std::string_view str) noexcept;

    template<size_t N>
    putils::string<N> chop(std::string_view str) noexcept;
}

#include "chop.inl"
