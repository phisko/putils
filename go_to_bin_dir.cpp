#include "go_to_bin_dir.hpp"

#ifdef __unix__
# include <unistd.h>
# include <sys/stat.h>
# include <assert.h>
#elif defined(_WIN32)
# include <windows.h>
# include <processenv.h>
#endif

namespace putils {
	void goToBinDir(std::string currentPath) noexcept {
#ifdef __unix__
        struct stat sb;
        if (lstat(currentPath.c_str(), &sb) == -1)
            assert(false);

        // If av[0] was a symbolic link, dereference it
        if (S_ISLNK(sb.st_mode)) {
            char buf[1024];
            auto ret = readlink(currentPath.c_str(), buf, 1023);
            if (ret == -1)
                assert(false);
            buf[ret] = 0;
            currentPath = buf;
        }
#endif

        size_t last = currentPath.find_last_of("/\\");
        currentPath = currentPath.substr(0, last);
#ifdef __unix__
        chdir(currentPath.c_str());
#elif defined(_WIN32)
		SetCurrentDirectory(currentPath.c_str());
#endif
    }
}