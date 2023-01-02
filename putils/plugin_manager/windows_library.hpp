#pragma once

#ifdef _WIN32

// stl
#	include <string_view>

// windows
#	include <Windows.h>

// putils
#	include "library.hpp"

namespace putils {
	class PUTILS_PLUGIN_MANAGER_EXPORT windows_library final : public library {
	public:
		windows_library(std::string_view name) noexcept;
		~windows_library() noexcept;

		void * load_symbol(const std::string & name) noexcept override;

	private:
		HMODULE _handle;
	};
}

#endif
