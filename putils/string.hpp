#pragma once

// stl
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <span>

// fmt
#include <fmt/core.h>

// reflection
#include "putils/reflection.hpp"

namespace putils {
	namespace detail_string {
		constexpr char default_class_name[32] = "putils_string";
	}

	template<size_t MaxSize, const char * ClassName = detail_string::default_class_name>
	class string {
	public:
		using value_type = char;
		static constexpr auto npos = std::string::npos;

	public:
		constexpr string() noexcept = default;

		constexpr string(std::string_view str) noexcept;
		constexpr string(const char * str) noexcept;

		constexpr string & operator=(std::string_view str) noexcept;
		constexpr string & operator=(const char * str) noexcept;

		template<typename... FmtArgs>
		constexpr string(fmt::format_string<FmtArgs...> format, FmtArgs &&... fmt_args) noexcept;

		// Reflectible (no overload/template)
		constexpr void assign(std::string_view str) noexcept;

		template<typename... FmtArgs>
		constexpr void set(fmt::format_string<FmtArgs...> format, FmtArgs &&... fmt_args) noexcept;

		template<typename T>
		constexpr string & operator+=(const T & rhs) noexcept;

		template<typename T>
		constexpr string operator+(const T & rhs) const noexcept;

		constexpr string substr(size_t start = 0, size_t count = MaxSize) const noexcept;

		constexpr void clear() noexcept;

		constexpr size_t find(const char * substr, size_t pos = 0) const noexcept;
		constexpr size_t find(char c, size_t pos = 0) const noexcept;
		constexpr size_t find(std::string_view substr, size_t pos = 0) const noexcept;

		constexpr char at(size_t i) const noexcept;

	public:
		using buffer = value_type[MaxSize];

		constexpr operator std::string_view() const noexcept;

		constexpr char operator[](size_t i) const noexcept;
		constexpr char & operator[](size_t i) noexcept;

		constexpr std::string str() const noexcept;
		constexpr const buffer & data() const noexcept;
		constexpr const buffer & c_str() const noexcept;

		constexpr size_t size() const noexcept;
		constexpr size_t length() const noexcept;
		constexpr bool empty() const noexcept;
		constexpr size_t max_size() const noexcept;
		constexpr bool full() const noexcept;

	public:
		constexpr const char * begin() const noexcept;
		constexpr const char * end() const noexcept;

		constexpr char * begin() noexcept;
		constexpr char * end() noexcept;

		constexpr operator std::span<const char>() const noexcept;
		constexpr operator std::span<char>() noexcept;

	private:
		size_t _size = 0;
		buffer _buff = "";
	};

	template<size_t MaxSize, const char * ClassName>
#define refltype string<MaxSize, ClassName>
	putils_reflection_info_template {
		static constexpr auto class_name = ClassName;
		putils_reflection_methods(
			putils_reflection_attribute(clear),
			putils_reflection_attribute(at),
			putils_reflection_attribute(assign),
			putils_reflection_attribute(c_str),
			putils_reflection_attribute(size),
			putils_reflection_attribute(empty),
			putils_reflection_attribute(full)
		);
	};
#undef refltype

	template<size_t Size, const char * Name>
	constexpr auto begin(const string<Size, Name> & s) noexcept;
	template<size_t Size, const char * Name>
	constexpr auto end(const string<Size, Name> & s) noexcept;

	template<size_t Size, const char * Name>
	constexpr bool operator==(const string<Size, Name> & lhs, const char * rhs) noexcept;
	template<size_t Size, const char * Name>
	constexpr bool operator!=(const string<Size, Name> & lhs, const char * rhs) noexcept;

	template<size_t Size, const char * Name>
	constexpr bool operator==(const string<Size, Name> & lhs, const std::string & rhs) noexcept;
	template<size_t Size, const char * Name>
	constexpr bool operator!=(const string<Size, Name> & lhs, const std::string & rhs) noexcept;

	template<size_t Size, const char * Name>
	constexpr bool operator==(const string<Size, Name> & lhs, const string<Size, Name> & rhs) noexcept;
	template<size_t Size, const char * Name>
	constexpr bool operator!=(const string<Size, Name> & lhs, const string<Size, Name> & rhs) noexcept;

	template<size_t S1, const char * N1, size_t S2, const char * N2>
	constexpr bool operator==(const string<S1, N1> & lhs, const string<S2, N2> & rhs) noexcept;
	template<size_t S1, const char * N1, size_t S2, const char * N2>
	constexpr bool operator!=(const string<S1, N1> & lhs, const string<S2, N2> & rhs) noexcept;

	template<size_t Size, const char * Name>
	constexpr inline bool operator==(const char * lhs, const putils::string<Size, Name> & rhs) noexcept;
	template<size_t Size, const char * Name>
	constexpr inline bool operator!=(const char * lhs, const putils::string<Size, Name> & rhs) noexcept;

	template<size_t Size, const char * Name>
	constexpr inline bool operator==(const std::string & lhs, const putils::string<Size, Name> & rhs) noexcept;
	template<size_t Size, const char * Name>
	constexpr inline bool operator!=(const std::string & lhs, const putils::string<Size, Name> & rhs) noexcept;

	template<size_t Size, const char * Name>
	constexpr std::ostream & operator<<(std::ostream & s, const putils::string<Size, Name> & str) noexcept;
}

namespace std {
	template<size_t N, const char * S>
	struct hash<putils::string<N, S>> {
		constexpr std::size_t operator()(const putils::string<N, S> & s) const noexcept;
	};
}

#include "string.inl"