#pragma once

#include "type.hpp"
#include "fwd.hpp"

#define putils_macro_comma ,

namespace putils {
	// For each `T` in `Types` call `f(putils::meta::type<T>)`
	// `T` can then be recovered by using putils_wrapped_type
	// For instance:
	// ```
	//		putils::for_each_type<int, double>([](auto && t) {
	//			using T = putils_wrapped_type(t);
	//			std::cout << typeid(T).name() << std::endl;
	//		});
	// ```
	// Will print something similar to:
	//		int
	//		double

	template<typename ...Types, typename Func>
	constexpr void for_each_type(Func && f) noexcept;

    // For each `e` element in `tuple`, call `f(e)`
    // For instance:
	// ```
    //      std::tuple<int, std::string, int> t(42, "test", 1);
    //      putils::tuple_for_each(t, [](auto &attr) {
	//			std::cout << attr << std::endl;
	//		});
	// ```
    // Will print
    //      42
    //      test
    //      1
    //

    template<typename F, typename ...Args>
	constexpr void tuple_for_each(std::tuple<Args...> & tuple, F && f) noexcept;

    template<typename F, typename ...Args>
	constexpr void tuple_for_each(const std::tuple<Args...> & tuple, F && f) noexcept;

    // Implementation details

	template<typename ...Types, typename Func>
	constexpr void for_each_type(Func && f) noexcept {
		tuple_for_each(std::tuple<putils::meta::type<Types>...>(), FWD(f));
	}

    namespace detail {
        template<typename F, typename Tuple, size_t ...Is>
        constexpr void tuple_for_each(F && f, Tuple && tuple, std::index_sequence<Is...>) noexcept {
            (f(std::get<Is>(tuple)), ...);
        }
    }

    template<typename F, typename ...Args>
    constexpr void tuple_for_each(std::tuple<Args...> & tuple, F && f) noexcept {
        detail::tuple_for_each(std::forward<F>(f), tuple, std::index_sequence_for<Args...>());
    }

    template<typename F, typename ...Args>
    constexpr void tuple_for_each(const std::tuple<Args...> & tuple, F && f) noexcept {
        detail::tuple_for_each(std::forward<F>(f), tuple, std::index_sequence_for<Args...>());
    }
}
