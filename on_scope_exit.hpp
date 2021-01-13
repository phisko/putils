#pragma once

#include "fwd.hpp"

namespace putils {
	template<typename Func>
	struct OnScopeExit {
		Func f;

		~OnScopeExit() noexcept {
			f();
		}
	};

	template<typename Func>
	OnScopeExit<Func> onScopeExit(Func && f) noexcept {
		return { FWD(f) };
	}
}