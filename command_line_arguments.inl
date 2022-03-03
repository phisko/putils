#pragma once

#include "command_line_arguments.hpp"
#include "to_string.hpp"
#include "reflection.hpp"
#include "string.hpp"

namespace putils {
    namespace impl {
        template<typename T>
        struct remove_optional;

        template<typename T>
        struct remove_optional<std::optional<T>> {
            using type = T;
        };

        template<typename T>
        struct remove_optional {
            using type = T;
        };

        template<typename T>
        static bool parseValue(int ac, const char * const * av, const char * key, T & member, bool isFlag) noexcept {
			using AttributeType = impl::remove_optional<putils_typeof(member)>::type;
            
            for (int i = 0; i < ac; ++i) {
                if (strcmp(av[i], key) == 0) {
                    if constexpr (std::is_same<AttributeType, bool>()) {
                        member = true;
                        return true;
                    }
                    ++i;
                    if (i >= ac)
                        return true;
                    member = putils::parse<AttributeType>(av[i]); 
                    return true;
                }
                else if (!isFlag) {
                    const auto keyLen = strlen(key);
                    if (strncmp(av[i], key, keyLen) != 0 || av[i][keyLen] != '=')
                        continue;
                    const char * value = av[i] + keyLen + 1; // skip the '='
                    member = putils::parse<AttributeType>(value);
                    return true;
                }
            }
            return false;
        }
    }

    template<typename T>
    T parseArguments(int ac, const char * const * av) noexcept {
        T ret;

        putils::reflection::for_each_attribute(ret, [&](const auto & attr) noexcept {
            const putils::string<1024> option("--%s", attr.name);
            if (impl::parseValue(ac, av, option, attr.member, false))
                return;
            if (const auto flag = putils::reflection::get_metadata<const char *>(attr.metadata, "flag")) {
                const putils::string<1024> dashFlag("-%s", *flag);
                impl::parseValue(ac, av, dashFlag, attr.member, true);
            }
        });

        return ret;
    }
}