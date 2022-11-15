#pragma once

#ifdef _WIN32

// windows
#include <windows.h>

// putils
#include "ADirectory.hpp"

namespace putils {
	class PUTILS_CORE_EXPORT WindowsDirectory final : public putils::ADirectory {
	public:
		WindowsDirectory(const char * path) noexcept;

		WindowsDirectory(const WindowsDirectory &) = delete;
		WindowsDirectory &operator=(const WindowsDirectory &) = delete;

		~WindowsDirectory() noexcept;

		bool getNextFile(File & out) noexcept override;

	private:
		putils::string<PUTILS_DIRECTORY_MAX_PATH> _path;
		HANDLE _handle;
		WIN32_FIND_DATA	_ffd;
	};
}

#endif