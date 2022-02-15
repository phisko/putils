#pragma once

namespace putils {
    template<typename T>
    T parseArguments(int ac, const char ** av) noexcept;
}

#include "command_line_arguments.inl"