#pragma once

#include "type.hpp"

namespace putils {
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
    //      putils::tuple_for_each(t, [](auto &attr) { std::cout << attr << std::endl; });
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

#define putils_macro_comma ,
#define putils_for_each_type(_TYPES_, _FUNC_) \
	putils::tuple_for_each(std::tuple<putils::meta::type<_TYPES_>...>(), _FUNC_)
}
