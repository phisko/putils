#pragma once

#include "Attributes.hpp"

namespace putils::reflection::runtime {
    template<typename T>
    const Attributes & getAttributes() noexcept;
    
	const AttributeInfo * findAttribute(const Attributes & attributes, std::string_view path, std::string_view separator = ".") noexcept;

	template<typename T>
	const AttributeInfo * findAttribute(std::string_view path, std::string_view separator = ".") noexcept;
}

#include "runtime_helper.inl"
