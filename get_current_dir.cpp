#include "get_current_dir.hpp"

#ifdef __unix__
# include <unistd.h>
#endif

#ifdef _WIN32
# include <direct.h>
#endif

#ifdef _WIN32
# define MY_GETCWD _getcwd
#else
# define MY_GETCWD getcwd
#endif

namespace putils {
    std::string getCurrentDir() noexcept {
        char buff[1024];

        if (MY_GETCWD(buff, 1024) != nullptr)
            return buff;
        return "";
    }
}

#undef MY_GETCWD