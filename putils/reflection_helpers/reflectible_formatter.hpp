#pragma once

// fmt
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

// reflection
#include "putils/reflection.hpp"

#include "enum_formatter.hpp"

template<putils::reflection::with_attributes T>
struct fmt::formatter<T> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const T & obj, FormatContext& ctx) const -> decltype(ctx.out()) {
		fmt::format_to(ctx.out(), "(");

		bool first = true;
		putils::reflection::for_each_attribute(obj, [&](const auto & attr) noexcept {
			if (!first)
				fmt::format_to(ctx.out(), ", ");
			first = false;

			fmt::format_to(ctx.out(), "{}: {}", attr.name, attr.member);
		});

		fmt::format_to(ctx.out(), ")");
		return ctx.out();
	}
};
