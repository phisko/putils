#pragma once

#ifdef _WIN32
#include <windows.h>
#include <processthreadsapi.h>
#endif

namespace putils {
	inline void set_thread_name(const wchar_t * name) {
#ifdef _WIN32
		SetThreadDescription(GetCurrentThread(), name);
#endif
	}
}