#pragma once

#include <meta/traits/is_specialization.hpp>

#include "command_line_arguments.hpp"
#include "to_string.hpp"

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
    }

    template<typename T>
    T parseArguments(int ac, const char ** av) noexcept {
        T ret;

        putils::reflection::for_each_attribute(ret, [&](const auto name, auto & attribute) noexcept {
			using AttributeType = impl::remove_optional<putils_typeof(attribute)>::type;
            
            const putils::string<1024> option("--%s", name);
            for (int i = 0; i < ac; ++i) {
                if (av[i] == option) {
					if constexpr (std::is_same<AttributeType, bool>()) {
						attribute = true;
						return;
					}

					++i;
					if (i >= ac)
						return;

					attribute = putils::parse<AttributeType>(av[i]);
                    return;
                }
                else if (strncmp(av[i], option, option.size()) == 0 && av[i][option.size()] == '=') {
                    const char * value = av[i] + option.size() + 1; // skip the '='
                    attribute = putils::parse<AttributeType>(value);
                    return;
                }
            }
        });

        return ret;
    }
}