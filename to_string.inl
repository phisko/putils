#include "to_string.hpp"

// stl
#include <sstream>

// magic_enum
#include <magic_enum.hpp>

// meta
#include "meta/fwd.hpp"

// putils
#include "read_stream.hpp"
#include "putils_profiling.hpp"

namespace putils {
    template<typename Obj, typename EnableIf>
    std::string toString(Obj && obj) noexcept {
		PUTILS_PROFILING_SCOPE;

        if constexpr (std::is_enum<std::decay_t<Obj>>())
            return std::string(magic_enum::enum_name(obj));
        else {
            std::stringstream s;
            s << FWD(obj);
            return s.str();
        }
    }

    template<typename Obj>
    void parse(Obj & obj, std::string_view str) noexcept {
		PUTILS_PROFILING_SCOPE;

        if constexpr (std::is_enum<Obj>()) {
            for (int i = 0; i < magic_enum::enum_count<Obj>(); ++i) {
                if (magic_enum::enum_names<Obj>()[i] == str) {
                    obj = static_cast<Obj>(i);
                    return;
                }
            }
            assert(false); // Unknown value
        }
        else {
            std::stringstream s;
            s << str;
            s >> obj;
        }
    }

    template<typename Obj>
    Obj parse(std::string_view str) noexcept {
        Obj ret;
        parse(ret, str);
        return ret;
    }
}