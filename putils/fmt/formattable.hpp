#pragma once

// fmt
#include <fmt/core.h>

namespace putils {
	template<typename T, typename Char = char>
	concept formattable = requires(fmt::formatter<T, Char> formatter, fmt::basic_format_parse_context<Char> ctx) {
		  formatter.parse(ctx);
	};
}