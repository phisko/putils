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

   public:
        // In each plugin, execute the [name] function, taking P as parameter
        template<typename ...P>
        void execute(const std::string & name, P && ...params) noexcept;

        // In each plugin, execute the [name] function, returning a T and taking a P as parameter
        // Returns a vector of all values returned
        template<size_t MaxReturns, typename T, typename ...P>
        putils::vector<T, MaxReturns> executeWithReturn(const std::string & name, P && ...params) noexcept;

        // Same as execute, but only in libraries which have been added to the directory since we last scanned
		template<typename ...P>
        void rescanDirectory(const char * path, const char * toExecute, P && ...params) noexcept;

		template<size_t MaxReturns, typename T, typename ...P>
        putils::vector<T, MaxReturns> rescanDirectoryWithReturn(const char * path, const char * toExecute, P && ...params) noexcept;
 

    private:
        std::unordered_map<putils::string<PUTILS_DIRECTORY_MAX_PATH>, Library *> _libraries;
    };
}

#include "PluginManager.inl"
