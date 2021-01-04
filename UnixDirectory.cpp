#ifdef __unix__

#include <sys/stat.h>
#include "UnixDirectory.hpp"

namespace putils {
	UnixDirectory::UnixDirectory(const std::string & path) noexcept
		:
		_handle(opendir(path.c_str())),
		_path(path)
	{
		assert(handle != nullptr);
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

		out.file = ent->d_name;
		if (out.file == "." || out.file == ".." || out.file.empty())
			return getNextFile();

		out.fullPath = _path + file;

		struct stat sb;
		if (lstat(out.fullPath.c_str(), &sb) == -1)
			return false;

		out.isDirectory = S_ISDIR(sb.st_mode);
		out.isLink = S_ISLNK(sb.st_mode);

		return true;
	}
}

#endif