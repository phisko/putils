#pragma once

#include "json.hpp"
#include "magic_enum.hpp"
#include "vector.hpp"

namespace putils::reflection {
	template<typename T>
	void fromJSON(const putils::json & object, T && out) noexcept;

	template<typename T>
	putils::json toJSON(const T & obj) noexcept;
}

#include "json_helper.inl"