#include "concatenate.hpp"

// meta
#include "putils/meta/fwd.hpp"

// putils
#include "profiling.hpp"

namespace putils {
	namespace detail {
		template<typename... Args>
		std::string concatenate_impl(std::stringstream & s, Args &&... args) noexcept;

		template<typename T, typename... Args>
		std::string concatenate_impl(std::stringstream & s, T && first, Args &&... args) noexcept {
			s << FWD(first);
			return concatenate_impl(s, FWD(args)...);
		};

		template<>
		inline std::string concatenate_impl(std::stringstream & s) noexcept {
			return s.str();
		}
	}

	template<putils::streamable<std::stringstream>... Args>
	std::string concatenate(Args &&... args) noexcept {
		PUTILS_PROFILING_SCOPE;
		std::stringstream s;
		return detail::concatenate_impl(s, FWD(args)...);
	}
}