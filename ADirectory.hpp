#pragma once

// stl
#include <vector>

// putils
#include "string.hpp"
#include "putils_profiling.hpp"

#ifndef PUTILS_DIRECTORY_MAX_PATH
# define PUTILS_DIRECTORY_MAX_PATH 256
#endif

namespace putils {
    class PUTILS_CORE_EXPORT ADirectory {
        // File structure
    public:
		using string = putils::string<PUTILS_DIRECTORY_MAX_PATH>;

        struct File {
            string name;
            string fullPath;
            bool isDirectory = false;
            bool isLink = false;
        };

        // Destructor
        virtual ~ADirectory() noexcept = default;

        // Read the next entry in the directory
        virtual bool getNextFile(File & out) noexcept = 0;

        // Apply a function to each file in the directory
		template<typename Func> // Func: void(const File &)
        void for_each(Func && func) noexcept {
			PUTILS_PROFILING_SCOPE;

			File f;
            while (getNextFile(f))
                func(f);
        }

        // Get a list of all the files
        std::vector<File> getFiles() noexcept {
			PUTILS_PROFILING_SCOPE;

            std::vector<File> ret;

            for_each([&ret](const File & f) noexcept {
                ret.push_back(std::move(f));
            });

            return ret;
        }
    };
}