#include "thread_name.hpp"

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

	const std::string & get_thread_name() noexcept {
		static thread_local const std::string ret = [] {
			wchar_t * buff;
			GetThreadDescription(GetCurrentThread(), &buff);
			const std::wstring s = buff;
			return std::string{ s.begin(), s.end() };
		}();

		return ret;
	}
}