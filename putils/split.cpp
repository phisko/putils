#include "split.hpp"

// scn
#include <scn/scn.h>

// putils
#include "putils/profiling.hpp"

namespace putils {
	std::vector<std::string_view> split(std::string_view str, char delim) noexcept {
		PUTILS_PROFILING_SCOPE;

		std::vector<std::string_view> ret;

		std::string_view tmp;
		auto result = scn::getline(str, tmp, delim);
		while (result) {
			ret.push_back(tmp);
			result = scn::getline(result.range(), tmp, delim);
		}

		return ret;
	}
}
