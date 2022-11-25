#include "regex.hpp"

// stl
#include "putils/profiling.hpp"

namespace putils::regex {
    bool operator==(const char * str, const std::regex & reg) noexcept {
		PUTILS_PROFILING_SCOPE;
        return std::regex_match(str, reg);
    }

    std::cmatch operator==(const char * str, const std::pair<std::regex, bool> & reg) noexcept {
		PUTILS_PROFILING_SCOPE;
        std::cmatch m;
        std::regex_match(str, m, reg.first);
        return m;
    }

    std::pair<std::regex, bool> operator ""_m(const char * str, size_t) noexcept {
		PUTILS_PROFILING_SCOPE;
        return std::make_pair(std::regex(str), true);
    }

    std::regex operator ""_r(const char * str, size_t) noexcept {
		PUTILS_PROFILING_SCOPE;
        return std::regex(str);
    }
}
