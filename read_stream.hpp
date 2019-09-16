#pragma once

#include <sstream>

namespace putils {
    template<typename Stream>
    std::string read_stream(Stream && istream) {
        std::ostringstream ret;

        while (istream && istream.peek() >= 0)
            ret << (char) istream.get();

        return ret.str();
    }
}