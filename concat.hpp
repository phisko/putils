#pragma once

#include <string>
#include <sstream>
#include "fwd.hpp"
#include "string.hpp"

namespace putils {
    template<typename ... Args>
    std::string concat(Args && ... args) noexcept;
}

#include "concat.inl"
