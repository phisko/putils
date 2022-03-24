#if defined(__unix__) || defined(__APPLE__)

#include "UnixLibrary.hpp"
#include <dlfcn.h>
#include <sstream>
#include <iostream>

namespace putils {
	UnixLibrary::UnixLibrary(std::string_view name) noexcept
		:
		Library(name),
		_handle(dlopen(std::string(name).c_str(), RTLD_NOW))
	{
		if (_handle == nullptr) {
			std::stringstream s;
			s << "Failed to load library '" << name << "': " << dlerror();
			std::cerr << s.str() << '\n';
		}
	}

	UnixLibrary::~UnixLibrary() noexcept {
		if (_handle != nullptr)
			dlclose(_handle);
	}

	void * UnixLibrary::loadSymbol(const std::string & name) noexcept {
		return dlsym(_handle, name.c_str());
	}
}

#endif