#pragma once

#include <string_view>
#include <optional>
#include "meta/table.hpp"
#include "meta/nameof.hpp"
#include "meta/for_each.hpp"
#include "traits.hpp"
#include "members.hpp"

// type_info template, specialized for each reflectible type

namespace putils::reflection {
	template<typename T>
	struct type_info {
		// static constexpr auto class_name = const char *;
		// static constexpr auto attributes = std::tuple<std::pair<const char *, MemberPointer>...>;
		// static constexpr auto methods = std::tuple<std::pair<const char *, MemberPointer>...>;
		// static constexpr auto parents = std::tuple<putils::meta::type<Parent>...>;
		// static constexpr auto used_types = std::tuple<putils::meta::type<UsedType>...>;
	};
}

/*
========== Example ==========

struct A {};

#define refltype A
putils_reflection_info{
	putils_reflection_class_name;
};
#undef refltype

struct B : A {
	int i;
	double d;

	void f() const;
	bool g(int param);
};

#define refltype B
putils_reflection_info{
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(i),
		putils_reflection_attribute(d)
	);
	putils_reflection_methods(
		putils_reflection_attribute(f),
		putils_reflection_attribute(g)
	);
	putils_reflection_parents(
		putils_reflection_type(A)
	);
};
#undef refltype

template<typename T>
struct C {
	int i;
};

template<typename T>
#define refltype C<T>
putils_reflection_info_template{
	putils_reflection_class_name;
	putils_reflection_attributes(
		putils_reflection_attribute(i)
	);
};
*/

// Define a type_info for a templated type, like C in the example above
#define putils_reflection_info_template struct putils::reflection::type_info<refltype>

// Define a type_info for a given type
#define putils_reflection_info template<> putils_reflection_info_template 

// Place inside classes that need to have reflectible private fields
#define putils_reflection_friend(T) friend struct putils::reflection::type_info<T>;

// implementation detail
#define putils_impl_reflection_static_table(NAME, ...) static constexpr auto NAME = putils::make_table(__VA_ARGS__);
#define putils_impl_reflection_static_tuple(NAME, ...) static constexpr auto NAME = std::make_tuple(__VA_ARGS__);

// Lets you define a custom class name
#define putils_reflection_custom_class_name(className) static constexpr auto class_name = putils_nameof(className) + (std::string_view(putils_nameof(className)).rfind("::") != std::string::npos ? std::string_view(putils_nameof(className)).rfind("::") + 2 : 0);

// Uses refltype as class name
#define putils_reflection_class_name putils_reflection_custom_class_name(refltype)

#define putils_reflection_attributes(...) putils_impl_reflection_static_table(attributes, __VA_ARGS__)
#define putils_reflection_methods(...) putils_impl_reflection_static_table(methods, __VA_ARGS__)
#define putils_reflection_parents(...) putils_impl_reflection_static_tuple(parents, __VA_ARGS__)
#define putils_reflection_used_types(...) putils_impl_reflection_static_tuple(used_types, __VA_ARGS__)

#define putils_reflection_attribute(member) #member, &refltype::##member
#define putils_reflection_attribute_private(member) #member + 1, &refltype::##member
#define putils_reflection_type(T) putils::meta::type<T>()


namespace putils::reflection {
	template<typename T>
	constexpr auto get_class_name() {
		if constexpr (detail::type_info_with_parents<T>::class_name != nullptr)
			return detail::type_info_with_parents<T>::class_name;
		else
			return typeid(T).name();
	}

#define putils_impl_reflection_member_getter_and_for_each(NAME, foreach) \
	template<typename T>\
	constexpr const auto & get_##NAME##s() {\
		return detail::type_info_with_parents<T>::##NAME##s;\
	}\
\
	template<typename T, typename Func>\
	constexpr void for_each_##NAME(Func && func) {\
		foreach(get_##NAME##s<T>(), func);\
	}

	// get_attributes/methods<T>():
	//		returns a tuple<pair<const char *, MemberPointer>> of all the attributes/methods in T and its parents
	// get_parents/used_types<T>():
	//		returns a tuple<putils::meta::type<T>> of all the parents/used types of T and its parents

	// for_each_attribute/method<T>(functor):
	//		calls `functor(name, memberPointer)` for each attribute/method in T and its parents
	// for_each_parent/used_type<T>(functor):
	//		calls `functor(putils::meta::type<Type>)` for each parent/used type of T and its parents

	putils_impl_reflection_member_getter_and_for_each(attribute, detail::for_each_member_table);
	putils_impl_reflection_member_getter_and_for_each(method, detail::for_each_member_table);
	putils_impl_reflection_member_getter_and_for_each(parent, tuple_for_each);
	putils_impl_reflection_member_getter_and_for_each(used_type, tuple_for_each);

#pragma region attributes
	// For each attribute in T, get a reference to it in obj
	template<typename T, typename Func>
	constexpr void for_each_attribute(T && obj, Func && func) {
		for_each_attribute<std::decay_t<T>>([&](const char * name, const auto member) {
			func(name, obj.*member);
		});
	}

	// Try to find an attribute called "name" and get a member pointer to it
	template<typename Ret, typename T>
	constexpr std::optional<Ret T::*> get_attribute(std::string_view name) {
		Ret T:: * ret = nullptr;
		bool found = false;
		for_each_attribute<T>([&](std::string_view attrName, const auto member) {
			if constexpr (std::is_same<putils::MemberType<putils_typeof(member)>, Ret>()) {
				if (name == attrName) {
					ret = member;
					found = true;
				}
			}
		});

		if (found)
			return ret;
		return std::nullopt;
	}

	// Try to find an attribute called "name" and get a reference to it in obj
	template<typename Ret, typename T>
	constexpr auto get_attribute(T && obj, std::string_view name) {
		const auto member = get_attribute<Ret, std::decay_t<T>>(name);

		using ReturnType = decltype(&(obj.*(*member)));
		if (!member)
			return (ReturnType)nullptr;
		return &(obj.*(*member));
	}
