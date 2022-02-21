#include "to_string.hpp"
#include <cstring>

namespace putils {
    std::string toString(bool v) noexcept {
        std::stringstream s;
        s << std::boolalpha << v;
        return s.str();
    }

    std::string toString(std::istream & s) noexcept {
        return read_stream(s);
    }

    template<>
    void parse(bool & obj, const char * str) noexcept {
		obj = strcmp(str, "true") == 0;
    };

	template<>
	void parse(std::string & obj, const char * str) noexcept {
		obj = str;
	}

    template<>
    bool parse(const char * str) noexcept {
        return strcmp(str, "true") == 0; 
    }

    template<>
    std::string parse(const char * str) noexcept {
        return str; 
    }
}