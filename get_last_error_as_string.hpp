#pragma once

#ifdef _WIN32

// stl
#include <string>

namespace putils {
    std::string GetLastErrorAsString() noexcept;
}

#endif
