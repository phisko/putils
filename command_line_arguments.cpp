#include "command_line_arguments.hpp"

namespace putils {
    std::vector<std::string_view> toArgumentVector(int ac, const char * const * av) noexcept {
        std::vector<std::string_view> ret;
        for (int i = 0; i < ac; ++i)
            ret.emplace_back(av[i]);
        return ret;
    }
}