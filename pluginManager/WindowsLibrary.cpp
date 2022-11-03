#ifdef _WIN32

#include "WindowsLibrary.hpp"

// stl
#include <algorithm>
#include <sstream>

// putils
#include "get_last_error_as_string.hpp"
#include "concatenate.hpp"
#include "putils_profiling.hpp"

namespace putils {
	WindowsLibrary::WindowsLibrary(std::string_view name) noexcept : Library(name) {
		PUTILS_PROFILING_SCOPE;

		std::string copy(name);

		std::replace(copy.begin(), copy.end(), '/', '\\');
		_handle = LoadLibrary(copy.c_str());
		if (_handle == nullptr)
			std::cerr << "Failed to load library '" << name << "': " << GetLastErrorAsString() << std::endl;
	}

	WindowsLibrary::~WindowsLibrary() noexcept {
		PUTILS_PROFILING_SCOPE;
		if (_handle != nullptr)
			FreeLibrary(_handle);
	}

	void * WindowsLibrary::loadSymbol(const std::string & name) noexcept {
		PUTILS_PROFILING_SCOPE;
		return (void *)GetProcAddress(_handle, name.c_str());
	}
}

#endif