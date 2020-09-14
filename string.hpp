#pragma once

#include <assert.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <span>
#include "reflection.hpp"

namespace putils {
	namespace detail_string {
		static constexpr char defaultClassName[] = "putils_string";
	}

	template<size_t MaxSize, const char * ClassName = detail_string::defaultClassName>
	class string {
	public:
		using value_type = char;
		static constexpr auto npos = std::string::npos;
		static constexpr auto max_size = MaxSize;

	public:
		constexpr string() {
			static_assert(MaxSize > 0);
			_buff[0] = 0;
		}

		template<typename ...Args>
		constexpr string(const char * format, Args... args) {
			static_assert(MaxSize > 0);
			_size = snprintf(_buff, MaxSize, format, args...);
		}

		constexpr string(const char * str) : _size(strlen(str)) {
			static_assert(MaxSize > 0);
			strncpy_s(_buff, str, MaxSize);
		}

		constexpr string(const std::string & s) : _size(s.size()) {
			static_assert(MaxSize > 0);
			assert(_size < MaxSize);
			strncpy_s(_buff, s.data(), _size);
		}

		constexpr string(std::string_view s) : _size(s.size()) {
			static_assert(MaxSize > 0);
			assert(_size < MaxSize);
			strncpy_s(_buff, s.data(), _size);
		}

		constexpr string & operator=(const char * rhs) {
			strncpy_s(_buff, rhs, MaxSize);
			_size = strlen(rhs);
			return *this;
		}

		template<size_t N, const char * S>
		constexpr string & operator=(const string<N, S> & rhs) {
			strncpy_s(_buff, rhs.c_str(), MaxSize);
			_size = rhs.size();
			return *this;
		}

		constexpr string & operator=(std::string_view rhs) {
			assert(rhs.size() < MaxSize);
			strncpy_s(_buff, rhs.data(), rhs.size());
			_size = rhs.size();
			return *this;
		}

		constexpr string & operator=(const std::string & rhs) {
			assert(rhs.size() < MaxSize);
			strncpy_s(_buff, rhs.data(), rhs.size());
			_size = rhs.size();
			return *this;
		}

		template<typename ...Args>
		constexpr void set(const char * format, Args... args) {
			_size = snprintf(_buff, MaxSize, format, args...);
		}

		// Reflectible (no overload/template)
		constexpr void assign(const char * s) {
			*this = s;
		}

		constexpr string & operator+=(const char * pRhs) {
			strncat_s(_buff, pRhs, MaxSize - _size);
			_size += strlen(pRhs);
			assert(_size < MaxSize);
			return *this;
		}

		constexpr string & operator+=(std::string_view rhs) {
			strncat_s(_buff, rhs.data(), rhs.size());
			_size += rhs.size();
			assert(_size < MaxSize);
			return *this;
		}

		template<size_t Size, const char * Name>
		constexpr string & operator+=(const string<Size, Name> & rhs) { return *this += rhs.c_str(); }

		constexpr string & operator+=(char rhs) {
			_buff[_size++] = rhs;
			assert(_size < MaxSize);
			_buff[_size] = 0;
			return *this;
		}

		constexpr string & operator+=(int rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%d", rhs);
			return *this;
		}

		constexpr string & operator+=(float rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%f", rhs);
			return *this;
		}

		constexpr string & operator+=(size_t rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%zu", rhs);
			return *this;
		}

#if defined(_MSC_VER) && _MSC_VER > 1920 // MSVC 2019
		constexpr string & operator+=(unsigned int rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%zu", rhs);
			return *this;
		}

		constexpr string & operator+=(intptr_t rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%zu", rhs);
			return *this;
		}
#endif

		template<typename T>
		constexpr string operator+(T rhs) const {
			string ret(_buff);
			return ret += rhs;
		}

		constexpr bool operator==(const char * rhs) const { return strcmp(_buff, rhs) == 0; }
		constexpr bool operator!=(const char * rhs) const { return strcmp(_buff, rhs) != 0; }

