#include "set_thread_name.hpp"

#ifdef _WIN32
#include <windows.h>
#include <processthreadsapi.h>
#endif

namespace putils {
	void set_thread_name(const wchar_t * name) noexcept {
#ifdef _WIN32
		SetThreadDescription(GetCurrentThread(), name);
#endif
	}
}