#include "library.hpp"

// putils
#include "putils/profiling.hpp"

// meta
#include "putils/meta/fwd.hpp"

namespace putils {
	template<typename String>
	library::library(String && name) noexcept
		: _name(FWD(name)) {}

	template<typename T, typename... P>
	library::func<T, P...> library::load_method(const std::string & name) noexcept {
		PUTILS_PROFILING_SCOPE;

		if (_symbols.find(name) == _symbols.end()) {
			void * symbol = load_symbol(name);
			if (!symbol)
				return NULL;
			_symbols[name] = symbol;
		}

		const auto ret = reinterpret_cast<T (*)(P...)>(_symbols[name]);
		if (ret == nullptr)
			std::cerr << _name << ": Failed to load method '" << name << "'" << std::endl;

		return ret;
	}

	template<typename T, typename... P>
	T library::execute(const std::string & name, P &&... args) noexcept {
		PUTILS_PROFILING_SCOPE;
		return (load_method<T, P...>(name))(FWD(args)...);
	}
}
