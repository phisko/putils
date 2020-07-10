#pragma once

#include <type_traits>

// Use this when MemberType alias doesn't work
#define PUTILS_MEMBER_TYPE(T, Member) std::remove_reference_t<decltype(std::declval<T>().*(std::declval<Member>()))>

namespace putils {
	template<typename MemberPtr>
	struct MemberTypeWrapper;

	template<typename T, typename Member>
	struct MemberTypeWrapper<Member T::*> {
		using type = Member;
	};

	template<typename T>
	using MemberType = typename MemberTypeWrapper<std::decay_t<T>>::type;

	template<typename T, typename U>
	std::ptrdiff_t member_offset(U T::* member) {
		return (ptrdiff_t)&(reinterpret_cast<T const volatile*>(NULL)->*member);
	}
}