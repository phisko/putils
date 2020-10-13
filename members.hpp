#pragma once

#include <type_traits>

// Use this when MemberType alias doesn't work
#define PUTILS_MEMBER_TYPE(T, MemberPtr) std::remove_reference_t<decltype(std::declval<T>().*(std::declval<MemberPtr>()))>

namespace putils {
	namespace detail {
		template<typename MemberPtr>
		struct MemberTypeWrapper;

		template<typename T, typename Member>
		struct MemberTypeWrapper<Member T:: *> {
			using type = Member;
		};
	}

	template<typename MemberPtr>
	using MemberType = typename detail::MemberTypeWrapper<std::decay_t<MemberPtr>>::type;

	namespace detail {
		template<typename MemberPtr>
		struct ObjectTypeWrapper;

		template<typename T, typename Member>
		struct ObjectTypeWrapper<Member T:: *> {
			using type = T;
		};
	}

	template<typename MemberPtr>
	using ObjectType = typename detail::ObjectTypeWrapper<std::decay_t<MemberPtr>>::type;

	template<typename T, typename U>
	std::ptrdiff_t member_offset(U T::* member) {
		return (ptrdiff_t)&(reinterpret_cast<T const volatile*>(NULL)->*member);
	}
}