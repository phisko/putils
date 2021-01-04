#pragma once

#ifdef _WIN32

#include <Windows.h>
#include <string_view>

#include "Library.hpp"

namespace putils {
	class WindowsLibrary final : public Library {
	public:
		WindowsLibrary(std::string_view name) noexcept;
		~WindowsLibrary() noexcept;

		void * loadSymbol(const std::string & name) noexcept override;

		// Attributes
	private:
		HMODULE _handle;
	};
}

#endif
