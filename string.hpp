#pragma once

#include <assert.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include "reflection.hpp"

namespace putils {
	namespace detail_string {
		static constexpr char defaultClassName[] = "putils_string";
	}

	template<size_t MaxSize, const char * ClassName = detail_string::defaultClassName>
	class string {
	public:
		using value_type = char;
		static constexpr auto npos = (size_t)-1;
		static constexpr auto max_size = MaxSize;

	public:
		string() {
			static_assert(MaxSize > 0);
			_buff[0] = 0;
		}

		template<typename ...Args>
		string(const char * format, Args... args) {
			static_assert(MaxSize > 0);
			_size = snprintf(_buff, MaxSize, format, args...);
		}

		string(const char * str) : _size(strlen(str)) {
			static_assert(MaxSize > 0);
			strncpy_s(_buff, str, MaxSize);
		}

		string(const std::string & s) : _size(s.size()) {
			static_assert(MaxSize > 0);
			assert(_size < MaxSize);
			strncpy_s(_buff, s.data(), _size);
		}

		string(std::string_view s) : _size(s.size()) {
			static_assert(MaxSize > 0);
			assert(_size < MaxSize);
			strncpy_s(_buff, s.data(), _size);
		}

		string & operator=(const char * rhs) {
			strncpy_s(_buff, rhs, MaxSize);
			_size = strlen(rhs);
			return *this;
		}

		template<size_t N, const char * S>
		string & operator=(const string<N, S> & rhs) {
			strncpy_s(_buff, rhs.c_str(), MaxSize);
			_size = rhs.size();
			return *this;
		}

		string & operator=(std::string_view rhs) {
			assert(rhs.size() < MaxSize);
			strncpy_s(_buff, rhs.data(), rhs.size());
			_size = rhs.size();
			return *this;
		}

		string & operator=(const std::string & rhs) {
			assert(rhs.size() < MaxSize);
			strncpy_s(_buff, rhs.data(), rhs.size());
			_size = rhs.size();
			return *this;
		}

		template<typename ...Args>
		void set(const char * format, Args... args) {
			_size = snprintf(_buff, MaxSize, format, args...);
		}

		// Reflectible (no overload/template)
		void assign(const char * s) {
			*this = s;
		}

		string & operator+=(const char * pRhs) {
			strncat_s(_buff, pRhs, MaxSize - _size);
			_size += strlen(pRhs);
			assert(_size < MaxSize);
			return *this;
		}

		string & operator+=(std::string_view rhs) {
			strncat_s(_buff, rhs.data(), rhs.size());
			_size += rhs.size();
			assert(_size < MaxSize);
			return *this;
		}

		template<size_t Size, const char * Name>
		string & operator+=(const string<Size, Name> & rhs) { return *this += rhs.c_str(); }

		string & operator+=(char rhs) {
			_buff[_size++] = rhs;
			assert(_size < MaxSize);
			_buff[_size] = 0;
			return *this;
		}

		string & operator+=(int rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%d", rhs);
			return *this;
		}

		string & operator+=(float rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%f", rhs);
			return *this;
		}

		string & operator+=(size_t rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%zu", rhs);
			return *this;
		}

#if defined(_MSC_VER) && _MSC_VER > 1920 // MSVC 2019
		string & operator+=(unsigned int rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%zu", rhs);
			return *this;
		}

		string & operator+=(intptr_t rhs) {
			_size += snprintf(_buff + _size, MaxSize - _size, "%zu", rhs);
			return *this;
		}
#endif

		template<typename T>
		string operator+(T rhs) const {
			string ret(_buff);
			return ret += rhs;
		}

		bool operator==(const char * rhs) const { return strcmp(_buff, rhs) == 0; }
		bool operator!=(const char * rhs) const { return strcmp(_buff, rhs) != 0; }

		bool operator==(const std::string & rhs) const { return *this == rhs.c_str(); }
		bool operator!=(const std::string & rhs) const { return *this != rhs.c_str(); }

		template<size_t Size, const char * Name>
		bool operator==(const string<Size, Name> & rhs) const { return *this == rhs.c_str(); }
		template<size_t Size, const char * Name>
		bool operator!=(const string<Size, Name> & rhs) const { return *this != rhs.c_str(); }

		string substr(size_t start = 0, size_t count = MaxSize) const {
			string ret;
			strncpy_s(ret._buff, _buff + start, count);
			ret._size = std::min(strlen(_buff + start), count);
			return ret;
		}

		void clear() {
			_buff[0] = 0;
			_size = 0;
		}

		size_t find(const char * substr, size_t pos = 0) const {
			assert(pos <= _size);

			const auto ret = strstr(_buff + pos, substr);
			if (ret == nullptr)
				return npos;
			return ret - _buff;
		}

		size_t find(char c, size_t pos = 0) const {
			for (size_t i = pos; i < _size; ++i)
				if (_buff[i] == c)
					return i;
			return npos;
		}

		char at(size_t i) const { return _buff[i]; }

	public:
		using Buffer = value_type[MaxSize];
		operator const Buffer &() const { return _buff; }
		operator Buffer &() { return _buff; }

		operator std::string_view() const { return std::string_view(_buff); }
		
		char operator[](size_t i) const { return _buff[i]; }
		char & operator[](size_t i) { return _buff[i]; }

		std::string str() const { return _buff; }
		const Buffer & data() const { return _buff; }
		const Buffer & c_str() const { return _buff; }

		size_t size() const { return _size; }
		size_t length() const { return _size; }
		bool empty() const { return _size == 0; }
		bool full() const { return _size >= MaxSize; }

		friend std::ostream & operator<<(std::ostream & s, const string & str) {
			return s << str._buff;
		}

	public:
		const char * begin() const { return _buff; }
		const char * end() const { return _buff + _size; }

		char * begin() { return _buff; }
		char * end() { return _buff + _size; }

	private:
		size_t _size = 0;
		Buffer _buff;

	public:
		static const auto get_class_name() { return ClassName; }
		pmeta_get_attributes();
		pmeta_get_methods(
			pmeta_reflectible_attribute(&string::clear),
			pmeta_reflectible_attribute(&string::at),
			pmeta_reflectible_attribute(&string::assign),
			pmeta_reflectible_attribute(&string::size),
			pmeta_reflectible_attribute(&string::empty),
			pmeta_reflectible_attribute(&string::full)
		);
		pmeta_get_parents();
	};

	template<size_t Size, const char * Name>
	inline bool operator==(const char * lhs, const putils::string<Size, Name> & rhs) { return rhs == lhs; }
	template<size_t Size, const char * Name>
	inline bool operator!=(const char * lhs, const putils::string<Size, Name> & rhs) { return rhs != lhs; }

	template<size_t Size, const char * Name>
	inline bool operator==(const std::string & lhs, const putils::string<Size, Name> & rhs) { return rhs == lhs; }
	template<size_t Size, const char * Name>
	inline bool operator!=(const std::string & lhs, const putils::string<Size, Name> & rhs) { return rhs != lhs; }
}

namespace std {
	template<size_t N, const char * S>
	struct hash<putils::string<N, S>> {
		std::size_t operator()(const putils::string<N, S> & s) const {
			size_t result = 0;
			constexpr auto prime = 31;
			for (const auto c : s)
				result = c + (result * prime);
			return result;
		}
	};
}
