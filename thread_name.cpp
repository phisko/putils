#include "thread_name.hpp"

#ifdef _MSC_VER
#include <windows.h>
#include <processthreadsapi.h>
#endif

namespace putils {
	void set_thread_name(const wchar_t * name) noexcept {
#ifdef _MSC_VER
		SetThreadDescription(GetCurrentThread(), name);
#endif
	}

	const std::string & get_thread_name() noexcept {
		static thread_local const std::string ret = [] {
#ifdef _MSC_VER
			wchar_t * buff;
			GetThreadDescription(GetCurrentThread(), &buff);
			const std::wstring s = buff;
			return std::string{ s.begin(), s.end() };
#else
            return "";
#endif
		}();

		return ret;
	}
}