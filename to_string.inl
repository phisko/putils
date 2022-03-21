#include "to_string.hpp"
#include <sstream>
#include "fwd.hpp"
#include "read_stream.hpp"

namespace putils {
    template<typename Obj, typename EnableIf>
    std::string toString(Obj && obj) noexcept {
        std::stringstream s;
        s << FWD(obj);
        return s.str();
    }

    template<typename Obj>
    void parse(Obj & obj, std::string_view str) noexcept {
        std::stringstream s;
        s << str;
        s >> obj;
    };

    template<typename Obj>
    Obj parse(std::string_view str) noexcept {
        std::stringstream s;
        s << str;

        Obj ret;
        s >> ret;
        return ret;
    };
}