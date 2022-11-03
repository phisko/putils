#include "get_current_dir.hpp"

// putils
#include "putils_profiling.hpp"

// unix
#ifdef __unix__
# include <unistd.h>
#endif

// windows
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
		PUTILS_PROFILING_SCOPE;

        char buff[1024];

        if (MY_GETCWD(buff, 1024) != nullptr)
            return buff;
        return "";
    }
}

#undef MY_GETCWD