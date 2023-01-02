#include "string.hpp"

// stl
#include <cstring>
#include <cassert>

#define TemplateDecl template<size_t MaxSize, const char * ClassName>
#define TString string<MaxSize, ClassName>
// clang-format off

#ifdef _MSC_VER
#	define MY_STRNCPY strncpy_s
#	define MY_STRNCAT strncat_s
#else
#	define MY_STRNCPY strncpy
#	define MY_STRNCAT strncat
#endif

namespace putils {
	TemplateDecl
	constexpr TString::string() noexcept {
		static_assert(MaxSize > 0);
		_buff[0] = 0;
	}

	TemplateDecl
	template<typename... Args>
	constexpr TString::string(const char * format, Args... args) noexcept {
		static_assert(MaxSize > 0);
		_size = snprintf(_buff, MaxSize, format, args...);
	}

	TemplateDecl
	constexpr TString::string(const char * str) noexcept : _size(strlen(str)) {
		static_assert(MaxSize > 0);
		MY_STRNCPY(_buff, str, MaxSize);
	}

	TemplateDecl
	constexpr TString::string(const std::string & s) noexcept : _size(s.size()) {
		static_assert(MaxSize > 0);
		assert(_size < MaxSize);
		MY_STRNCPY(_buff, s.data(), _size);
	}

	TemplateDecl
	constexpr TString::string(std::string_view s) noexcept : _size(s.size()) {
		static_assert(MaxSize > 0);
		assert(_size < MaxSize);
		MY_STRNCPY(_buff, s.data(), _size);
	}

	TemplateDecl
	constexpr TString & TString::operator=(const char * rhs) noexcept {
		MY_STRNCPY(_buff, rhs, MaxSize);
		_size = strlen(rhs);
		return *this;
	}

	TemplateDecl
	template<size_t N, const char * S>
	constexpr TString & TString::operator=(const string<N, S> & rhs) noexcept {
		MY_STRNCPY(_buff, rhs.c_str(), MaxSize);
		_size = rhs.size();
		return *this;
	}

	TemplateDecl
	constexpr TString & TString::operator=(std::string_view rhs) noexcept {
		assert(rhs.size() < MaxSize);
		MY_STRNCPY(_buff, rhs.data(), rhs.size());
		_size = rhs.size();
		return *this;
	}

	TemplateDecl
	constexpr TString & TString::operator=(const std::string & rhs) noexcept {
		assert(rhs.size() < MaxSize);
		MY_STRNCPY(_buff, rhs.data(), rhs.size());
		_size = rhs.size();
		return *this;
	}

	TemplateDecl
	template<typename... Args>
	constexpr void TString::set(const char * format, Args... args) noexcept {
		_size = snprintf(_buff, MaxSize, format, args...);
	}

	TemplateDecl
	constexpr void TString::assign(const char * s) noexcept {
		*this = s;
	}

	TemplateDecl
	constexpr TString & TString::operator+=(const char * pRhs) noexcept {
		MY_STRNCAT(_buff, pRhs, MaxSize - _size);
		_size += strlen(pRhs);
		assert(_size < MaxSize);
		return *this;
	}

	TemplateDecl
	constexpr TString & TString::operator+=(std::string_view rhs) noexcept {
		MY_STRNCAT(_buff, rhs.data(), rhs.size());
		_size += rhs.size();
		assert(_size < MaxSize);
		return *this;
	}

	TemplateDecl
	template<size_t Size, const char * Name>
	constexpr TString & TString::operator+=(const string<Size, Name> & rhs) noexcept {
		return *this += rhs.c_str();
	}

	TemplateDecl
	constexpr TString & TString::operator+=(char rhs) noexcept {
		_buff[_size++] = rhs;
		assert(_size < MaxSize);
		_buff[_size] = 0;
		return *this;
	}

	TemplateDecl
	constexpr TString & TString::operator+=(int rhs) noexcept {
		_size += snprintf(_buff + _size, MaxSize - _size, "%d", rhs);
		return *this;
	}

	TemplateDecl
	constexpr TString & TString::operator+=(float rhs) noexcept {
		_size += snprintf(_buff + _size, MaxSize - _size, "%f", rhs);
		return *this;
	}

	TemplateDecl
	constexpr TString & TString::operator+=(size_t rhs) noexcept {
		_size += snprintf(_buff + _size, MaxSize - _size, "%zu", rhs);
		return *this;
	}

	TemplateDecl
	constexpr TString & TString::operator+=(unsigned int rhs) noexcept {
		_size += snprintf(_buff + _size, MaxSize - _size, "%u", rhs);
		return *this;
	}

	TemplateDecl
	constexpr TString & TString::operator+=(intptr_t rhs) noexcept {
		_size += snprintf(_buff + _size, MaxSize - _size, "%zu", rhs);
		return *this;
	}

	TemplateDecl
	template<typename T>
	constexpr TString TString::operator+(T rhs) const noexcept {
		string ret(_buff);
		ret += rhs;
		return ret;
	}

	TemplateDecl
	constexpr TString TString::substr(size_t start, size_t count) const noexcept {
		string ret;
		MY_STRNCPY(ret._buff, _buff + start, count);
		ret._size = std::min(strlen(_buff + start), count);
		return ret;
	}

	TemplateDecl
	constexpr void TString::clear() noexcept {
		_buff[0] = 0;
		_size = 0;
	}

