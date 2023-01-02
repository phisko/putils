#pragma once

// stl
#include <string>
#include <unordered_map>
#include <iostream>

namespace putils {
	//
	// Interface for shared libraries
	//
	class PUTILS_PLUGIN_MANAGER_EXPORT library {
	public:
		template<typename String>
		library(String && name) noexcept;
		virtual ~library() noexcept = default;

	protected:
		virtual void * load_symbol(const std::string & name) noexcept = 0;

		// Return a pointer to the [name] function of the library, returning a T and taking P as parameters
		// Uses pseudo-flyweight to avoid reloading symbols
	public:
		template<typename T, typename... P>
		using func = T (*)(P...);

		template<typename T, typename... P>
		func<T, P...> load_method(const std::string & name) noexcept;

		// Executes the [name] function of the library, returning a T and taking P as parameters
	public:
		template<typename T, typename... P>
		T execute(const std::string & name, P &&... args) noexcept;

		// Name getter
	public:
		std::string_view get_name() const noexcept { return _name; }

		// Attributes
	private:
		std::string _name;
		std::unordered_map<std::string, void *> _symbols;
	};
}

#include "library.inl"
