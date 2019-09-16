#pragma once

#include <type_traits>
#include <vector>

namespace putils {
    template<typename S, typename T>
    class is_streamable {
        template<typename SS, typename TT>
        static auto test(int)
        -> decltype(std::declval<SS &>() << std::declval<TT>(), std::true_type());

        template<typename, typename>
        static auto test(...) -> std::false_type;

    public:
        static const bool value = decltype(test<S, T>(0))::value;
    };

    template<typename S, typename T>
    class is_unstreamable {
        template<typename SS, typename TT>
        static auto test(int)
        -> decltype(std::declval<SS &>() >> std::declval<TT &>(), std::true_type());

        template<typename, typename>
        static auto test(...) -> std::false_type;

    public:
        static const bool value = decltype(test<S, T>(0))::value;
    };

	namespace detail {
		// To allow ADL with custom begin/end
		using std::begin;
		using std::end;

		template <typename T>
		auto is_iterable_impl(int)
			-> decltype (
				begin(std::declval<T&>()) != end(std::declval<T&>()), // begin/end and operator !=
				void(), // Handle evil operator ,
				++std::declval<decltype(begin(std::declval<T&>()))&>(), // operator ++
				void(*begin(std::declval<T&>())), // operator*
				std::true_type{});

		template <typename T>
		std::false_type is_iterable_impl(...);
	}

	template <typename T>
	using is_iterable = decltype(detail::is_iterable_impl<T>(0));
}

#define putils_member_detector(member)\
	template<typename T, typename = int>\
	struct has_member_##member : std::false_type {};\
\
	template<typename T>\
	struct has_member_##member<T, decltype((void)&T::##member, 0)> : std::true_type {};

namespace putils {
	namespace detail {
		template<typename T>
		struct function_traits;

		template<typename R, typename ... Args>
		struct function_traits<
			R(*)(Args...)
		> { using return_type = R; };

		template<typename R, typename ... Args>
		struct function_traits<
			R(*)(Args...) noexcept
		> { using return_type = R; };
	}

	template<typename T>
	using function_return_type = typename detail::function_traits<std::decay_t<T>>::return_type;
}

namespace putils {
	template<typename>
	struct is_std_vector;

	template<typename T>
	struct is_std_vector<std::vector<T>> : std::true_type {};

	template<typename>
	struct is_std_vector : std::false_type {};
}