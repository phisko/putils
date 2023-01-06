#include "plugin_manager.hpp"

// stl
#include <filesystem>
#include <string_view>

// putils
#include "library_factory.hpp"
#include "putils/profiling.hpp"

// meta
#include "putils/meta/fwd.hpp"

namespace putils {
	template<typename... P>
	void plugin_manager::rescan_directory(const char * path, const char * to_execute, P &&... params) noexcept {
		PUTILS_PROFILING_SCOPE;

#ifdef _WIN32
		static constexpr auto extension = ".dll";
#else
		static constexpr auto extension = ".so";
#endif

		for (const auto & entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_directory())
				continue;

			if (entry.path().extension() != extension)
				continue;

			const auto library_path = entry.path().string();
			if (_libraries.find(library_path) != _libraries.end())
				continue;

			auto plugin = library_factory::make(library_path.c_str());
			_libraries[library_path] = plugin;
			if (to_execute != nullptr) {
				const auto func = plugin->load_method<void, P...>(to_execute);
				if (func != nullptr)
					func(FWD(params)...);
			}
		}
	}

	template<size_t MaxReturns, typename T, typename... P>
	putils::vector<T, MaxReturns> plugin_manager::rescan_directory_with_return(const char * path, const char * to_execute, P &&... params) noexcept {
		PUTILS_PROFILING_SCOPE;

#ifdef _WIN32
		static constexpr auto extension = ".dll";
#else
		static constexpr auto extension = ".so";
#endif
		putils::vector<T, MaxReturns> ret;

		for (const auto & entry : std::filesystem::directory_iterator(path)) {
			if (ret.full())
				break;

			if (entry.is_directory())
				continue;

			if (entry.path().extension() != extension)
				continue;

			const auto library_path = entry.path().string();
			if (_libraries.find(library_path) != _libraries.end())
				continue;

			const auto plugin = library_factory::make(library_path.c_str());
			_libraries[library_path] = plugin;
			if (to_execute != nullptr) {
				const auto func = plugin->load_method<T, P...>(to_execute);
				if (func != nullptr)
					ret.push_back(func(FWD(params)...));
			}
		}

		return ret;
	}

	template<typename... P>
	void plugin_manager::execute(const std::string & name, P &&... params) noexcept {
		PUTILS_PROFILING_SCOPE;

		for (const auto & [_, plugin] : _libraries) {
			const auto func = plugin->template load_method<void, P...>(name);
			if (func != nullptr)
				func(FWD(params)...);
		}
	}

	template<size_t MaxReturns, typename T, typename... P>
	putils::vector<T, MaxReturns> plugin_manager::execute_with_return(const std::string & name, P &&... params) noexcept {
		PUTILS_PROFILING_SCOPE;

		putils::vector<T, MaxReturns> ret;

		for (const auto & [_, plugin] : _libraries) {
			const auto func = plugin->template load_method<T, P...>(name);
			if (func != nullptr) {
				ret.push_back(func(FWD(params)...));
				if (ret.full())
					return ret;
			}
		}

		return ret;
	}
}
