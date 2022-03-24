#ifdef _WIN32

#include "WindowsLibrary.hpp"
#include <algorithm>
#include <sstream>

#include "get_last_error_as_string.hpp"
#include "concatenate.hpp"

namespace putils {
	WindowsLibrary::WindowsLibrary(std::string_view name) noexcept : Library(name) {
		std::string copy(name);

		std::replace(copy.begin(), copy.end(), '/', '\\');
		_handle = LoadLibrary(copy.c_str());
		if (_handle == nullptr)
			std::cerr << "Failed to load library '" << name << "': " << GetLastErrorAsString() << '\n';
	}

	WindowsLibrary::~WindowsLibrary() noexcept {
		if (_handle != nullptr)
			FreeLibrary(_handle);
	}

	void * WindowsLibrary::loadSymbol(const std::string & name) noexcept {
		return (void *)GetProcAddress(_handle, name.c_str());
	}
}

#endif