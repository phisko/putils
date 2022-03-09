#include "regex.hpp"

namespace putils::regex {
    bool operator==(const char * str, const std::regex & reg) noexcept {
        return std::regex_match(str, reg);
    }

    std::cmatch operator==(const char * str, const std::pair<std::regex, bool> & reg) noexcept {
        std::cmatch m;
        std::regex_match(str, m, reg.first);
        return m;
    }

    std::pair<std::regex, bool> operator ""_m(const char * str, size_t) noexcept {
        return std::make_pair(std::regex(str), true);
    }

    std::regex operator ""_r(const char * str, size_t) noexcept {
        return std::regex(str);
    }
}
