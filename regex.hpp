#pragma once

#include <putils_core_export.h>

// stl
#include <regex>

namespace putils::regex {
    PUTILS_CORE_EXPORT bool operator==(const char * str, const std::regex & reg) noexcept;
    PUTILS_CORE_EXPORT std::cmatch operator==(const char * str, const std::pair<std::regex, bool> & reg) noexcept;
    PUTILS_CORE_EXPORT std::pair<std::regex, bool> operator ""_m(const char * str, size_t) noexcept;
    PUTILS_CORE_EXPORT std::regex operator ""_r(const char * str, size_t) noexcept;
}
