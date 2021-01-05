#include "chop.hpp"

namespace putils {
    std::string_view chop(std::string_view str) noexcept {
        return str.substr(0, str.find_last_not_of(" \t\n\r") + 1);
    }
}