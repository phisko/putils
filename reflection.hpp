#pragma once

#include <string_view>
#include <optional>
#include "meta/table.hpp"
#include "meta/nameof.hpp"
#include "meta/for_each.hpp"
#include "traits.hpp"

#define putils_reflection_attribute(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).rfind("::") + 2).data(), memberPtr
#define putils_reflection_attribute_private(memberPtr) std::string_view(#memberPtr).substr(std::string_view(#memberPtr).rfind("::") + 3).data(), memberPtr
#define putils_reflection_type(T) putils_nameof(T), putils::meta::type<T>()

#define putils_reflection_class_name(className) static const auto reflection_get_class_name() { return putils_nameof(className); }

#define putils_reflection_static_impl(NAME, ...) static const auto & NAME() {\
	static thread_local const auto table = putils::make_table(__VA_ARGS__); \
	return table; \
}

#define putils_reflection_attributes(...) putils_reflection_static_impl(reflection_get_attributes, __VA_ARGS__)
#define putils_reflection_methods(...) putils_reflection_static_impl(reflection_get_methods, __VA_ARGS__)
#define putils_reflection_parents(...) putils_reflection_static_impl(reflection_get_parents, __VA_ARGS__)
#define putils_reflection_used_types(...) putils_reflection_static_impl(reflection_get_used_types, __VA_ARGS__)

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

	putils_member_detector(reflection_get_used_types);
	template<typename T>
	using has_used_types = has_member_reflection_get_used_types<T>;
	template<typename T>
	const auto & get_used_types() {
		if constexpr (has_used_types<T>())
			return T::reflection_get_used_types();
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
	void for_each_attribute(T && obj, Func && func) {
		for_each_attribute<T>([&](const char * name, const auto member) {
			func(name, obj.*member);
		});
	}

	template<typename Ret, typename T>
	std::optional<Ret T::*> get_attribute(const char * name) {
		std::optional<Ret T:: *> ret;
		for_each_attribute<T>([&](const char * attrName, const auto member) {
			if constexpr (std::is_same<putils_typeof(member), Ret T:: *>()) {
				if (strcmp(name, attrName) == 0)
					ret = member;
			}
		});
		return ret;
	}

	template<typename Ret, typename T>
	Ret * get_attribute(T && obj, const char * name) {
		const auto member = get_attribute<Ret, std::decay_t<T>>(name);
		if (!member)
			return nullptr;
		return &(obj.*(*member));
	}

	template<typename T, typename Func>
	void for_each_method(Func && func) {
		for_each_member(get_methods<T>(), func);

		for_each_parent<T>([&func](const char * name, auto && type) {
			using Parent = putils_wrapped_type(type);
			for_each_method<Parent>(func);
		});
	}

	template<typename T, typename Func>
	void for_each_method(T && obj, Func && func) {
		for_each_method<T>([&](const char * name, const auto member) {
			func(name, [&](auto && ... args) { return (obj.*member)(FWD(args)...); });
		});
	}

	template<typename Ret, typename T>
	std::optional<Ret T::*> get_method(const char * name) {
		std::optional<Ret T:: *> ret;
		for_each_method<T>([&](const char * attrName, const auto member) {
			using wantedSignature = putils::member_function_signature<Ret T::*>;
			using currentSignature = putils::member_function_signature<putils_typeof(member)>;
			if constexpr (std::is_same<wantedSignature, currentSignature>()) {
				if (strcmp(name, attrName) == 0)
					ret = (Ret T::*)member;
			}
		});
		return ret;
	}

	template<typename Ret, typename T>
	auto get_method(T && obj, const char * name) {
		const auto member = get_method<Ret, std::decay_t<T>>(name);

		const auto ret = [&](auto && ... args) {
			return (obj.*(*member))(FWD(args)...);
		};

		using ReturnType = std::optional<decltype(ret)>;
		if (!member)
			return ReturnType(std::nullopt);
		return ReturnType(ret);
	}

	template<typename T, typename Func>
	void for_each_used_type(Func && func) {
		for_each_member(get_used_types<T>(), func);
	}
}
