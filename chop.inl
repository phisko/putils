#include "chop.hpp"

namespace putils {
    template<size_t N>
    putils::string<N> chop(std::string_view str) noexcept {
        return str.substr(0, str.find_last_not_of(" \t\n\r") + 1);
    }
}