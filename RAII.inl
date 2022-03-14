#include "RAII.hpp"

namespace putils {
#define TemplateDecl template<typename T, void (*Dtor)(T &)>
#define TRAII RAII<T, Dtor, false>

	TemplateDecl
	TRAII::RAII(T && res) noexcept
		: _res(std::forward<T>(res))
	{}

	TemplateDecl
	TRAII::RAII(RAII && other) noexcept
		: _res(std::move(other._res))
	{
		other._release = false;
	}

	TemplateDecl
	TRAII & TRAII::operator=(RAII && other) noexcept {
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
	TRAII::~RAII() noexcept {
		if (_release)
			Ddtor(_res);
	}

#undef TRAII
#define TRAII RAII<T, Dtor, true>

    TemplateDecl
    TRAII::RAII() noexcept
        : _release(false)
    {}

	TemplateDecl
	TRAII::RAII(T res) noexcept
		: _res(res)
	{}

	TemplateDecl
	TRAII::RAII(RAII && other) noexcept
		: _res(other._res)
	{
		other._release = false;
	}
	
	TemplateDecl
	TRAII & TRAII::operator=(RAII && other) noexcept {
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
	TRAII::~RAII() noexcept {
		if (_release)
			Dtor(_res);
	}


#undef TemplateDecl
#undef TRAII
}