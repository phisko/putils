#include "to_string.hpp"

// stl
#include <sstream>

// putils
#include "read_stream.hpp"
#include "putils_profiling.hpp"

namespace putils {
    std::string toString(bool v) noexcept {
		PUTILS_PROFILING_SCOPE;

        std::stringstream s;
        s << std::boolalpha << v;
        return s.str();
    }

    std::string toString(std::istream & s) noexcept {
		PUTILS_PROFILING_SCOPE;

        return read_stream(s);
    }

    template<>
    void parse(bool & obj, std::string_view str) noexcept {
		obj = str == "true";
    };

	template<>
	void parse(std::string & obj, std::string_view str) noexcept {
		obj = str;
	}

    template<>
    bool parse(std::string_view str) noexcept {
        return str == "true";
    }

    template<>
    std::string parse(std::string_view str) noexcept {
        return std::string(str);
    }
}