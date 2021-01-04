#pragma once

#include <string_view>

namespace putils {
	std::string_view remove_directory(const char * file) noexcept;
	std::string_view file_extension(const char * file) noexcept;
	std::string_view get_directory(const char * file) noexcept;
	std::string_view remove_extension(const char * file) noexcept;
}