#pragma once

// stl
#include <string>
#include <unordered_map>

// scn
#include <scn/scn.h>

// magic_enum
#include <magic_enum.hpp>

// meta
#include "putils/reflection.hpp"

namespace scn {
	template<putils::reflection::reflectible T>
		requires requires { !std::ranges::range<T>; }
	struct scanner<T> : scn::empty_parser {
		template <typename Context>
		error scan(T & val, Context & ctx) {
			if constexpr (std::is_default_constructible<T>() && std::is_move_assignable<T>())
				val = T{};

			const auto code_unit = scn::read_code_unit(ctx.range());
			if (!code_unit)
				return code_unit.error();
			if (code_unit.value() != '{')
				return error(error::invalid_scanned_value, "First character was not '{'");
			scn::skip_range_whitespace(ctx, false);

			while (true) {
				std::string_view attribute_name;
				const auto attribute_name_result = scn::scan_usertype(ctx, "{}", attribute_name);
				if (!attribute_name_result)
					return attribute_name_result;
				scn::skip_range_whitespace(ctx, false);

				if (!attribute_name.ends_with(':'))
					return error(error::invalid_scanned_value, "Attribute name not followed by ':'");
				attribute_name = attribute_name.substr(0, attribute_name.size() - 1);

				const auto attr_result = putils::reflection::for_each_attribute(val, [&](const auto & attr) -> std::optional<error> {
					if (attr.name != attribute_name)
						return std::nullopt;

					return putils_detail::scan_element(attr.member, ctx, '}');
				});

				if (!attr_result)
					return error(error::invalid_scanned_value, "Unknown attribute specified");

				if (!*attr_result)
					return *attr_result;

				const auto code_unit = scn::read_code_unit(ctx.range());
				if (!code_unit)
					return code_unit.error();
				if (code_unit.value() == '}')
					return code_unit.error();
			}
		}
	};
}
