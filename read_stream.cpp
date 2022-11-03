#include "read_stream.hpp"

// putils
#include "putils_profiling.hpp"

namespace putils {
    std::string read_stream(std::istream & istream) noexcept {
		PUTILS_PROFILING_SCOPE;

        std::ostringstream ret;

        while (istream && istream.peek() >= 0)
            ret << (char) istream.get();

        return ret.str();
    }
}