#if defined(__unix__) || defined(__APPLE__)

#include "UnixLibrary.hpp"

// stl
#include <sstream>
#include <iostream>

// unix
#include <dlfcn.h>

// putils
#include "putils_profiling.hpp"

namespace putils {
	UnixLibrary::UnixLibrary(std::string_view name) noexcept
		:
		Library(name),
		_handle(dlopen(std::string(name).c_str(), RTLD_NOW))
	{
		PUTILS_PROFILING_SCOPE;

		if (_handle == nullptr) {
			std::stringstream s;
			s << "Failed to load library '" << name << "': " << dlerror();
			std::cerr << s.str() << std::endl;
		}
	}

	UnixLibrary::~UnixLibrary() noexcept {
		PUTILS_PROFILING_SCOPE;

		if (_handle != nullptr)
			dlclose(_handle);
	}

	void * UnixLibrary::loadSymbol(const std::string & name) noexcept {
		PUTILS_PROFILING_SCOPE;
		return dlsym(_handle, name.c_str());
	}
}

#endif