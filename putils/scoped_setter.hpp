#pragma once

// stl
#include <cassert>

// meta
#include "putils/meta/fwd.hpp"

namespace putils {
	template<typename T>
	struct scoped_setter {
		template<typename U>
		scoped_setter(T & dest, U && src) noexcept
			: dest(dest), old_value(std::move(dest)) {
			dest = FWD(src);
		}

		~scoped_setter() {
			dest = std::move(old_value);
		}

	private:
		T & dest;
		T old_value;
	};

	template<typename T, typename U>
	scoped_setter<T> set_for_scope(T & dest, U && src) noexcept {
		return scoped_setter<T>(dest, FWD(src));
	}
}