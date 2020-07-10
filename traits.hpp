#pragma once

#include <type_traits>
#include <functional>
#include "function.hpp"

namespace putils {
	namespace detail {
		template<typename S, typename T>
		class is_streamable {
			template<typename SS, typename TT>
			static auto test(int)
				-> decltype(std::declval<SS &>() << std::declval<TT>(), std::true_type());

			template<typename, typename>
			static auto test(...)->std::false_type;

		public:
			using type = decltype(test<S, T>(0));
		};
	}

	template<typename S, typename T>
	using is_streamable = typename detail::is_streamable<S, T>::type;

	namespace detail {
		template<typename S, typename T>
		class is_unstreamable {
			template<typename SS, typename TT>
			static auto test(int)
				-> decltype(std::declval<SS &>() >> std::declval<TT &>(), std::true_type());

			template<typename, typename>
			static auto test(...)->std::false_type;

		public:
			using type = decltype(test<S, T>(0));
		};
	}

	template<typename S, typename T>
	using is_unstreamable = typename detail::is_unstreamable<S, T>::type;

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
	template<typename T, typename = std::void_t<>>\
	struct has_member_##member : std::false_type {};\
\
	template<typename T>\
	struct has_member_##member<T, std::void_t<decltype((void)&T::##member)>> : std::true_type {};

#define putils_nested_type_detector(member)\
	template<typename T, typename = std::void_t<>>\
	struct has_nested_type_##member : std::false_type {};\
\
	template<typename T>\
	struct has_nested_type_##member<T, std::void_t<typename T::##member>> : std::true_type {};


namespace putils {
	namespace detail {
#define SPECIALIZATION(TYPE) \
		template<typename R, typename ... Args> \
		struct function_traits< \
			TYPE \
		> : std::true_type { \
			using signature = R(*)(Args...);\
			using return_type = R; \
			using arguments = std::tuple<Args...>; \
		};

		template<typename T>
		struct function_traits : std::false_type {};

		SPECIALIZATION(R(*)(Args...))
		SPECIALIZATION(R(*)(Args...) noexcept)
		SPECIALIZATION(std::function<R(Args...)>)

		template<typename R, size_t Size, typename ... Args>
		struct function_traits<
			putils::function<R(Args...), Size>
		> : std::true_type {
			using signature = R(*)(Args...);
			using return_type = R;
			using arguments = std::tuple<Args...>;
		};
#undef SPECIALIZATION
	}

	template<typename T>
	using is_function = detail::function_traits<std::decay_t<T>>;

	template<typename T>
	using function_signature = typename detail::function_traits<std::decay_t<T>>::signature;

	template<typename T>
	using function_return_type = typename detail::function_traits<std::decay_t<T>>::return_type;

	template<typename T>
	using function_arguments = typename detail::function_traits<std::decay_t<T>>::arguments;
}

namespace putils {
	namespace detail {
#define SPECIALIZATION(TYPE) \
		template<typename T, typename R, typename ... Args> \
		struct member_function_traits< \
			TYPE \
		> : std::true_type { \
			using decay = R(T::*)(Args...); /* Remove const noexcept */ \
			using signature = R(Args...); \
			using return_type = R; \
			using arguments = std::tuple<Args...>; \
		};

		template<typename T>
		struct member_function_traits : std::false_type {};

		SPECIALIZATION(R(T::*)(Args...))
		SPECIALIZATION(R(T::*)(Args...) const)
		SPECIALIZATION(R(T::*)(Args...) noexcept)
		SPECIALIZATION(R(T::*)(Args...) const noexcept)
#undef SPECIALIZATION
	}

	template<typename T>
	using is_member_function_ptr = detail::member_function_traits<std::decay_t<T>>;

	template<typename T>
	using member_function_decay = typename detail::member_function_traits<std::decay_t<T>>::decay;

	template<typename T>
	using member_function_signature = typename detail::member_function_traits<std::decay_t<T>>::signature;

	template<typename T>
	using member_function_return_type = typename detail::member_function_traits<std::decay_t<T>>::return_type;

	template<typename T>
	using member_function_arguments = typename detail::member_function_traits<std::decay_t<T>>::arguments;
}

namespace putils {
	template<typename T, template<typename...> class Template>
	struct is_specialization : std::false_type {};

	template<template<typename...> class Template, typename ... Args>
	struct is_specialization<Template<Args...>, Template> : std::true_type {};
}