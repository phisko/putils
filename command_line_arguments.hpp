#pragma once

namespace putils {
    template<typename T>
    T parseArguments(int ac, const char * const * av) noexcept;
}

#include "command_line_arguments.inl"