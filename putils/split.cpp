#include "split.hpp"

// stl
#include <sstream>

// putils
#include "putils/profiling.hpp"

namespace putils {
	std::vector<std::string> split(const char * str, char delim) noexcept {
		PUTILS_PROFILING_SCOPE;

		std::vector<std::string> ret;

		std::string tmp;
		std::stringstream stream(str);
		while (std::getline(stream, tmp, delim))
			ret.push_back(tmp);

		return ret;
	}
}
