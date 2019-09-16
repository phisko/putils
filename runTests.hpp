#pragma once

#include <iostream>

namespace putils {
    template<typename ...Tests>
    bool runTests(Tests && ...tests);

    template<>
    inline bool runTests() { return true; }

    template<typename Str, typename Test, typename ...Tests>
    bool runTests(Str && str, Test && test, Tests && ...tests) {
        bool ret = test();
        std::cout << str << ": " << (ret ? "[OK]" : "[KO]") << std::endl;
        return ret && runTests(std::forward<Tests>(tests)...);
    }

    namespace test {
        inline bool runTests() {
            return putils::runTests(
                    "Good test", [] { return true; },
                    "Bad tests", [] { return false; }
            );
        }
    }
}
