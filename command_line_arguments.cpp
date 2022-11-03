#include "command_line_arguments.hpp"

// putils
#include "putils_profiling.hpp"

namespace putils {
    std::vector<std::string_view> toArgumentVector(int ac, const char * const * av) noexcept {
		PUTILS_PROFILING_SCOPE;

        std::vector<std::string_view> ret;
        for (int i = 0; i < ac; ++i)
            ret.emplace_back(av[i]);
        return ret;
    }
}