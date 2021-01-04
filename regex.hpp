#pragma once

#include <regex>

namespace putils::regex {
    bool operator==(const char *& str, const std::regex & reg) noexcept;
    std::cmatch operator==(const char * str, const std::pair<std::regex, bool> & reg) noexcept;
    std::pair<std::regex, bool> operator ""_m(const char * str, size_t) noexcept;
    std::regex operator ""_r(const char * str, size_t) noexcept;
}
