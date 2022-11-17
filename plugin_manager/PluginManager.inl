#include "PluginManager.hpp"

// stl
#include <filesystem>
#include <string_view>

// putils
#include "LibraryFactory.hpp"
#include "putils_profiling.hpp"

namespace putils {
	template<typename ...P>
	void PluginManager::rescanDirectory(const char * path, const char * toExecute, P && ...params) noexcept {
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

			const auto libraryPath = entry.path().string();
			if (_libraries.find(libraryPath) != _libraries.end())
				continue;

			auto plugin = LibraryFactory::make(libraryPath.c_str());
			_libraries[libraryPath] = plugin;
			if (toExecute != nullptr) {
				const auto func = plugin->loadMethod<void, P...>(toExecute);
				if (func != nullptr)
					func(FWD(params)...);
			}
		}
	}

	template<size_t MaxReturns, typename T, typename ...P>
	putils::vector<T, MaxReturns> PluginManager::rescanDirectoryWithReturn(const char * path, const char * toExecute, P && ...params) noexcept {
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

			const auto libraryPath = entry.path().string();
            if (_libraries.find(libraryPath) != _libraries.end())
                continue;

            auto plugin = LibraryFactory::make(libraryPath.c_str());
            _libraries[libraryPath] = plugin;
            if (toExecute != nullptr) {
                const auto func = plugin->loadMethod<T, P...>(toExecute);
                if (func != nullptr)
                    ret.push_back(func(FWD(params)...));
            }
        }

		return ret;
	}

	template<typename ...P>
	void PluginManager::execute(const std::string & name, P && ...params) noexcept {
		PUTILS_PROFILING_SCOPE;

		for (const auto & [_, plugin] : _libraries) {
			auto func = plugin->template loadMethod<void, P...>(name);
			if (func != nullptr)
				func(std::forward<P>(params)...);
		}
	}

	template<size_t MaxReturns, typename T, typename ...P>
	putils::vector<T, MaxReturns> PluginManager::executeWithReturn(const std::string & name, P && ...params) noexcept {
		PUTILS_PROFILING_SCOPE;

		putils::vector<T, MaxReturns> ret;

		for (const auto & [_, plugin] : _libraries) {
			auto func = plugin->template loadMethod<T, P...>(name);
			if (func != nullptr) {
				ret.push_back(func(std::forward<P>(params)...));
				if (ret.full())
					return ret;
			}
		}

		return ret;
	}
}
