#pragma once

// putils
#include "type_attributes.hpp"

namespace putils::reflection::runtime {
	template<typename T>
	const type_attributes & get_attributes() noexcept;

	PUTILS_CORE_EXPORT const attribute_info * find_attribute(const type_attributes & attributes, std::string_view path, std::string_view separator = ".") noexcept;

	template<typename T>
	const attribute_info * find_attribute(std::string_view path, std::string_view separator = ".") noexcept;
}

#include "runtime_helper.inl"
