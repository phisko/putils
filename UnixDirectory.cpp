#ifdef __unix__

#include "UnixDirectory.hpp"

// unix
#include <sys/stat.h>

namespace putils {
	UnixDirectory::UnixDirectory(const std::string & path) noexcept
		:
		_handle(opendir(path.c_str())),
		_path(path)
	{
		assert(_handle != nullptr);
		if (_path[_path.length() - 1] != '/')
			_path = _path + "/";
	}

	UnixDirectory::~UnixDirectory() noexcept {
		if (_handle != nullptr)
			closedir(_handle);
	}

	bool UnixDirectory::getNextFile(File & out) noexcept {
		if (_handle == nullptr)
			return false;

		struct dirent * ent = readdir(_handle);

		if (ent == nullptr)
			return false;

		out.name = ent->d_name;
		if (out.name == "." || out.name == ".." || out.name.empty())
			return getNextFile(out);

		out.fullPath = _path + out.name;

		struct stat sb;
		if (lstat(out.fullPath.c_str(), &sb) == -1)
			return false;

		out.isDirectory = S_ISDIR(sb.st_mode);
		out.isLink = S_ISLNK(sb.st_mode);

		return true;
	}
}

#endif