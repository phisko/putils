#pragma once

// stl
#include <ranges>
#include <vector>

// scn
#include <scn/scn.h>

// fmt
#include <fmt/ranges.h>

// meta
#include "putils/static_assert.hpp"

#include "scan_element.hpp"

namespace scn {
	template<typename K, typename V>
	struct scanner<std::pair<K, V>> : scn::empty_parser {
		template <typename Context>
		error scan(std::pair<K, V> & val, Context & ctx) {
			// First try with comma, assuming another pair follows
			const auto key_result = scn::scan_usertype(ctx, "{}", val.first);
			if (!key_result)
				return key_result;
			putils_detail::putback_end_if_string(val.first, ':', ctx);

			const auto code_point = scn::read_code_unit(ctx.range());
			if (!code_point)
				return code_point.error();
			if (code_point.value() != ':')
				return error(error::invalid_scanned_value, "Expected 'key: value' format");
			scn::skip_range_whitespace(ctx, false);

			return putils_detail::scan_element(val.second, ctx, '}');
		}
	};

	template<std::ranges::range T>
		requires requires { typename T::value_type; }
	struct scanner<T> : scn::empty_parser {
		template <typename Context>
		error scan(T & val, Context & ctx) {
			val.clear();

			constexpr fmt::range_format format_type = fmt::detail::range_format_kind_<T>::value;
			constexpr auto brackets =
				format_type == fmt::range_format::sequence ? "[]" :
				format_type == fmt::range_format::map ? "{}" :
				format_type == fmt::range_format::set ? "{}" :
				nullptr;
			static_assert(brackets != nullptr);

			// Check for opening bracket
			auto code_unit = scn::read_code_unit(ctx.range());
			if (!code_unit)
				return code_unit.error();
			if (code_unit.value() != brackets[0])
				return error(error::invalid_scanned_value, "Value did not start with opening bracket");
			scn::skip_range_whitespace(ctx, false);

			// Check for immediate closing bracket
			code_unit = scn::read_code_unit(ctx.range(), false);
			if (!code_unit)
				return code_unit.error();
			if (code_unit.value() == brackets[1])
				return code_unit.error();

			using value_type = typename T::value_type;

			error result;
			if constexpr (format_type == fmt::range_format::map) {
				using key_type = typename T::key_type;
				using mapped_type = typename T::mapped_type;
				return scan_through_vector<std::pair<key_type, mapped_type>>(val, ctx, brackets[1]);
			}
			else
				return scan_through_vector<value_type>(val, ctx, brackets[1]);
		}

		template<typename ElementType, typename Context>
		error scan_through_vector(T & val, Context & ctx, char closing_bracket) {
			while (true) {
				scn::skip_range_whitespace(ctx, false);

				ElementType element;
				const auto element_result = putils_detail::scan_element(element, ctx, closing_bracket);
				if (!element_result)
					return element_result;

				if constexpr (requires { val.emplace_back(std::move(element)); })
					val.emplace_back(std::move(element));
				else
					val.emplace(std::move(element));

				scn::skip_range_whitespace(ctx, false);
				const auto code_unit = scn::read_code_unit(ctx.range());
				if (!code_unit)
					return code_unit.error();
				if (code_unit.value() == closing_bracket)
					return code_unit.error();
			}
		}
	};
}