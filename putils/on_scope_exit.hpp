#pragma once

// meta
#include "putils/meta/concepts/invocable.hpp"

namespace putils {
	template<putils::invocable<void()> Func>
	struct on_scope_exit {
		Func f;

		~on_scope_exit() noexcept {
			f();
		}
	};
}