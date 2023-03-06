#pragma once

// fmt
#include <fmt/core.h>
#include <fmt/ranges.h>

// reflection
#include "putils/fmt/formattable.hpp"
#include "putils/reflection.hpp"

namespace fmt {
	namespace putils_detail {
		template<typename T, typename Char>
		using enable_if_not_range = std::enable_if_t<
			std::bool_constant<
				range_format_kind<T, Char>::value == range_format::disabled
			>::value
		>;
	}

	template<putils::reflection::reflectible T, typename Char>
	struct formatter<T, Char, putils_detail::enable_if_not_range<T, Char>> {
		constexpr auto parse(format_parse_context & ctx) -> decltype(ctx.begin()) {
			return ctx.begin();
		}

		template<typename FormatContext>
		auto format(const T & obj, FormatContext & ctx) -> decltype(ctx.out()) {
			*ctx.out()++ = '{';
			putils::reflection::for_each_attribute(obj, [&](const auto & attr) {
				format_attribute(attr, ctx);
			});
			*ctx.out()++ = '}';
			return ctx.out();
		}

		bool first_attribute = true;

		template<typename Attr, typename Context>
		void format_attribute(const Attr & attr, Context & ctx) noexcept {
			if constexpr (putils::formattable<putils_typeof(attr.member)>) {
				if (!first_attribute)
					fmt::format_to(ctx.out(), ", ");
				first_attribute = false;

				fmt::format_to(ctx.out(), "{}: {}", attr.name, attr.member);
			}
		}
	};
}