#pragma once

#include <regex>

#include "string.hpp"
#include "WindowsLibrary.hpp"
#include "UnixLibrary.hpp"

#ifndef PUTILS_LIBRARY_MAX_PATH
# define PUTILS_LIBRARY_MAX_PATH 256
#endif

namespace putils {
    namespace {
		using string = putils::string<PUTILS_LIBRARY_MAX_PATH>;

        template<typename Str1, typename Str2>
        string addLibToPath(Str1 &&name, Str2 &&extension) noexcept {
            static std::regex end(string("^.*\\") + extension + "$");

            if (!std::regex_match(name, end))
            {
                string path;
                string lib = name;

                auto it = std::string_view(name).find_last_of("/");
                if (it != std::string_view::npos)
                {
					path = name;
					path[it + 1] = 0;
                    lib = std::string_view(name).substr(it + 1);
                }
                return path + "lib" + lib + extension;
            }
            else
                return name;
        }
    }

    namespace LibraryFactory
    {
        inline putils::Library *make(const char * name) {
			using string = putils::string<PUTILS_LIBRARY_MAX_PATH>;

            static std::unordered_map<string, std::unique_ptr<Library>> _register;

            if (_register.find(name) != _register.end())
                return _register.at(name).get();

#ifdef MINGW
            auto lib = std::make_unique<WindowsLibrary>(addLibToPath(name, ".dll"));
            _register.emplace(name, std::move(lib));
#elif defined(_WIN32)
            static std::regex		end(R"(^.*\.dll$)");

            const auto toLoad = std::regex_match(name, end) ? std::string(name) : string(name) + ".dll";
            auto lib = std::make_unique<WindowsLibrary>(toLoad);
            _register.emplace(name, std::move(lib));
#elif defined(__unix__)
            auto lib = std::make_unique<UnixLibrary>(addLibToPath(name, ".so"));
            _register.emplace(name, std::move(lib));
#endif

            return _register.at(name).get();
        }
    }
}
