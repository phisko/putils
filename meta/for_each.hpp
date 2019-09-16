#pragma once

#include "type.hpp"

namespace pmeta {
    // Implementation details
    namespace detail {
        template<typename F, typename Tuple, size_t ...Is>
        void tuple_for_each(F && f, Tuple && tuple, std::index_sequence<Is...>) {
            (f(std::get<Is>(tuple)), ...);
        }
    }

    // For each `e` element in `tuple`, call `f(e)`
    // For instance:
    //      std::tuple<int, std::string, int> t(42, "test", 1);
    //      pmeta::tuple_for_each(t, [](auto &attr) { std::cout << attr << std::endl; });
    // Will print
    //      42
    //      test
    //      1
    //

    template<typename F, typename ...Args>
    void tuple_for_each(std::tuple<Args...> & tuple, F && f) {
        detail::tuple_for_each(std::forward<F>(f), tuple, std::index_sequence_for<Args...>());
    }

    template<typename F, typename ...Args>
    void tuple_for_each(const std::tuple<Args...> & tuple, F && f) {
        detail::tuple_for_each(std::forward<F>(f), tuple, std::index_sequence_for<Args...>());
    }

#define pmeta_comma ,
#define pmeta_for_each(_TYPES_, _FUNC_) \
	pmeta::tuple_for_each(std::tuple<pmeta::type<_TYPES_>...>(), _FUNC_)
}
