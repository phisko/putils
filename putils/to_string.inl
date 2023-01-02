#include "to_string.hpp"

// stl
#include <sstream>

// magic_enum
#include <magic_enum.hpp>

// meta
#include "putils/meta/fwd.hpp"

// putils
#include "read_stream.hpp"
#include "profiling.hpp"

namespace putils {
	template<putils::streamable<std::stringstream> Obj>
	std::string to_string(Obj && obj) noexcept {
		PUTILS_PROFILING_SCOPE;

		if constexpr (std::is_enum<std::decay_t<Obj>>())
			return std::string(magic_enum::enum_name(obj));
		else {
			std::stringstream s;
			s << FWD(obj);
			return s.str();
		}
	}

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

		for (int i = 0; i < magic_enum::enum_count<E>(); ++i) {
			if (magic_enum::enum_names<E>()[i] == str) {
				obj = static_cast<E>(i);
				return;
			}
		}
		assert(false); // Unknown value
	}

	template<typename Obj>
	Obj parse(std::string_view str) noexcept {
		Obj ret;
		parse(ret, str);
		return ret;
	}
}