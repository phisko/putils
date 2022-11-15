#pragma once

// stl
#include <vector>
#include <span>
#include <string_view>

namespace putils {
    PUTILS_CORE_EXPORT std::vector<std::string_view> toArgumentVector(int ac, const char * const * av) noexcept;

    template<typename T>
    T parseArguments(std::span<const std::string_view> args, std::string_view help = "") noexcept;
}

#include "command_line_arguments.inl"