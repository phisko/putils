#include "thread_name.hpp"

// windows
#ifdef _MSC_VER
#	include <windows.h>
#	include <processthreadsapi.h>
#endif

// putils
#include "putils/profiling.hpp"

namespace putils {
	void set_thread_name(const wchar_t * name) noexcept {
		PUTILS_PROFILING_SCOPE;

#ifdef _MSC_VER
		SetThreadDescription(GetCurrentThread(), name);
#endif
	}

	const std::string & get_thread_name() noexcept {
		PUTILS_PROFILING_SCOPE;

		static thread_local const std::string ret = [] {
#ifdef _MSC_VER
			wchar_t * buff;
			GetThreadDescription(GetCurrentThread(), &buff);
			const std::wstring s = buff;
#	pragma warning(disable : 4244) // Disable "conversion from wchar_t to char" warning
			return std::string{ s.begin(), s.end() };
#	pragma warning(default : 4244)
#else
			return "";
#endif
		}();

		return ret;
	}
}