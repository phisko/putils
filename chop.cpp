#include "chop.hpp"

namespace putils {
    std::string chop(std::string_view str) noexcept {
        return std::string(str.substr(0, str.find_last_not_of(" \t\n\r") + 1));
    }
}