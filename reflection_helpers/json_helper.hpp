#pragma once

// magic_enum
#include <magic_enum.hpp>

// nlohmann
#include <nlohmann/json.hpp>

// putils
#include "vector.hpp"

namespace putils::reflection {
	template<typename T>
	void fromJSON(const nlohmann::json & object, T && out) noexcept;

	template<typename T>
	nlohmann::json toJSON(const T & obj) noexcept;
}

#include "json_helper.inl"