#pragma once

#include <string>
#include "string.hpp"

namespace putils {
    inline std::string chop(std::string_view str) {
        return std::string(str.substr(0, str.find_last_not_of(" \t\n\r") + 1));
    }

	template<size_t N>
	inline putils::string<N> static_chop(std::string_view str) {
        return putils::string<N>(str.substr(0, str.find_last_not_of(" \t\n\r") + 1));
	}
}
