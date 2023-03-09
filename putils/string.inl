#include "string.hpp"

// stl
#include <cstring>
#include <cassert>

#define TemplateDecl template<size_t MaxSize, const char * ClassName>
#define TString string<MaxSize, ClassName>
// clang-format off

namespace putils {
	TemplateDecl
	constexpr TString::string(std::string_view str) noexcept {
		assign(str);
	}

	TemplateDecl
	constexpr TString::string(const char * str) noexcept {
		assign(str);
	}

	TemplateDecl
	constexpr TString & TString::operator=(std::string_view str) noexcept {
		assign(str);
		return *this;
	}

	TemplateDecl
	constexpr TString & TString::operator=(const char * str) noexcept {
		assign(str);
		return *this;
	}

	TemplateDecl
	template<typename... FmtArgs>
	constexpr TString::string(fmt::format_string<FmtArgs...> format, FmtArgs &&... fmt_args) noexcept {
		set(format, FWD(fmt_args)...);
	}

	TemplateDecl
	template<typename... FmtArgs>
	constexpr void TString::set(fmt::format_string<FmtArgs...> format, FmtArgs &&... fmt_args) noexcept {
		_size = fmt::format_to_n(_buff, MaxSize - 1, format, FWD(fmt_args)...).size;
		_size = std::min(_size, MaxSize - 1);
		_buff[_size] = 0;
	}

	TemplateDecl
	constexpr void TString::assign(std::string_view str) noexcept {
		set("{}", str);
	}

	TemplateDecl
	template<typename T>
	constexpr TString & TString::operator+=(const T & rhs) noexcept {
		const auto dest = _buff + _size;
		_size += fmt::format_to_n(dest, MaxSize - _size - 1, "{}", rhs).size;
		_buff[_size] = 0;
		return *this;
	}

	TemplateDecl
	template<typename T>
	constexpr TString TString::operator+(const T & rhs) const noexcept {
		string ret(_buff);
		ret += rhs;
		return ret;
	}

	TemplateDecl
	constexpr TString TString::substr(size_t start, size_t count) const noexcept {
		string ret;
		ret._size = fmt::format_to_n(ret._buff, std::min(MaxSize - 1, count), "{}", _buff + start).size;
		ret._size = std::min(_size, MaxSize - 1);
		ret._buff[ret._size] = 0;
		return ret;
	}

	TemplateDecl
	constexpr void TString::clear() noexcept {
		_size = 0;
		_buff[_size] = 0;
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
	constexpr size_t TString::max_size() const noexcept {
		return MaxSize - 1;
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

// clang-format on
#undef TemplateDecl
#undef TString
