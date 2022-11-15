#ifdef _WIN32

#include "WindowsDirectory.hpp"

// windows
#include <direct.h>
#include <fileapi.h>
#include <handleapi.h>

// stl
#include <cassert>

// putils
#include "putils_profiling.hpp"

namespace putils {
	WindowsDirectory::WindowsDirectory(const char * path) noexcept : _path(path) {
		PUTILS_PROFILING_SCOPE;

		if (_path[_path.size() - 1] != '/')
			_path = _path + "/";
		_handle = FindFirstFile((_path + "*").c_str(), &_ffd);
		assert(_handle != nullptr);
	}

	WindowsDirectory::~WindowsDirectory() noexcept {
		PUTILS_PROFILING_SCOPE;

		if (_handle != nullptr)
			FindClose(_handle);
	}

	bool WindowsDirectory::getNextFile(File & out) noexcept {
		PUTILS_PROFILING_SCOPE;

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
}

#endif