	TemplateDecl
	constexpr size_t TString::find(const char * substr, size_t pos) const noexcept {
		assert(pos <= _size);

		const auto ret = strstr(_buff + pos, substr);
		if (ret == nullptr)
			return npos;
		return ret - _buff;
	}

	TemplateDecl
	constexpr size_t TString::find(char c, size_t pos) const noexcept {
		for (size_t i = pos; i < _size; ++i)
			if (_buff[i] == c)
				return i;
		return npos;
	}

	TemplateDecl
	constexpr size_t TString::find(std::string_view substr, size_t pos) const noexcept {
		for (; pos < _size; ++pos) {
			const auto remaining_size = _size - pos;
			if (strncmp(_buff + pos, substr.data(), std::min(remaining_size, substr.size())) == 0)
				return pos;
		}
		return npos;
	}

	TemplateDecl
	constexpr char TString::at(size_t i) const noexcept {
		return _buff[i];
	}

	TemplateDecl
	constexpr TString::operator std::string_view() const noexcept {
		return std::string_view(_buff);
	}

	TemplateDecl
	constexpr char TString::operator[](size_t i) const noexcept {
		return _buff[i];
	}

	TemplateDecl
	constexpr char & TString::operator[](size_t i) noexcept {
		return _buff[i];
	}

	TemplateDecl
	constexpr std::string TString::str() const noexcept {
		return _buff;
	}

	TemplateDecl
	constexpr const typename TString::buffer & TString::data() const noexcept {
		return _buff;
	}

	TemplateDecl
	constexpr const typename TString::buffer & TString::c_str() const noexcept {
		return _buff;
	}

	TemplateDecl
	constexpr size_t TString::size() const noexcept {
		return _size;
	}

	TemplateDecl
	constexpr size_t TString::length() const noexcept {
		return _size;
	}

	TemplateDecl
	constexpr bool TString::empty() const noexcept {
		return _size == 0;
	}

	TemplateDecl
	constexpr bool TString::full() const noexcept {
		return _size >= MaxSize - 1;
	}

	TemplateDecl
	constexpr const char * TString::begin() const noexcept {
		return _buff;
	}

	TemplateDecl
	constexpr const char * TString::end() const noexcept {
		return _buff + _size;
	}

	TemplateDecl
	constexpr char * TString::begin() noexcept {
		return _buff;
	}

	TemplateDecl
	constexpr char * TString::end() noexcept {
		return _buff + _size;
	}

	TemplateDecl
	constexpr TString::operator std::span<const char>() const noexcept {
		return { _buff, _size };
	}

	TemplateDecl
	constexpr TString::operator std::span<char>() noexcept {
		return { _buff, _size };
	}

	TemplateDecl
	constexpr auto begin(const TString & s) noexcept {
		return s.begin();
	}

	TemplateDecl
	constexpr auto end(const TString & s) noexcept {
		return s.end();
	}

	TemplateDecl
	constexpr bool operator==(const TString & lhs, const char * rhs) noexcept {
		return strcmp(lhs.c_str(), rhs) == 0;
	}

	TemplateDecl
	constexpr bool operator!=(const TString & lhs, const char * rhs) noexcept {
		return strcmp(lhs.c_str(), rhs) != 0;
	}

	TemplateDecl
	constexpr bool operator==(const TString & lhs, const std::string & rhs) noexcept {
		return lhs == rhs.c_str();
	}

	TemplateDecl
	constexpr bool operator!=(const TString & lhs, const std::string & rhs) noexcept {
		return lhs != rhs.c_str();
	}

	TemplateDecl
	constexpr bool operator==(const TString & lhs, const TString & rhs) noexcept {
		return lhs == rhs.c_str();
	}

	template<size_t S1, const char * N1, size_t S2, const char * N2>
	constexpr bool operator==(const string<S1, N1> & lhs, const string<S2, N2> & rhs) noexcept {
		return lhs == rhs.c_str();
	}

	template<size_t S1, const char * N1, size_t S2, const char * N2>
	constexpr bool operator!=(const string<S1, N1> & lhs, const string<S2, N2> & rhs) noexcept {
		return lhs != rhs.c_str();
	}

	TemplateDecl
	constexpr bool operator!=(const TString & lhs, const TString & rhs) noexcept {
		return lhs != rhs.c_str();
	}

	TemplateDecl
	constexpr inline bool operator==(const char * lhs, const TString & rhs) noexcept {
		return rhs == lhs;
	}

	TemplateDecl
	constexpr inline bool operator!=(const char * lhs, const TString & rhs) noexcept {
		return rhs != lhs;
	}

	TemplateDecl
	constexpr inline bool operator==(const std::string & lhs, const TString & rhs) noexcept {
		return rhs == lhs;
	}

	TemplateDecl
	constexpr inline bool operator!=(const std::string & lhs, const TString & rhs) noexcept {
		return rhs != lhs;
	}

	TemplateDecl
	constexpr std::ostream & operator<<(std::ostream & s, const TString & str) noexcept {
		return s << str.c_str();
	}
}

namespace std {
	TemplateDecl
	constexpr std::size_t hash<putils::TString>::operator()(const putils::TString & s) const noexcept {
		size_t result = 0;
		constexpr auto prime = 31;
		for (const auto c : s)
			result = c + (result * prime);
		return result;
	}
}

#undef MY_STRNCPY
#undef MY_STRNCAT

// clang-format on
#undef TemplateDecl
#undef TString
