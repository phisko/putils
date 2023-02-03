#include "thread_name.hpp"

// windows
#ifdef _MSC_VER
#include <windows.h>
#include <processthreadsapi.h>
#endif

// putils
#include "putils/profiling.hpp"
#include "putils/range.hpp"

namespace putils {
	void set_thread_name(std::string_view name) noexcept {
		PUTILS_PROFILING_SCOPE;

#ifdef _MSC_VER
		SetThreadDescription(GetCurrentThread(), std::wstring{ putils_range(name) }.c_str());
#endif
	}

	std::string get_thread_name() noexcept {
		PUTILS_PROFILING_SCOPE;

#ifdef _MSC_VER
		wchar_t * buff;
		GetThreadDescription(GetCurrentThread(), &buff);
		const std::wstring s = buff;
#pragma warning(disable : 4244) // Disable "conversion from wchar_t to char" warning
		return std::string{ putils_range(s) };
#pragma warning(default : 4244)
#else
		return "";
#endif
	}
}