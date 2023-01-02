#pragma once

#if defined(__unix__) || defined(__APPLE__)

// stl
#include <string_view>

// putils
#include "library.hpp"

namespace putils {
	class unix_library final : public library {
	public:
		unix_library(std::string_view name) noexcept;
		~unix_library() noexcept;

		void * load_symbol(const std::string & name) noexcept override;

	private:
		void * _handle = nullptr;
	};
}

#endif