#pragma once

// stl
#include <string>
#include <sstream>

// putils
#include "string.hpp"

// meta
#include "meta/fwd.hpp"

namespace putils {
    template<typename ... Args>
    std::string concatenate(Args && ... args) noexcept;
}

#include "concatenate.inl"
