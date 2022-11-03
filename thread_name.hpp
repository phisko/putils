#pragma once

// stl
#include <string>

namespace putils {
	void set_thread_name(const wchar_t * name) noexcept;
	const std::string & get_thread_name() noexcept;
}