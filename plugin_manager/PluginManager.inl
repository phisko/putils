#include "PluginManager.hpp"

// stl
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

		putils::Directory d(path);
		d.for_each([&](const putils::Directory::File & f) noexcept {
			const std::string_view view = f.fullPath;

            if (f.isDirectory)
                return;

            const auto dotPos = view.find_last_of('.');
            if (dotPos == std::string::npos)
                return;

			const auto fileExt = view.substr(dotPos);
            if (fileExt != extension)
                return;

            if (_libraries.find(f.fullPath) != _libraries.end())
                return;

            auto plugin = LibraryFactory::make(f.fullPath.c_str());
            _libraries[f.fullPath.c_str()] = plugin;
            if (toExecute != nullptr) {
                const auto func = plugin->loadMethod<void, P...>(toExecute);
                if (func != nullptr)
                    func(FWD(params)...);
            }
		});
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

		putils::Directory d(path);
		d.for_each([&](const putils::Directory::File & f) noexcept {
			if (ret.full())
				return;

			const std::string_view view = f.fullPath;
            if (f.isDirectory)
                return;

            const auto dotPos = view.find_last_of('.');
            if (dotPos == std::string::npos)
                return;

			const auto fileExt = view.substr(dotPos);
            if (fileExt != extension)
                return;

            if (_libraries.find(f.fullPath) != _libraries.end())
                return;

            auto plugin = LibraryFactory::make(f.fullPath.c_str());
            _libraries[f.fullPath.c_str()] = plugin;
            if (toExecute != nullptr) {
                const auto func = plugin->loadMethod<T, P...>(toExecute);
                if (func != nullptr)
                    ret.push_back(func(FWD(params)...));
            }
        });

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
