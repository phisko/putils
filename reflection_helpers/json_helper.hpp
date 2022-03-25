#pragma once

#include <magic_enum.hpp>
#include <nlohmann/json.hpp>
#include "vector.hpp"

namespace putils::reflection {
	template<typename T>
	void fromJSON(const nlohmann::json & object, T && out) noexcept;

	template<typename T>
	nlohmann::json toJSON(const T & obj) noexcept;
}

#include "json_helper.inl"