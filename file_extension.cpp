#include "file_extension.hpp"

namespace putils {
	std::string_view remove_directory(const char * file) noexcept {
		const auto slash = std::string_view(file).find_last_of("\\/");
		if (slash == std::string_view::npos)
			return file;
		return file + slash + 1;
	}
	
	std::string_view file_extension(const char * file) noexcept {
		const auto dot = std::string_view(file).find_last_of(".");
		if (dot == std::string_view::npos)
			return file;
		return file + dot + 1;
	}

	std::string_view get_directory(const char * file) noexcept {
		const auto slash = std::string_view(file).find_last_of("\\/");
		if (slash == std::string_view::npos)
			return "";
		return std::string_view(file, slash);
	}

	std::string_view remove_extension(const char * file) noexcept {
		const auto dot = std::string_view(file).find_last_of(".");
		if (dot == std::string_view::npos)
			return file;
		return std::string_view(file, dot);
	}
}
