#pragma once

#include <functional>
#include <type_traits>

namespace putils {
    namespace {
        template<typename F, class Tuple, std::size_t ...Is>
        decltype(auto) apply(F && f, Tuple && tuple, std::index_sequence<Is...>) noexcept {
            return f(std::get<Is>(tuple)...);
        }
    }

    template<typename F, typename ...Args>
    std::result_of_t<F(Args...)> apply(const std::tuple<Args...> & tuple, F && f) noexcept {
        return apply(std::forward<F>(f), tuple, std::index_sequence_for<Args...>());
    }
}
