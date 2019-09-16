#pragma once

#include <sstream>
#include "read_stream.hpp"
#include "fwd.hpp"

namespace putils {
    template<typename Obj>
    std::string toString(Obj && obj) {
        std::stringstream s;
        s << FWD(obj);
        return s.str();
    }

    inline std::string toString(bool v) {
        std::stringstream s;
        s << std::boolalpha << v;
        return s.str();
    }

    inline std::string toString(std::istream & s) {
        return read_stream(s);
    }

    template<typename Obj>
    void parse(Obj & obj, const char * str) {
        std::stringstream s(FWD(str));
        s >> obj;
    };

    template<>
    inline void parse(bool & obj, const char * str) {
		obj = strcmp(str, "true") == 0;
    };

	template<>
	inline void parse(std::string & obj, const char * str) {
		obj = str;
	}

    template<typename Obj>
    Obj parse(const char * str) {
        std::stringstream s(FWD(str));

        Obj ret;
        s >> ret;
        return ret;
    };

    template<>
    inline bool parse(const char * str) { return strcmp(str, "true") == 0; }

    template<>
    inline std::string parse(const char * str) { return str; }
}