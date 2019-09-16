#pragma once

#include <string_view>
#include <vector>
#include <regex>
#include <iostream>
#include "Directory.hpp"
#include "Library.hpp"
#include "LibraryFactory.hpp"
#include "string.hpp"
#include "vector.hpp"

namespace putils {
    //
    // PluginManager which either loads all libraries in a folder
    // or loads libraries contained in a CSV config file
    // Offers functions to execute a given function in all libraries,
    // in all libraries of certain categories, with or without
    // return values
    //
    class PluginManager {
        // Constructor
    public:
		PluginManager() = default;
        PluginManager(const PluginManager &) = delete;
        PluginManager & operator=(const PluginManager &) = delete;

		template<typename ...P>
		void rescanDirectory(const char * path, const char * toExecute, P && ...params) noexcept {
#ifdef _WIN32
			static constexpr auto extension = ".dll";
#else
			static constexpr auto extension = ".so";
#endif

			putils::Directory d(path);
			d.for_each([&](const putils::Directory::File & f) {
				std::string_view view = f.fullPath;
				const auto fileExt = view.substr(view.find_last_of('.'));

				if (!f.isDirectory && fileExt == extension && _libraries.find(f.fullPath) == _libraries.end()) {
					auto plugin = LibraryFactory::make(f.fullPath);
					_libraries[f.fullPath] = plugin;
					if (toExecute != nullptr) {
						const auto func = plugin->loadMethod<void, P...>(toExecute);
						if (func != nullptr)
							func(FWD(params)...);
					}
				}
			});
		}

		template<size_t MaxReturns, typename T, typename ...P>
		putils::vector<T, MaxReturns> rescanDirectoryWithReturn(const char * path, const char * toExecute, P && ...params) noexcept {
#ifdef _WIN32
			static constexpr auto extension = ".dll";
#else
			static constexpr auto extension = ".so";
#endif
			putils::vector<T, MaxReturns> ret;

			putils::Directory d(path);
			d.for_each([&](const putils::Directory::File & f) {
				if (ret.full())
					return;

				std::string_view view = f.fullPath;
				const auto fileExt = view.substr(view.find_last_of('.'));

				if (!f.isDirectory && fileExt == extension && _libraries.find(f.fullPath) == _libraries.end()) {
					auto plugin = LibraryFactory::make(f.fullPath);
					_libraries[f.fullPath] = plugin;
					if (toExecute != nullptr) {
						const auto func = plugin->loadMethod<T, P...>(toExecute);
						if (func != nullptr)
							ret.push_back(func(FWD(params)...));
					}
				}
			});

			return ret;
		}

    public:
        // In each plugin, execute the [name] function, taking P as parameter
        template<typename ...P>
        void execute(const std::string & name, P && ...params) noexcept {
			for (const auto &[_, plugin] : _libraries) {
                auto func = plugin->loadMethod<void, P...>(name);
                if (func != nullptr)
                    func(std::forward<P>(params)...);
			}
        }

        // In each plugin, execute the [name] function, returning a T and taking a P as parameter
        // Returns a vector of all values returned
        template<size_t MaxReturns, typename T, typename ...P>
        putils::vector<T, MaxReturns> executeWithReturn(const std::string & name, P && ...params) noexcept {
            putils::vector<T, MaxReturns> ret;

            for (const auto & [_, plugin] : _libraries) {
                auto func = plugin->loadMethod<T, P...>(name);
				if (func != nullptr) {
                    ret.push_back(func(std::forward<P>(params)...));
					if (ret.full())
						return;
				}
            }

            return ret;
        }

    private:
        std::unordered_map<putils::string<PUTILS_DIRECTORY_MAX_PATH>, Library *> _libraries;
    };
}
