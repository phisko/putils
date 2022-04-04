#pragma once

#include <string>
#include <sstream>
#include "meta/fwd.hpp"
#include "string.hpp"

namespace putils {
    template<typename ... Args>
    std::string concatenate(Args && ... args) noexcept;
}

#include "concatenate.inl"
