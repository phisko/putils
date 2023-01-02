#pragma once

// stl
#include <string>
#include <sstream>

// meta
#include "putils/meta/concepts/streamable.hpp"

namespace putils {
	template<putils::streamable<std::stringstream>... Args>
	std::string concatenate(Args &&... args) noexcept;
}

#include "concatenate.inl"
