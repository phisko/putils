#include "concat.hpp"

namespace putils {
    namespace detail {
        template<typename ... Args>
        std::string concat_impl(std::stringstream & s, Args && ... args) noexcept;

        template<typename T, typename ... Args>
        std::string concat_impl(std::stringstream & s, T && first, Args && ... args)  noexcept{
            s << FWD(first);
            return concat_impl(s, FWD(args)...);
        };

        template<>
        inline std::string concat_impl(std::stringstream & s) noexcept {
            return s.str();
        }
    }

    template<typename ...Args>
    std::string concat(Args && ...args) noexcept {
        std::stringstream s;
        return detail::concat_impl(s, FWD(args)...);
    }
}