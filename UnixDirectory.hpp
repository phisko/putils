#pragma once

#ifdef __unix__

#include <dirent.h>
#include "ADirectory.hpp"

namespace putils {
    class UnixDirectory final : public ADirectory {
    public:
        UnixDirectory(const std::string & path) noexcept;
        ~UnixDirectory() noexcept override;

    public:
        bool getNextFile(File & out) noexcept override;

    private:
        DIR * _handle;
        putils::string<PUTILS_DIRECTORY_MAX_PATH> _path;
    };
}

#endif