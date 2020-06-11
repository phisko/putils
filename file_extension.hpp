#pragma once

#include <string_view>
#include <string.h>

namespace putils {
	static std::string_view remove_directory(const char * file) {
		const auto slash = std::string_view(file).find_last_of("\\/");
		if (slash == std::string_view::npos)
			return file;
		return file + slash + 1;
	}
	
	static std::string_view file_extension(const char * file) {
		const auto dot = std::string_view(file).find_last_of(".");
		if (dot == std::string_view::npos)
			return file;
		return file + dot + 1;
	}

	static std::string_view get_directory(const char * file) {
		const auto slash = std::string_view(file).find_last_of("\\/");
		if (slash == std::string_view::npos)
			return file;
		return std::string_view(file, slash);
	}

	static std::string_view remove_extension(const char * file) {
		const auto dot = std::string_view(file).find_last_of(".");
		if (dot == std::string_view::npos)
			return file;
		return std::string_view(file, dot);
	}
}