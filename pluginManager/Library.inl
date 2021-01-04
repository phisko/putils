#include "Library.hpp"

namespace putils {
	template<typename String>
	Library::Library(String && name) noexcept 
		: _name(std::forward<String>(name))
	{}

	template<typename T, typename ...P>
	Library::Func<T, P...> Library::loadMethod(const std::string & name) noexcept {
		if (_symbols.find(name) == _symbols.end()) {
			void * symbol = loadSymbol(name);
			if (!symbol)
				return NULL;
			_symbols[name] = symbol;
		}

		auto ret = reinterpret_cast<T(*)(P ...)>(_symbols[name]);
		if (ret == nullptr)
			std::cerr << _name << ": Failed to load method '" << name << "'" << std::endl;

		return ret;
	}

	template<typename T, typename ...P>
	T Library::execute(const std::string & name, P && ...args) noexcept {
		return (loadMethod<T, P...>(name))(std::forward<P>(args)...);
	}


}
