#pragma once

// stl
#include <optional>

// fmt
#include <fmt/core.h>

namespace fmt {
	template<typename T>
	struct formatter<std::optional<T>> {
		constexpr auto parse(format_parse_context & ctx) -> decltype(ctx.begin()) {
			return ctx.begin();
		}

		template<typename FormatContext>
		auto format(std::optional<T> obj, FormatContext & ctx) const -> decltype(ctx.out()) {
			if (!obj)
				return fmt::format_to(ctx.out(), "null");
			else
				return fmt::format_to(ctx.out(), "({})", *obj);
		}
	};
}
