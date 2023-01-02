#include "erase.hpp"

// stl
#include <algorithm>

// meta
#include "putils/meta/fwd.hpp"

// putils
#include "profiling.hpp"

namespace putils {
	template<typename Container, typename Val>
	void erase(Container & container, Val && val) noexcept {
		PUTILS_PROFILING_SCOPE;

		const auto it = std::find(container.begin(), container.end(), FWD(val));
		if (it == container.end())
			return;
		container.erase(it);
	}
}