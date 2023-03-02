#include "parse.hpp"

// stl
#include <sstream>

// meta
#include "putils/meta/fwd.hpp"

// putils
#include "read_stream.hpp"
#include "profiling.hpp"

namespace putils {
	template<putils::unstreamable<std::stringstream> Obj>
	void parse(Obj & obj, std::string_view str) noexcept {
		PUTILS_PROFILING_SCOPE;

		std::stringstream s;
		s << str;
		s >> obj;
	}

	template<typename E>
		requires std::is_enum_v<E>
	void parse(E & obj, std::string_view str) noexcept {
		PUTILS_PROFILING_SCOPE;

		if constexpr (magic_enum::enum_count<E>() > 0) {
			for (int i = 0; i < magic_enum::enum_count<E>(); ++i) {
				if (magic_enum::enum_names<E>()[i] == str) {
					obj = static_cast<E>(i);
					return;
				}
			}
			assert(false); // Unknown value
		}
		else {
			using underlying_type = std::underlying_type_t<E>;
			underlying_type non_enum_value = underlying_type(obj);
			parse(non_enum_value, str);
			obj = E(non_enum_value);
		}
	}

	template<typename Obj>
	Obj parse(std::string_view str) noexcept {
		Obj ret;
		parse(ret, str);
		return ret;
	}
}