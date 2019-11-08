#pragma once

#include <string_view>
#include "meta/table.hpp"
#include "meta/nameof.hpp"
#include "meta/for_each.hpp"
#include "traits.hpp"

#define pmeta_reflectible_attribute(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).rfind("::") + 2).data(), memberPtr
#define pmeta_reflectible_attribute_private(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).rfind("::") + 3).data(), memberPtr
#define pmeta_reflectible_parent(T) pmeta_nameof(T), pmeta::type<T>()

#define pmeta_get_class_name(className) static const auto get_class_name() { return pmeta_nameof(className); }
#define pmeta_get_attributes(...) static const auto & get_attributes() { \
    static const auto table = pmeta::make_table(__VA_ARGS__); \
    return table; \
}
#define pmeta_get_methods(...) static const auto & get_methods() { \
    static const auto table = pmeta::make_table(__VA_ARGS__); \
    return table; \
}
#define pmeta_get_parents(...) static const auto & get_parents() { \
    static const auto table = pmeta::make_table(__VA_ARGS__); \
    return table; \
}

namespace putils
{
	putils_member_detector(get_class_name);
	putils_member_detector(get_attributes);
	putils_member_detector(get_methods);
	putils_member_detector(get_parents);

	template<typename Attributes, typename Func> // Func: void(const char * name, MemberPointer ptr)
	void for_each_member(Attributes && attributes, Func && func) {
		pmeta::tuple_for_each(attributes.getKeyValues(), [&func](auto && p) {
			func(p.first, p.second);
		});
	}

	template<typename T, typename Func>
	void for_each_parent(Func && func) {
		if constexpr (putils::has_member_get_parents<T>::value) {
			for_each_member(T::get_parents(), [&func](const char * name, auto && type) {
				func(name, type);

				using Parent = pmeta_wrapped(type);
				for_each_parent<Parent>(func);
			});
		}
	}

	template<typename T, typename Func>
	void for_each_attribute(Func && func) {
		if constexpr (putils::has_member_get_attributes<T>::value)
			for_each_member(T::get_attributes(), func);

		for_each_parent<T>([&func](const char * name, auto && type) {
			using Parent = pmeta_wrapped(type);
			for_each_attribute<Parent>(func);
		});
	}

	template<typename T, typename Func>
	void for_each_method(Func && func) {
		if constexpr (putils::has_member_get_methods<T>::value)
			for_each_member(T::get_methods(), func);

		for_each_parent<T>([&func](const char * name, auto && type) {
			using Parent = pmeta_wrapped(type);
			for_each_method<Parent>(func);
		});
	}
}