#pragma endregion

#pragma region methods
	// For each method in T, get a functor calling it on obj
	template<typename T, typename Func>
	constexpr void for_each_method(T && obj, Func && func) {
		for_each_method<std::decay_t<T>>([&](const char * name, const auto member) {
			func(name, [&obj, member](auto && ... args) { return (obj.*member)(FWD(args)...); });
		});
	}

	// Try to find a method called "name" and get a member function pointer to it
	template<typename Ret, typename T>
	constexpr std::optional<Ret T::*> get_method(std::string_view name) {
		std::optional<Ret T:: *> ret;
		for_each_method<T>([&](std::string_view attrName, const auto member) {
			using wantedSignature = putils::member_function_signature<Ret T::*>;
			using currentSignature = putils::member_function_signature<putils_typeof(member)>;
			if constexpr (std::is_same<wantedSignature, currentSignature>()) {
				if (name == attrName)
					ret = (Ret T::*)member;
			}
		});
		return ret;
	}

	// Try to find a method called "name" and get a functor calling it on obj
	template<typename Ret, typename T>
	constexpr auto get_method(T && obj, std::string_view name) {
		const auto member = get_method<Ret, std::decay_t<T>>(name);

		const auto ret = [&obj, member](auto && ... args) {
			return (obj.*(*member))(FWD(args)...);
		};

		using ReturnType = std::optional<decltype(ret)>;
		if (!member)
			return ReturnType(std::nullopt);
		return ReturnType(ret);
	}
#pragma endregion

#pragma region impl
	namespace detail {
		inline static const auto emptyTuple = putils::make_table();
	}

#pragma region macros
#define putils_impl_reflection_member_detector(NAME) \
	namespace detail {\
		putils_member_detector(NAME);\
	}\
	template<typename T>\
	constexpr bool has_##NAME() {\
		return detail::has_member_##NAME<type_info<T>>();\
	}

#define putils_impl_reflection_member_detector_with_parents(NAME) \
	namespace detail {\
		putils_member_detector(NAME);\
	}\
	template<typename T>\
	constexpr bool has_##NAME() {\
		if constexpr (detail::has_member_##NAME<type_info<T>>())\
			return true;\
		bool ret = false;\
		for_each_parent<T>([&](const auto parent) {\
			using Parent = putils_wrapped_type(parent);\
			if constexpr (detail::has_member_##NAME<type_info<Parent>>())\
				ret = true;\
		});\
		return ret;\
	}

#define putils_impl_reflection_member_get_single(NAME, defaultValue) \
	namespace detail{\
		template<typename T>\
		constexpr const auto & get_single_##NAME() {\
			if constexpr (detail::has_member_##NAME<type_info<T>>())\
				return type_info<T>::##NAME;\
			else\
				return defaultValue;\
		}\
	}

#define putils_impl_reflection_member_get_all(NAME) \
	namespace detail {\
		template<typename T, typename ... Ts>\
		constexpr auto get_all_##NAME() {\
			if constexpr (sizeof...(Ts) == 0)\
				return get_single_##NAME<T>();\
			else\
				return std::tuple_cat(get_single_##NAME<T>(), get_all_##NAME<Ts...>());\
		}\
\
		template<typename T, typename ... Parents>\
		constexpr auto get_all_##NAME(const std::tuple<putils::meta::type<Parents>...> &) {\
			return get_all_##NAME<T, Parents...>();\
		}\
	}
#pragma endregion

#pragma region parents
	putils_impl_reflection_member_detector(parents);
	putils_impl_reflection_member_get_single(parents, detail::emptyTuple);
	namespace detail {
		template<typename T>
		constexpr auto get_all_parents();

		template<typename ... Ts>
		constexpr auto get_all_parents(const std::tuple<putils::meta::type<Ts>...> &) {
			return std::tuple_cat(get_all_parents<Ts>()...);
		}
	
		template<typename T>
		constexpr auto get_all_parents() {
			if constexpr (has_parents<T>())
				return std::tuple_cat(get_single_parents<T>(), get_all_parents(get_single_parents<T>()));
			else
				return get_single_parents<T>();
		}
	}
#pragma endregion

#pragma region class_name
	putils_impl_reflection_member_detector(class_name);
	putils_impl_reflection_member_get_single(class_name, typeid(T).name());
#pragma endregion

#define putils_impl_reflection_member(NAME) \
	putils_impl_reflection_member_detector_with_parents(NAME) \
	putils_impl_reflection_member_get_single(NAME, detail::emptyTuple) \
	putils_impl_reflection_member_get_all(NAME)

	putils_impl_reflection_member(attributes);
	putils_impl_reflection_member(methods);
	putils_impl_reflection_member(used_types);

	namespace detail {
		template<typename T>
		struct type_info_with_parents {
			static constexpr auto class_name = has_member_class_name<type_info<T>>() ? get_single_class_name<T>() : nullptr;
			static constexpr auto parents = get_all_parents<T>();
			static constexpr auto attributes = get_all_attributes<T>(parents);
			static constexpr auto methods = get_all_methods<T>(parents);
			static constexpr auto used_types = get_all_used_types<T>(parents);
		};

		template<typename Attributes, typename Func>
		constexpr void for_each_member_table(Attributes && attributes, Func && func) {
			putils::tuple_for_each(attributes, [&func](auto && p) {
				func(p.first, p.second);
			});
		}
	}
#pragma endregion
}
