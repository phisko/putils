#pragma once

#include <string_view>
#include "meta/table.hpp"
#include "meta/nameof.hpp"
#include "meta/for_each.hpp"
#include "traits.hpp"

#define putils_reflection_attribute(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).rfind("::") + 2).data(), memberPtr
#define putils_reflection_attribute_private(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).rfind("::") + 3).data(), memberPtr
#define putils_reflection_parent(T) putils_nameof(T), putils::meta::type<T>()

#define putils_reflection_class_name(className) static const auto reflection_get_class_name() { return putils_nameof(className); }
#define putils_reflection_attributes(...) static const auto & reflection_get_attributes() { \
    static thread_local const auto table = putils::make_table(__VA_ARGS__); \
    return table; \
}
#define putils_reflection_methods(...) static const auto & reflection_get_methods() { \
    static thread_local const auto table = putils::make_table(__VA_ARGS__); \
    return table; \
}
#define putils_reflection_parents(...) static const auto & reflection_get_parents() { \
    static thread_local const auto table = putils::make_table(__VA_ARGS__); \
    return table; \
}

namespace putils::reflection
{
	namespace detail {
		inline static const auto emptyTuple = putils::make_table();
	}

	putils_member_detector(reflection_get_class_name);
	template<typename T>
	using has_class_name = has_member_reflection_get_class_name<T>;
	template<typename T>
	const auto get_class_name() {
		if constexpr (has_class_name<T>())
			return T::reflection_get_class_name();
		else
			return typeid(T).name();
	}

	putils_member_detector(reflection_get_attributes);
	template<typename T>
	using has_attributes = has_member_reflection_get_attributes<T>;
	template<typename T>
	const auto & get_attributes() {
		if constexpr (has_attributes<T>())
			return T::reflection_get_attributes();
		else
			return detail::emptyTuple;
	}

	putils_member_detector(reflection_get_methods);
	template<typename T>
	using has_methods = has_member_reflection_get_methods<T>;
	template<typename T>
	const auto & get_methods() {
		if constexpr (has_methods<T>())
			return T::reflection_get_methods();
		else
			return detail::emptyTuple;
	}

	putils_member_detector(reflection_get_parents);
	template<typename T>
	using has_parents = has_member_reflection_get_parents<T>;
	template<typename T>
	const auto & get_parents() {
		if constexpr (has_parents<T>())
			return T::reflection_get_parents();
		else
			return detail::emptyTuple;
	}

	template<typename Attributes, typename Func> // Func: void(const char * name, MemberPointer ptr)
	void for_each_member(Attributes && attributes, Func && func) {
		putils::tuple_for_each(attributes.getKeyValues(), [&func](auto && p) {
			func(p.first, p.second);
		});
	}

	template<typename T, typename Func>
	void for_each_parent(Func && func) {
		for_each_member(get_parents<T>(), [&func](const char * name, auto && type) {
			func(name, type);

			using Parent = putils_wrapped_type(type);
			for_each_parent<Parent>(func);
		});
	}

	template<typename T, typename Func>
	void for_each_attribute(Func && func) {
		for_each_member(get_attributes<T>(), func);

		for_each_parent<T>([&func](const char * name, auto && type) {
			using Parent = putils_wrapped_type(type);
			for_each_attribute<Parent>(func);
		});
	}

	template<typename T, typename Func>
	void for_each_method(Func && func) {
		for_each_member(get_methods<T>(), func);

		for_each_parent<T>([&func](const char * name, auto && type) {
			using Parent = putils_wrapped_type(type);
			for_each_method<Parent>(func);
		});
	}
}
