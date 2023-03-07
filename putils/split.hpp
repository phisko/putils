#pragma once

// stl
#include <string_view>
#include <vector>

namespace putils {
	PUTILS_CORE_EXPORT std::vector<std::string_view> split(std::string_view str, char delim) noexcept;
}