#pragma once

// stl
#include <string>

namespace putils {
	PUTILS_CORE_EXPORT std::string get_call_stack(int frames_to_ignore = 0) noexcept;
}