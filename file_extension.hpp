#pragma once

// stl
#include <string_view>

namespace putils {
	PUTILS_CORE_EXPORT std::string_view remove_directory(const char * file) noexcept;
	PUTILS_CORE_EXPORT std::string_view file_extension(const char * file) noexcept;
	PUTILS_CORE_EXPORT std::string_view get_directory(const char * file) noexcept;
	PUTILS_CORE_EXPORT std::string_view remove_extension(const char * file) noexcept;
}