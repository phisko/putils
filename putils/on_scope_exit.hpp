#pragma once

namespace putils {
	template<typename Func>
	struct on_scope_exit {
		Func f;

		~on_scope_exit() noexcept {
			f();
		}
	};
}