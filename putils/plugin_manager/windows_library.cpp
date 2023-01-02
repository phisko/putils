#ifdef _WIN32

#	include "windows_library.hpp"

// stl
#	include <algorithm>
#	include <sstream>

// putils
#	include "putils/get_last_error_as_string.hpp"
#	include "putils/concatenate.hpp"
#	include "putils/profiling.hpp"

namespace putils {
	windows_library::windows_library(std::string_view name) noexcept : library(name) {
		PUTILS_PROFILING_SCOPE;

		std::string copy(name);

		std::replace(copy.begin(), copy.end(), '/', '\\');
		_handle = LoadLibrary(copy.c_str());
		if (_handle == nullptr)
			std::cerr << "Failed to load library '" << name << "': " << get_last_error_as_string() << std::endl;
	}

	windows_library::~windows_library() noexcept {
		PUTILS_PROFILING_SCOPE;
		if (_handle != nullptr)
			FreeLibrary(_handle);
	}

	void * windows_library::load_symbol(const std::string & name) noexcept {
		PUTILS_PROFILING_SCOPE;
		return (void *)GetProcAddress(_handle, name.c_str());
	}
}

#endif