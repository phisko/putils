#pragma once

// stl
#include <vector>
#include <span>
#include <string_view>

namespace putils {
    PUTILS_CORE_EXPORT std::vector<std::string_view> to_argument_vector(int ac, const char * const * av) noexcept;

    template<typename T>
    T parse_arguments(std::span<const std::string_view> args, std::string_view help = "") noexcept;
}

#include "command_line_arguments.inl"