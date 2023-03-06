#pragma once

// stl
#include <string_view>

// scn
#include <scn/scn.h>

namespace scn {
	namespace putils_detail {
		template<typename T>
		concept string_like = requires(T obj) {
			std::string_view(obj);
		};

		template<typename T, typename Context>
		void putback_end_if_string(T & val, char end, Context & ctx) {
			if constexpr (string_like<T>)
				if (val.ends_with(end)) {
					val = val.substr(0, val.size() - 1);
					scn::putback_n(ctx.range(), 1);
				}
		}

		template<typename T, typename Context>
		error scan_element(T & val, Context & ctx, char closing_bracket) {
			const auto result = scn::scan_usertype(ctx, "{}", val);
			if (!result)
				return result;
			putback_end_if_string(val, ',', ctx);
			putback_end_if_string(val, closing_bracket, ctx);
			scn::skip_range_whitespace(ctx, false);
			return result;
		}
	}
}