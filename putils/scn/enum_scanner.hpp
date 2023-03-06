#pragma once

// scn
#include <scn/scn.h>

// magic_enum
#include <magic_enum.hpp>

// meta
#include "putils/meta/concepts/enumeration.hpp"

namespace scn {
	template<putils::enumeration T>
	struct scanner<T> : scn::empty_parser {
		template <typename Context>
		error scan(T & val, Context & ctx) {
			if constexpr (magic_enum::enum_count<T>() == 0) {
				std::underlying_type_t<T> non_enum_value;
				const auto result = scn::scan_usertype(ctx, "{}", non_enum_value);
				val = T(non_enum_value);
				return result;
			}
			else {
				std::string str;
				const auto result = scn::scan_usertype(ctx, "{}", str);

				if (result) {
					for (int i = 0; i < magic_enum::enum_count<T>(); ++i) {
						if (magic_enum::enum_names<T>()[i] == str) {
							val = T(i);
							return result;
						}
					}
				}

				return result;
			}
		}
	};
}