#pragma once

#ifdef _WIN32

// stl
#include <string>

namespace putils {
    PUTILS_CORE_EXPORT std::string GetLastErrorAsString() noexcept;
}

#endif
