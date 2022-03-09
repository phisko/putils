#pragma once

namespace putils {
    template<typename T>
    T parseArguments(size_t ac, const char * const * av) noexcept;
}

#include "command_line_arguments.inl"