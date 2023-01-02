#pragma once

#ifndef PUTILS_LIBRARY_MAX_PATH
#define PUTILS_LIBRARY_MAX_PATH 1024
#endif

// putils
#include "windows_library.hpp"
#include "unix_library.hpp"

namespace putils {
	namespace library_factory {
		PUTILS_PLUGIN_MANAGER_EXPORT putils::library * make(const char * name) noexcept;
	}
}
