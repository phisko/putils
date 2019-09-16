#pragma once

#include <string_view>
#include <string.h>

namespace putils {
	static const char * file_extension(const char * file) {
		const auto dot = std::string_view(file).find_last_of(".");
		if (dot == std::string_view::npos)
			return file;
		return file + dot + 1;
	}

	template<size_t N>
	static putils::string<N> get_directory(const char * file) {
		const auto slash = std::string_view(file).find_last_of("/");
		assert(slash != std::string_view::npos);

		putils::string<N> ret;
		strncpy_s(ret, N, file, slash);
		return ret;
	}
}