#pragma once

// stl
#include <string>
#include <vector>

namespace putils {
	PUTILS_CORE_EXPORT std::vector<std::string> split(const char * str, char delim) noexcept;
}