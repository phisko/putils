#if defined(__unix__) || defined(__APPLE__)

#	include "unix_library.hpp"

// stl
#	include <sstream>
#	include <iostream>

// unix
#	include <dlfcn.h>

// putils
#	include "putils/profiling.hpp"

namespace putils {
	unix_library::unix_library(std::string_view name) noexcept
		: library(name),
		  _handle(dlopen(std::string(name).c_str(), RTLD_NOW)) {
		PUTILS_PROFILING_SCOPE;

		if (_handle == nullptr) {
			std::stringstream s;
			s << "Failed to load library '" << name << "': " << dlerror();
			std::cerr << s.str() << std::endl;
		}
	}

	unix_library::~unix_library() noexcept {
		PUTILS_PROFILING_SCOPE;

		if (_handle != nullptr)
			dlclose(_handle);
	}

	void * unix_library::load_symbol(const std::string & name) noexcept {
		PUTILS_PROFILING_SCOPE;
		return dlsym(_handle, name.c_str());
	}
}

#endif