#pragma once

#include "meta/fwd.hpp"

namespace putils {
	template<typename Func>
	struct OnScopeExit {
		Func f;

		~OnScopeExit() noexcept {
			f();
		}
	};

	template<typename Func>
	OnScopeExit<std::decay_t<Func>> onScopeExit(Func && f) noexcept {
		return { FWD(f) };
	}
}