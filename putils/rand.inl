#include "rand.hpp"

// stl
#include <random>

// putils
#include "putils/profiling.hpp"

namespace putils {
	template<typename T>
	T rand(T start, T end) noexcept {
		PUTILS_PROFILING_SCOPE;

		if constexpr (std::is_floating_point<T>()) {
			static std::random_device r;
			static std::default_random_engine e(r());
			std::uniform_real_distribution<T> d(start, end);
			return d(e);
		}
		else {
			static std::random_device r;
			static std::default_random_engine e(r());
			static std::uniform_int_distribution<int> d;
			return start + d(e) % (end - start);
		}
	}
}