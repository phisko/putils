#include "chop.hpp"

namespace putils {
    std::string_view chop(std::string_view str) noexcept {
        const auto space = " \t\n\r";
        const auto firstNonSpace = str.find_first_not_of(space);
        const auto lastNonSpace = str.find_last_not_of(space) + 1;
        auto ret = str.substr(firstNonSpace != std::string::npos ? firstNonSpace : 0, lastNonSpace);
        while (!ret.empty() && ((ret.front() == '"' && ret.back() == '"') || (ret.front() == '\'' && ret.back() == '\'')))
            ret = ret.substr(1, ret.size() - 2);
        return ret;
    }
}