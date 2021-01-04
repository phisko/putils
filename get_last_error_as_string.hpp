#pragma once

#ifdef _WIN32

#include <string>

namespace putils {
    std::string GetLastErrorAsString() noexcept;
}

#endif
