#pragma once

// stl
#include <type_traits>
#include <utility>

// meta
#include "putils/meta/fwd.hpp"

namespace putils {
	template<typename T, void (*Dtor)(T &), bool = std::is_scalar<T>()>
	class raii;

	//
	// raii template
	// Create one by giving it a resource and a function to release that resource
	//
	template<typename T, void (*Dtor)(T &)>
	class raii<T, Dtor, false> {
	public:
		explicit raii(T && res) noexcept;
		raii(raii && other) noexcept;
		raii & operator=(raii && other) noexcept;
		raii & operator=(T && other) noexcept;
		~raii() noexcept;

	public:
		T & get() noexcept { return _res; }
		const T & get() const noexcept { return _res; }

	private:
		T _res;
		bool _release{ true };
	};

	template<typename T, void (*Dtor)(T &)>
	class raii<T, Dtor, true> {
	public:
		raii() noexcept;
		explicit raii(T res) noexcept;
		raii(raii && other) noexcept;
		raii & operator=(raii && other) noexcept;
		raii & operator=(T other) noexcept;

		~raii() noexcept;

		raii(const raii &) = delete;
		raii & operator=(const raii &) = delete;

	public:
		T & get() noexcept { return _res; }
		const T & get() const noexcept { return _res; }

	private:
		T _res;
		bool _release{ true };
	};
}

#include "raii.inl"