		constexpr bool operator==(const std::string & rhs) const { return *this == rhs.c_str(); }
		constexpr bool operator!=(const std::string & rhs) const { return *this != rhs.c_str(); }

		template<size_t Size, const char * Name>
		constexpr bool operator==(const string<Size, Name> & rhs) const { return *this == rhs.c_str(); }
		template<size_t Size, const char * Name>
		constexpr bool operator!=(const string<Size, Name> & rhs) const { return *this != rhs.c_str(); }

		constexpr string substr(size_t start = 0, size_t count = MaxSize) const {
			string ret;
			strncpy_s(ret._buff, _buff + start, count);
			ret._size = std::min(strlen(_buff + start), count);
			return ret;
		}

		constexpr void clear() {
			_buff[0] = 0;
			_size = 0;
		}

		constexpr size_t find(const char * substr, size_t pos = 0) const {
			assert(pos <= _size);

			const auto ret = strstr(_buff + pos, substr);
			if (ret == nullptr)
				return npos;
			return ret - _buff;
		}

		constexpr size_t find(char c, size_t pos = 0) const {
			for (size_t i = pos; i < _size; ++i)
				if (_buff[i] == c)
					return i;
			return npos;
		}

		constexpr char at(size_t i) const { return _buff[i]; }

	public:
		using Buffer = value_type[MaxSize];
		constexpr operator const Buffer &() const { return _buff; }
		constexpr operator Buffer &() { return _buff; }

		constexpr operator std::string_view() const { return std::string_view(_buff); }
		
		constexpr char operator[](size_t i) const { return _buff[i]; }
		constexpr char & operator[](size_t i) { return _buff[i]; }

		constexpr std::string str() const { return _buff; }
		constexpr const Buffer & data() const { return _buff; }
		constexpr const Buffer & c_str() const { return _buff; }

		constexpr size_t size() const { return _size; }
		constexpr size_t length() const { return _size; }
		constexpr bool empty() const { return _size == 0; }
		constexpr bool full() const { return _size >= MaxSize; }

		constexpr friend std::ostream & operator<<(std::ostream & s, const string & str) {
			return s << str._buff;
		}

	public:
		constexpr const char * begin() const { return _buff; }
		constexpr const char * end() const { return _buff + _size; }

		constexpr char * begin() { return _buff; }
		constexpr char * end() { return _buff + _size; }

		constexpr operator std::span<const char>() const { return { _buff, _size }; }
		constexpr operator std::span<char>() { return { _buff, _size }; }

	private:
		size_t _size = 0;
		Buffer _buff;
	};

	template<size_t MaxSize, const char * ClassName>
#define refltype string<MaxSize, ClassName>
	putils_reflection_info_template{
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
	constexpr auto begin(const string<Size, Name> & s) { return s.begin(); }
	template<size_t Size, const char * Name>
	constexpr auto end(const string<Size, Name> & s) { return s.end(); }

	template<size_t Size, const char * Name>
	constexpr inline bool operator==(const char * lhs, const putils::string<Size, Name> & rhs) { return rhs == lhs; }
	template<size_t Size, const char * Name>
	constexpr inline bool operator!=(const char * lhs, const putils::string<Size, Name> & rhs) { return rhs != lhs; }

	template<size_t Size, const char * Name>
	constexpr inline bool operator==(const std::string & lhs, const putils::string<Size, Name> & rhs) { return rhs == lhs; }
	template<size_t Size, const char * Name>
	constexpr inline bool operator!=(const std::string & lhs, const putils::string<Size, Name> & rhs) { return rhs != lhs; }
}

namespace std {
	template<size_t N, const char * S>
	struct hash<putils::string<N, S>> {
		constexpr std::size_t operator()(const putils::string<N, S> & s) const {
			size_t result = 0;
			constexpr auto prime = 31;
			for (const auto c : s)
				result = c + (result * prime);
			return result;
		}
	};
}
