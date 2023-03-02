#include "library_factory.hpp"

// stl
#include <regex>

// putils
#include "putils/string.hpp"
#include "putils/profiling.hpp"

namespace putils::library_factory {
	using string = putils::string<PUTILS_LIBRARY_MAX_PATH>;

	template<typename Str1, typename Str2>
	string add_lib_to_path(Str1 && name, Str2 && extension) noexcept {
		PUTILS_PROFILING_SCOPE;

		static const std::regex end((string("^.*\\") + extension + "$").c_str());

		if (std::regex_match(name, end))
			return name;

		string path;
		string lib = name;

		const auto it = std::string_view(name).find_last_of("/");
		if (it != std::string_view::npos) {
			path = name;
			path[it + 1] = 0;
			lib = std::string_view(name).substr(it + 1);
		}

		return path + "lib" + lib + extension;
	}

	putils::library * make(const char * name) noexcept {
		PUTILS_PROFILING_SCOPE;

		using string = putils::string<1024>;

		static std::unordered_map<string, std::unique_ptr<library>> _register;

		if (_register.find(name) != _register.end())
			return _register.at(name).get();

#ifdef MINGW
		auto lib = std::make_unique<windows_library>(add_lib_to_path(name, ".dll"));
		_register.emplace(name, std::move(lib));
#elif defined(_WIN32)
		static const std::regex end(R"(^.*\.dll$)");

		const auto to_load = std::regex_match(name, end) ? string(name) : string(name) + ".dll";
		auto lib = std::make_unique<windows_library>(to_load);
		_register.emplace(name, std::move(lib));
#elif defined(__unix__)
		auto lib = std::make_unique<unix_library>(add_lib_to_path(name, ".so"));
		_register.emplace(name, std::move(lib));
#endif

		return _register.at(name).get();
	}
}
