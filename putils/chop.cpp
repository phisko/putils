#include "chop.hpp"

// putils
#include "putils/profiling.hpp"

namespace putils {
    std::string_view chop(std::string_view str) noexcept {
		PUTILS_PROFILING_SCOPE;

        const auto space = " \t\n\r";

        auto first_non_space = str.find_first_not_of(space);
        if (first_non_space == std::string_view::npos)
            first_non_space = 0;

        const auto last_non_space = str.find_last_not_of(space) + 1;

        auto ret = str.substr(first_non_space, last_non_space - first_non_space);
        while (!ret.empty() && ((ret.front() == '"' && ret.back() == '"') || (ret.front() == '\'' && ret.back() == '\'')))
            ret = ret.substr(1, ret.size() - 2);
        return ret;
    }
}