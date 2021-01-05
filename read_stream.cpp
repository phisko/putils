#include "read_stream.hpp"

namespace putils {
    std::string read_stream(std::istream & istream) noexcept {
        std::ostringstream ret;

        while (istream && istream.peek() >= 0)
            ret << (char) istream.get();

        return ret.str();
    }
}