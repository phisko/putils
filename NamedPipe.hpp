#pragma once

#ifdef _WIN32

#include "WindowsNamedPipe.hpp"

namespace putils
{
    using NamedPipe = WindowsNamedPipe;
}

#elif defined(__unix__)

#include "UnixNamedPipe.hpp"

namespace putils {
    using NamedPipe = UnixNamedPipe;
}

#endif
