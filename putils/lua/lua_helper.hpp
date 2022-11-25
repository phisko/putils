#pragma once

// sol
#include <sol/sol.hpp>

namespace putils::lua {
	template<typename T>
	void register_type(sol::state & state);
}

#include "lua_helper.inl"