#pragma once

#include <assert.h>
#include "reflection.hpp"
#include "anonymous_variable.hpp"
#include "fwd.hpp"

namespace putils {
	namespace detail_vector {
		static constexpr char defaultClassName[] = "putils_vector";
	}

	template<typename T, size_t MaxSize, const char * ClassName = detail_vector::defaultClassName>
	class vector {
	public:
		using value_type = T;
		using size_type = size_t;
		using reference = T & ;
		using iterator = T * ;
		using const_iterator = const T *;

	public:
		vector() = default;

		template<size_t N>
		vector(const T(&arr)[N]) {
			for (const T & elem : arr)
				_buff[_size++] = elem;
		}

		vector(std::initializer_list<T> arr) {
			for (const T & elem : arr)
				_buff[_size++] = elem;
		}

		template<typename Val>
		T & push_back(Val && val) {
			assert(!full());
			_buff[_size] = FWD(val);
			return _buff[_size++];
		}

		template<typename Val>
		void try_push_back(Val && val) {
			if (!full())
				push_back(FWD(val));
		}

		template<typename ... Args>
		T & emplace_back(Args && ...args) {
			assert(!full());
			_buff[_size] = T(FWD(args)...);
			return _buff[_size++];
		}

		template<typename ... Args>
		void try_emplace_back(Args && ...args) {
			if (!full())
				emplace_back(FWD(args)...);
		}

		// Reflectible (no templates/overloads)
		T & add(const T & val) { return push_back(val); }
		void try_add(const T & val) { try_push_back(val); }

		T & back() { assert(_size > 0); return _buff[_size - 1]; }

		T & operator[](size_t index) {
			assert(index < _size);
			return _buff[index];
		}
		T & at(size_t index) { return (*this)[index]; }

		const T & operator[](size_t index) const {
			assert(index < _size);
			return _buff[index];
		}
		const T & at(size_t index) const { return (*this)[index]; }

		// Reflectible (no templates/overloads)
		T & get(size_t index) { return at(index); }

		void erase(T * ptr) {
			assert(ptr > _buff && ptr < _buff + _size);
			std::swap(*ptr, back());
			--_size;
		}

		void erase(T * start, T * until) {
			assert(start > _buff && until <= _buff + _size);
			while (start < until) {
				std::swap(*start, back());
				--_size;
				++start;
			}
		}

		// Reflectible (no templates/overloads)
		void remove(T * ptr) { erase(ptr); }

		auto size() const { return _size; }
		bool empty() const { return _size == 0; }
		bool full() const { return _size >= MaxSize; }

		auto begin() const { return _buff; }
		auto end() const { return _buff + _size; }

		auto begin() { return _buff; }
		auto end() { return _buff + _size; }

		auto data() const { return _buff; }
		auto data() { return _buff; }

		void clear() { _size = 0; }
		void resize(size_t size) { _size = size; }
		size_t capacity() const { return MaxSize; }

	private:
		T _buff[MaxSize] = {};
		size_t _size = 0;
	};

	template<typename T, size_t Size, const char * Name>
	auto begin(const vector<T, Size, Name> & v) { return v.begin(); }
	template<typename T, size_t Size, const char * Name>
	auto end(const vector<T, Size, Name> & v) { return v.end(); }

	template<typename ... Args>
	auto make_vector(Args && ... args) -> vector<std::common_type_t<Args...>, sizeof...(Args)> {
		return { FWD(args)... };
	}

	template<typename>
	struct is_vector : std::false_type {};

	template<typename T, size_t Size, const char * Name>
	struct is_vector<putils::vector<T, Size, Name>> : std::true_type {};
}

template<typename T, size_t MaxSize, const char * ClassName>
#define refltype putils::vector<T, MaxSize, ClassName>
putils_reflection_info_template{
	static constexpr auto class_name = ClassName;
	putils_reflection_methods(
		putils_reflection_attribute(add),
		putils_reflection_attribute(try_add),
		putils_reflection_attribute(back),
		putils_reflection_attribute(get),
		putils_reflection_attribute(remove),
		putils_reflection_attribute(size),
		putils_reflection_attribute(empty),
		putils_reflection_attribute(full),
		putils_reflection_attribute(clear)
	);
};
#undef refltype
