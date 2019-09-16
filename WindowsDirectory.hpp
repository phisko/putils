#pragma once

#ifdef _WIN32

#include <direct.h>
#include <windows.h>
#include <fileapi.h>
#include <handleapi.h>
#include <assert.h>
#include "ADirectory.hpp"
#include "concat.hpp"
#include "string.hpp"

namespace putils
{
	class WindowsDirectory final : public putils::ADirectory
	{
		// Constructor
	public:
		WindowsDirectory(const char * path) : _path(path) {
			if (_path[_path.size() - 1] != '/')
				_path = _path + "/";
			_handle = FindFirstFile((_path + "*").c_str(), &_ffd);
			assert(_handle != nullptr);
		}

		WindowsDirectory(const WindowsDirectory &) = delete;
		WindowsDirectory &operator=(const WindowsDirectory &) = delete;

		virtual ~WindowsDirectory() noexcept {
			if (_handle != nullptr)
				FindClose(_handle);
		}

		// Read next directory entry
	public:
		bool getNextFile(File & out) noexcept override {
			if (_handle == INVALID_HANDLE_VALUE)
				return false;

			if (FindNextFile(_handle, &_ffd) == false)
				return false;

			out.name = _ffd.cFileName;
			if (out.name == "." || out.name == "..")
				return getNextFile(out);

			out.fullPath = _path + out.name;
			out.isDirectory = (bool)(_ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

			return true;
		}

		// Attributes
	private:
		putils::string<PUTILS_DIRECTORY_MAX_PATH> _path;
		HANDLE _handle;
		WIN32_FIND_DATA	_ffd;
	};
}

#endif