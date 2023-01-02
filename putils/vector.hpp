#pragma once

// stl
#include <span>
#include <cassert>

// reflection
#include "putils/reflection.hpp"

// meta
#include "putils/meta/fwd.hpp"

// putils
#include "anonymous_variable.hpp"

namespace putils {
	namespace detail_vector {
		constexpr char default_class_name[32] = "putils_vector";
	}

	template<typename T, size_t MaxSize, const char * ClassName = detail_vector::default_class_name>
	class vector {
	public:
		using value_type = T;
		using size_type = size_t;
		using reference = T &;
		using iterator = T *;
		using const_iterator = const T *;

	public:
		constexpr vector() noexcept = default;

		template<size_t N>
		constexpr vector(const T (&arr)[N]) noexcept;

		constexpr vector(std::initializer_list<T> arr) noexcept;

		template<typename Val>
		constexpr T & push_back(Val && val) noexcept;

		template<typename Val>
		constexpr void try_push_back(Val && val) noexcept;

		template<typename... Args>
		constexpr T & emplace_back(Args &&... args) noexcept;

		template<typename... Args>
		constexpr void try_emplace_back(Args &&... args) noexcept;

		// Reflectible (no templates/overloads)
		constexpr T & add(const T & val) noexcept;
		constexpr void try_add(const T & val) noexcept;

		constexpr T & front() noexcept;
		constexpr const T & front() const noexcept;

		constexpr T & first() noexcept;

		constexpr T & back() noexcept;
		constexpr const T & back() const noexcept;

		constexpr T & last() noexcept;

		constexpr T & operator[](size_t index) noexcept;
		constexpr const T & operator[](size_t index) const noexcept;
		constexpr T & at(size_t index) noexcept;
		constexpr const T & at(size_t index) const noexcept;

		// Reflectible (no templates/overloads)
		constexpr T & get(size_t index) noexcept;

		constexpr void erase(T * ptr) noexcept;
		constexpr void erase(T * start, T * until) noexcept;

		// Reflectible (no templates/overloads)
		constexpr void remove(T * ptr) noexcept;

		constexpr auto size() const noexcept;
		constexpr bool empty() const noexcept;
		constexpr bool full() const;

		constexpr auto begin() const noexcept;
		constexpr auto end() const noexcept;

		constexpr auto begin() noexcept;
		constexpr auto end() noexcept;

		constexpr auto data() const noexcept;
		constexpr auto data() noexcept;

		constexpr void clear() noexcept;
		constexpr void resize(size_t size) noexcept;
		constexpr size_t capacity() const noexcept;

		constexpr operator std::span<const T>() const noexcept;
		constexpr operator std::span<T>() noexcept;

	private:
		T _buff[MaxSize] = {};
		size_t _size = 0;
	};

	template<typename T, size_t Size, const char * Name>
	constexpr auto begin(const vector<T, Size, Name> & v) noexcept;
	template<typename T, size_t Size, const char * Name>
	constexpr auto end(const vector<T, Size, Name> & v) noexcept;

	template<typename... Args>
	constexpr auto make_vector(Args &&... args) noexcept -> vector<std::common_type_t<Args...>, sizeof...(Args)>;

	template<typename>
	struct is_vector : std::false_type {};

	template<typename T, size_t Size, const char * Name>
	struct is_vector<putils::vector<T, Size, Name>> : std::true_type {};
}

template<typename T, size_t MaxSize, const char * ClassName>
#define refltype putils::vector<T, MaxSize, ClassName>
putils_reflection_info_template {
	static constexpr auto class_name = ClassName;
	putils_reflection_methods(
		putils_reflection_attribute(add),
		putils_reflection_attribute(try_add),
		putils_reflection_attribute(first),
		putils_reflection_attribute(last),
		putils_reflection_attribute(get),
		putils_reflection_attribute(remove),
		putils_reflection_attribute(size),
		putils_reflection_attribute(empty),
		putils_reflection_attribute(full),
		putils_reflection_attribute(clear)
	);
};
#undef refltype

#include "vector.inl"
