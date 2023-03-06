#pragma once

// fmt
#include <fmt/core.h>
#include <fmt/ranges.h>

// reflection
#include "putils/fmt/formattable.hpp"
#include "putils/reflection.hpp"

namespace fmt {
	namespace putils_detail {
		template<typename T, typename Char, bool Pred>
		using enable_if_reflectible = std::enable_if_t<
		    !fmt::is_range<T, Char>::value && Pred
		>;

		template<typename T, typename Char>
		using enable_if_reflectible_string = enable_if_reflectible<
		    T, Char, fmt::detail::is_std_string_like<T>::value
		>;

		template<typename T, typename Char>
		using enable_if_reflectible_not_string = enable_if_reflectible<
			T, Char, !fmt::detail::is_std_string_like<T>::value
		>;
	}

	template<putils::reflection::reflectible T, typename Char>
	struct formatter<T, Char, putils_detail::enable_if_reflectible_string<T, Char>> {
		constexpr auto parse(format_parse_context & ctx) const -> decltype(ctx.begin()) {
			return ctx.begin();
		}

		template<typename FormatContext>
		auto format(const T & obj, FormatContext & ctx) const -> decltype(ctx.out()) {
			return fmt::format_to(ctx.out(), "{}", std::string_view(obj));
		}
	};

	template<putils::reflection::reflectible T, typename Char>
	struct formatter<T, Char, putils_detail::enable_if_reflectible_not_string<T, Char>> {
		constexpr auto parse(format_parse_context & ctx) const -> decltype(ctx.begin()) {
			return ctx.begin();
		}

		template<typename FormatContext>
		auto format(const T & obj, FormatContext & ctx) const -> decltype(ctx.out()) {
			*ctx.out()++ = '{';
			putils::reflection::for_each_attribute(obj, [&](const auto & attr) {
				format_attribute(attr, ctx);
			});
			*ctx.out()++ = '}';
			return ctx.out();
		}

		mutable bool first_attribute = true;

		template<typename Attr, typename Context>
		void format_attribute(const Attr & attr, Context & ctx) const noexcept {
			if constexpr (putils::formattable<putils_typeof(attr.member)>) {
				if (!first_attribute)
					fmt::format_to(ctx.out(), ", ");
				first_attribute = false;

				fmt::format_to(ctx.out(), "{}: {}", attr.name, attr.member);
			}
		}
	};
}