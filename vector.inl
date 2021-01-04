#include "vector.hpp"

#define TemplateDecl template<typename T, size_t MaxSize, const char * ClassName>
#define TVector vector<T, MaxSize, ClassName>

namespace putils {

	TemplateDecl
	template<size_t N>
	constexpr TVector::vector(const T(&arr)[N]) noexcept {
		for (const T & elem : arr)
			_buff[_size++] = elem;
	}

	TemplateDecl
	constexpr TVector::vector(std::initializer_list<T> arr) noexcept {
		for (const T & elem : arr)
			_buff[_size++] = elem;
	}

	TemplateDecl
	template<typename Val>
	constexpr T & TVector::push_back(Val && val) noexcept {
		assert(!full());
		_buff[_size] = FWD(val);
		return _buff[_size++];
	}

	TemplateDecl
	template<typename Val>
	constexpr void TVector::try_push_back(Val && val) noexcept {
		if (!full())
			push_back(FWD(val));
	}

	TemplateDecl
	template<typename ... Args>
	constexpr T & TVector::emplace_back(Args && ...args) noexcept {
		assert(!full());
		_buff[_size] = T(FWD(args)...);
		return _buff[_size++];
	}

	TemplateDecl
	template<typename ... Args>
	constexpr void TVector::try_emplace_back(Args && ...args) noexcept {
		if (!full())
			emplace_back(FWD(args)...);
	}

	TemplateDecl
	constexpr T & TVector::add(const T & val) noexcept {
		return push_back(val); 
	}

	TemplateDecl
	constexpr void TVector::try_add(const T & val) noexcept {
		try_push_back(val); 
	}

	TemplateDecl
	constexpr T & TVector::back() noexcept {
		assert(_size > 0); return _buff[_size - 1];
	}

	TemplateDecl
	constexpr T & TVector::operator[](size_t index) noexcept {
		assert(index < _size);
		return _buff[index];
	}

	TemplateDecl
	constexpr const T & TVector::operator[](size_t index) const noexcept {
		assert(index < _size);
		return _buff[index];
	}

	TemplateDecl
	constexpr T & TVector::at(size_t index) noexcept {
		return (*this)[index]; 
	}

	TemplateDecl
	constexpr const T & TVector::at(size_t index) const noexcept {
		return (*this)[index]; 
	}

	TemplateDecl
		constexpr T & TVector::get(size_t index) noexcept {
		return at(index); 
	}

	TemplateDecl
	constexpr void TVector::erase(T * ptr) noexcept {
		assert(ptr > _buff && ptr < _buff + _size);
		std::swap(*ptr, back());
		--_size;
	}

	TemplateDecl
	constexpr void TVector::erase(T * start, T * until) noexcept {
		assert(start > _buff && until <= _buff + _size);
		while (start < until) {
			std::swap(*start, back());
			--_size;
			++start;
		}
	}

	TemplateDecl
	constexpr void TVector::remove(T * ptr) noexcept {
		erase(ptr); 
	}

	TemplateDecl
	constexpr auto TVector::size() const noexcept {
		return _size; 
	}

	TemplateDecl
	constexpr bool TVector::empty() const noexcept {
		return _size == 0; 
	}

	TemplateDecl
	constexpr bool TVector::full() const {
		return _size >= MaxSize; 
	}

	TemplateDecl
	constexpr auto TVector::begin() const noexcept {
		return _buff; 
	}

	TemplateDecl
	constexpr auto TVector::end() const noexcept {
		return _buff + _size; 
	}

	TemplateDecl
	constexpr auto TVector::begin() noexcept {
		return _buff; 
	}

	TemplateDecl
	constexpr auto TVector::end() noexcept {
		return _buff + _size; 
	}

	TemplateDecl
	constexpr auto TVector::data() const noexcept {
		return _buff; 
	}

	TemplateDecl
	constexpr auto TVector::data() noexcept {
		return _buff; 
	}

	TemplateDecl
	constexpr void TVector::clear() noexcept {
		_size = 0; 
	}

	TemplateDecl
	constexpr void TVector::resize(size_t size) noexcept {
		_size = size; 
	}

	TemplateDecl
	constexpr size_t TVector::capacity() const noexcept {
		return MaxSize; 
	}

	TemplateDecl
	constexpr TVector::operator std::span<const T>() const noexcept {
		return { _buff, _size }; 
	}

	TemplateDecl
	constexpr TVector::operator std::span<T>() noexcept {
		return { _buff, _size }; 
	}

	TemplateDecl
	constexpr auto begin(const TVector & v) noexcept { return v.begin(); }

	TemplateDecl
	constexpr auto end(const TVector & v) noexcept { return v.end(); }

	template<typename ... Args>
	constexpr auto make_vector(Args && ... args) noexcept -> vector<std::common_type_t<Args...>, sizeof...(Args)> {
		return { FWD(args)... };
	}
}

#undef TemplateDecl
#undef TVector