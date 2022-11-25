#pragma once

// magic_enum
#include <magic_enum.hpp>

// nlohmann
#include <nlohmann/json.hpp>

namespace putils::reflection {
	template<typename T>
	void from_json(const nlohmann::json & object, T && out) noexcept;

	template<typename T>
	nlohmann::json to_json(const T & obj) noexcept;
}

#include "json_helper.inl"