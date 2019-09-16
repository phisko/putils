#pragma once

#include "WindowsDirectory.hpp"
#include "UnixDirectory.hpp"
#include "runTests.hpp"

namespace putils {
#ifdef _WIN32
    using Directory = WindowsDirectory;
#elif defined(__unix__)
    using Directory = UnixDirectory;
#endif

    namespace test {
        inline bool directory() {
            return putils::runTests(
                    "for_each", [] {
                        Directory d(".");
                        d.for_each([](auto && f) {
                                       std::cout << f.name << std::endl;
                                       std::cout << "\t" << f.fullPath << std::endl;
                                       std::cout << "\t" << (f.isDirectory ? "Dir" : "NotDir") << std::endl;
                                   }
                        );
                        return true;
                    },
                    "getFiles", [] {
                        Directory d(".");
                        const auto files = d.getFiles();
                        for (const auto & f : files) {
                            std::cout << f.name << std::endl;
                            std::cout << "\t" << f.fullPath << std::endl;
                            std::cout << "\t" << (f.isDirectory ? "Dir" : "NotDir") << std::endl;
                        }
                        return true;
                    }
            );
        }
    }
}
