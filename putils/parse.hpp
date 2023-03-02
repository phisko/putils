#pragma once

// stl
#include <sstream>
#include <string>
#include <string_view>

// magic_enum
#include <magic_enum.hpp>

// meta
#include "putils/meta/concepts/streamable.hpp"
#include "putils/meta/concepts/unstreamable.hpp"

namespace putils {
	// to_string
	template<putils::streamable<std::stringstream> Obj>
	std::string to_string(Obj && obj) noexcept;

	PUTILS_CORE_EXPORT std::string to_string(bool v) noexcept;
	PUTILS_CORE_EXPORT std::string to_string(std::istream & s) noexcept;

	// parse to reference
	template<putils::unstreamable<std::stringstream> Obj>
	void parse(Obj & obj, std::string_view str) noexcept;

	template<typename E>
		requires std::is_enum_v<E>
	void parse(E & obj, std::string_view str) noexcept;

	template<>
	PUTILS_CORE_EXPORT void parse(bool & obj, std::string_view str) noexcept;

	template<>
	PUTILS_CORE_EXPORT void parse(std::string & obj, std::string_view str) noexcept;

	// parse to return value
	template<typename Obj>
	Obj parse(std::string_view str) noexcept;

	template<>
	PUTILS_CORE_EXPORT bool parse(std::string_view str) noexcept;

	template<>
	PUTILS_CORE_EXPORT std::string parse(std::string_view str) noexcept;
}

#include "to_string.inl"