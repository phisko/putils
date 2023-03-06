#pragma once

// stl
#include <optional>

// scn
#include <scn/scn.h>

namespace scn {
	template<typename T>
	struct scanner<std::optional<T>> : scn::empty_parser {
		template <typename Context>
		error scan(std::optional<T> & val, Context & ctx) {
			std::string str;
			const auto result = scn::scan_usertype(ctx, "{}", str);
			if (!result)
				return result;

			if (str == "null") {
				val = std::nullopt;
				return result;
			}

			T obj;
			const auto obj_result = scn::scan(str, "({})", obj);
			if (obj_result)
				val = std::move(obj);
			return obj_result.error();
		}
	};
}