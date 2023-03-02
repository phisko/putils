#pragma once

// fmt
#include <fmt/core.h>

// magic_enum
#include <magic_enum.hpp>

// meta
#include "putils/meta/concepts/enumeration.hpp"

template<putils::enumeration T>
struct fmt::formatter<T> {
	constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(T obj, FormatContext& ctx) const -> decltype(ctx.out()) {
		return fmt::format_to(ctx.out(), "{}", magic_enum::enum_name(obj));
	}
};