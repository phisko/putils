#include "command_line_arguments.hpp"

// putils
#include "putils/profiling.hpp"

namespace putils {
	std::vector<std::string_view> to_argument_vector(int ac, const char * const * av) noexcept {
		PUTILS_PROFILING_SCOPE;

		std::vector<std::string_view> ret;
		for (const auto arg : std::span(av, ac))
			ret.emplace_back(arg);
		return ret;
	}
}