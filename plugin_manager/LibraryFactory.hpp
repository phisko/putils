#pragma once

#ifndef PUTILS_LIBRARY_MAX_PATH
# define PUTILS_LIBRARY_MAX_PATH 1024
#endif

// putils
#include "WindowsLibrary.hpp"
#include "UnixLibrary.hpp"

namespace putils {
    namespace LibraryFactory {
        PUTILS_PLUGIN_MANAGER_EXPORT putils::Library * make(const char * name) noexcept;
    }
}
