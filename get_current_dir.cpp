#include "get_current_dir.hpp"

#ifdef __unix__
# include <unistd.h>
#endif

#ifdef _WIN32
# include <direct.h>
#endif


namespace putils {
    std::string getCurrentDir() noexcept {
        char buff[1024];

        if (_getcwd(buff, 1024) != nullptr)
            return buff;
        return "";
    }
}