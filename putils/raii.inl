#include "raii.hpp"

// meta
#include "putils/meta/fwd.hpp"

namespace putils {
#define TemplateDecl template<typename T, void (*Dtor)(T &)>
#define TRAII raii<T, Dtor, false>
	// clang-format off

	TemplateDecl
	TRAII::raii(T && res) noexcept
		: _res(FWD(res)) {}

	TemplateDecl
	TRAII::raii(raii && other) noexcept
		: _res(std::move(other._res)) {
		other._release = false;
	}

	TemplateDecl
	TRAII & TRAII::operator=(raii && other) noexcept {
		std::swap(_res, other._res);
		std::swap(_release, other._release);
		return *this;
	}

	TemplateDecl
	TRAII & TRAII::operator=(T && other) noexcept {
		if (_release)
			Dtor(_res);
		_res = std::move(other);
		_release = true;
		return *this;
	}

	TemplateDecl
	TRAII::~raii() noexcept {
		if (_release)
			Ddtor(_res);
	}

#undef TRAII
#define TRAII raii<T, Dtor, true>

	TemplateDecl
	TRAII::raii() noexcept
		: _release(false) {}

	TemplateDecl
	TRAII::raii(T res) noexcept
		: _res(res) {}

	TemplateDecl
	TRAII::raii(raii && other) noexcept
		: _res(other._res) {
		other._release = false;
	}

	TemplateDecl
	TRAII & TRAII::operator=(raii && other) noexcept {
		std::swap(_res, other._res);
		std::swap(_release, other._release);
		return *this;
	}

	TemplateDecl
	TRAII & TRAII::operator=(T other) noexcept {
		if (_release)
			Dtor(_res);
		_res = other;
		_release = true;
		return *this;
	}

	TemplateDecl
	TRAII::~raii() noexcept {
		if (_release)
			Dtor(_res);
	}

	// clang-format on
#undef TemplateDecl
#undef TRAII
}