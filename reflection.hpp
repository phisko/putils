#pragma once

#include <string_view>
#include <optional>

namespace putils::reflection {
	template<typename T>
	struct type_info {
		// static constexpr auto class_name = const char *;
		// static constexpr auto attributes = std::tuple<std::pair<const char *, MemberPointer>...>;
		// static constexpr auto methods = std::tuple<std::pair<const char *, MemberPointer>...>;
		// static constexpr auto parents = std::tuple<putils::meta::type<Parent>...>;
		// static constexpr auto used_types = std::tuple<putils::meta::type<UsedType>...>;
	};

	template<typename T>
	constexpr auto get_class_name() noexcept;

	// For each parent of T, get a `putils::type<Parent>` object
	template<typename T, typename Func>
	constexpr void for_each_parent(Func && func) noexcept;

	// For each type used by T, get a `putils::type<Type>` object
	template<typename T, typename Func>
	constexpr void for_each_used_type(Func && func) noexcept;

	// For each attribute in T, get its name and a member pointer to it
	template<typename T, typename Func>
	constexpr void for_each_attribute(Func && func) noexcept;

	// For each attribute in T, get its name and a reference to it in obj
	template<typename T, typename Func>
	constexpr void for_each_attribute(T && obj, Func && func) noexcept;

	// Try to find an attribute called "name" and get a member pointer to it
	template<typename Ret, typename T>
	constexpr std::optional<Ret T:: *> get_attribute(std::string_view name) noexcept;

	// Try to find an attribute called "name" and get a reference to it in obj
	template<typename Ret, typename T>
	constexpr auto get_attribute(T && obj, std::string_view name) noexcept;

	// For each method in T, get its name and a member pointer to it
	template<typename T, typename Func>
	constexpr void for_each_method(Func && func) noexcept;

	// For each method in T, get its name and a functor calling it on obj
	template<typename T, typename Func>
	constexpr void for_each_method(T && obj, Func && func) noexcept;

	// Try to find a method called "name" and get a member function pointer to it
	template<typename Ret, typename T>
	constexpr std::optional<Ret T:: *> get_method(std::string_view name) noexcept;

	// Try to find a method called "name" and get a functor calling it on obj
	template<typename Ret, typename T>
	constexpr auto get_method(T && obj, std::string_view name) noexcept;
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

int main() {
	B object;
	putils::reflection::for_each_attribute(object, [](const char * name, const auto & attr) {
		std::cout << name << ": " << attr << '\n';
	});

	return 0;
}
*/

#include "reflection.inl"