#pragma once

// stl
#include <string>

namespace putils {
	PUTILS_CORE_EXPORT void set_thread_name(const wchar_t * name) noexcept;
	PUTILS_CORE_EXPORT const std::string & get_thread_name() noexcept;
}