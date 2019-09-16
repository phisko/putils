#pragma once

#include <type_traits>

// Use this when MemberType alias doesn't work
#define PUTILS_MEMBER_TYPE(T, Member) std::remove_reference_t<decltype(std::declval<T>().*(std::declval<Member>()))>

namespace putils {
	template<typename T, typename Member>
	using MemberType = std::remove_reference_t<decltype(std::declval<T>().*(std::declval<Member>()))>;

	template<typename T, typename U>
	std::ptrdiff_t member_offset(U T::* member) {
		return reinterpret_cast<std::ptrdiff_t>(
			&(reinterpret_cast<T const volatile*>(NULL)->*member)
		);
	}

	namespace detail {
		template<typename T>
		struct member_traits;

		template<typename R, typename T, typename ... Args>
		struct member_traits<
			R (T::*)(Args...)
		> { using return_type = R; };

		template<typename R, typename T, typename ... Args>
		struct member_traits<
			R (T::*)(Args...) noexcept
		> { using return_type = R; };

		template<typename R, typename T, typename ... Args>
		struct member_traits<
			R (T::*)(Args...) const
		> { using return_type = R; };

		template<typename R, typename T, typename ... Args>
		struct member_traits<
			R (T::*)(Args...) const noexcept
		> { using return_type = R; };
	}

	template<typename T>
	using member_function_return_type = typename detail::member_traits<std::decay_t<T>>::return_type;
}