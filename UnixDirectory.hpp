#pragma once

#ifdef __unix__

#include <sys/stat.h>
#include <dirent.h>
#include "ADirectory.hpp"
#include "concat.hpp"

namespace putils {
    class UnixDirectory final : public ADirectory {
    public:
        UnixDirectory(const std::string & path)
                :
                _handle(opendir(path.c_str())),
                _path(path), {
			assert(handle != nullptr);
            if (_path[_path.length() - 1] != '/')
                _path = _path + "/";
        }

        virtual ~UnixDirectory() noexcept {
            if (_handle != nullptr)
                closedir(_handle);
        }

    public:
        bool getNextFile(File & out) noexcept override {
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

    private:
        DIR * _handle;
        std::string _path;
    };
}

#endif