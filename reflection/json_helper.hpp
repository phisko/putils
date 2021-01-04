#pragma once

#include "json.hpp"
#include "magic_enum.hpp"
#include "vector.hpp"

namespace putils::reflection {
	template<typename T>
	void fromJSON(const putils::json & object, T && out);

	template<typename T>
	putils::json toJSON(const T & obj);
}

#include "json_helper.inl"